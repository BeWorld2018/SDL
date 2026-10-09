/*
 * -lSDL3 glue: opens sdl3.library for the program (constructor), gives it
 * the program's TinyGL globals and exit(), plus the functions that must run
 * on the caller's side (varargs, SDL_GL_GetProcAddress).
 *
 * Built three times (see makefile): normal, -mresident32 and
 * -mresident32 -D__NO_SDL_CONSTRUCTORS.
 */

#include <constructor.h>
#include <stdarg.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/muimaster.h>
#include <proto/openurl.h>
#include <tgl/gl.h>

/* Inline LVO calls (SDL_InitTGL/SDL_SetExitPointer have no glue function) */
#include <proto/sdl3.h>

#include "../SDL_mosversion.h"

static void __SDL3_CloseModules(void);

#if defined(__NO_SDL_CONSTRUCTORS)
extern struct Library *SDL3Base;
#else
int _INIT_4_SDL3Base(void) __attribute__((alias("__CSTP_init_SDL3Base")));
void _EXIT_4_SDL3Base(void) __attribute__((alias("__DSTP_cleanup_SDL3Base")));

struct Library *SDL3Base;
struct Library *TinyGLBase;
GLContext      *__tglContext;

/* exit() exists only in programs (libnix startup). Shared libraries linked
   with -nostartfiles (sdl3_image, sdl3_mixer...) pull this object for the
   varargs wrappers but have no exit(): weak reference, NULL there.
   SDL_ExitProcess() handles a NULL exit pointer. */
extern void exit(int) __attribute__((weak));

void __SDL3_OpenLibError(ULONG version, const char *name, ULONG revision)
{
	struct Library *MUIMasterBase = OpenLibrary("muimaster.library", 0);

	if (MUIMasterBase)
	{
		size_t args[3] = { version, revision, (size_t)name };
		LONG ret = MUI_RequestA(NULL, NULL, 0, "SDL3 startup message", "_Ok|_MorphOS-Storage", "You need minimum version %.10ld.%.10ld of %s .\nYou can find last SDL3 package on MorphOS-Storage.net.", &args);
		if (ret == 0)
		{
			static const struct TagItem URLTags[] = {{TAG_DONE, (ULONG) NULL}};
			struct Library *OpenURLBase = OpenLibrary("openurl.library", 0);
			if (OpenURLBase)
			{
				URL_OpenA((STRPTR)"https://www.morphos-storage.net/?find=SDL_3", (struct TagItem*) URLTags);
				CloseLibrary(OpenURLBase);
			}
		}
		CloseLibrary(MUIMasterBase);
	}
}

static const char libname[] = "sdl3.library";
static BPTR OldLock, NewLock;

static CONSTRUCTOR_P(init_SDL3Base, 100)
{
	struct Library *base = OpenLibrary((STRPTR)libname, VERSION);

	if (base)
	{
		/* REVISION = SDL minor * 100 + micro: an older 3.x would
		   lack the vectors this program was linked against. */
		if (!LIB_MINVER(base, VERSION, REVISION))
		{
			CloseLibrary(base);
			base = NULL;
			__SDL3_OpenLibError(VERSION, libname, REVISION);
		}
		else
		{
			NewLock = Lock("PROGDIR:", ACCESS_READ); /* we let libauto open doslib */
			if (NewLock)
			{
				OldLock = CurrentDir(NewLock);

				SDL3Base = base;

				SDL_InitTGL((void **) &__tglContext, (struct Library **) &TinyGLBase, TGLGetMaximumContextVersion);

				/* Used by SDL_ExitProcess() */
				if (exit)
					SDL_SetExitPointer(exit);
			}
			else
			{
				CloseLibrary(base);
				base = NULL;
			}
		}
	}
	else
	{
		__SDL3_OpenLibError(VERSION, libname, REVISION);
	}

	return (base == NULL);
}

static DESTRUCTOR_P(cleanup_SDL3Base, 100)
{
	struct Library *base = SDL3Base;

	__SDL3_CloseModules();

	if (base)
	{
		if (NewLock)
		{
			UnLock(CurrentDir(OldLock));
			NewLock = 0;
		}
		CloseLibrary(base);
		SDL3Base = NULL;
	}
}

#endif

/* The caller's own TinyGL entry points, bound to its __tglContext (which
   sdl3.library keeps in sync with the current SDL GL context) */
SDL_FunctionPointer SDL_GL_GetProcAddress(const char *proc)
{
	return (SDL_FunctionPointer)tglGetProcAddress(proc);
}

/* SDL_LoadObject/SDL_LoadFunction/SDL_UnloadObject through dynload.library. */
#define __NOLIBBASE__
#include <proto/dynload.h>
#undef __NOLIBBASE__

static struct Library *DynLoadBase;
static void **__sdl3_modules;
static int __sdl3_num_modules, __sdl3_max_modules;

SDL_SharedObject *SDL_LoadObject(const char *sofile)
{
	void *handle;
	const char *loaderror;

	if (!DynLoadBase)
	{
		DynLoadBase = OpenLibrary("dynload.library", 0);
		if (!DynLoadBase)
		{
			SDL_SetError("Failed loading %s: dynload.library is not available", sofile);
			return NULL;
		}
	}

	handle = dlopen(sofile, RTLD_NOW | RTLD_LOCAL);
	loaderror = dlerror();
	if (!handle)
	{
		SDL_SetError("Failed loading %s: %s", sofile, loaderror ? loaderror : "unknown error");
		return NULL;
	}

	Forbid();  /* threads may load modules too */
	if (__sdl3_num_modules == __sdl3_max_modules)
	{
		const int newmax = __sdl3_max_modules ? __sdl3_max_modules * 2 : 8;
		void **list = AllocVec(newmax * sizeof(void *), MEMF_ANY);
		if (list)
		{
			if (__sdl3_modules)
			{
				CopyMem(__sdl3_modules, list, __sdl3_num_modules * sizeof(void *));
				FreeVec(__sdl3_modules);
			}
			__sdl3_modules = list;
			__sdl3_max_modules = newmax;
		}
	}
	if (__sdl3_num_modules < __sdl3_max_modules)
		__sdl3_modules[__sdl3_num_modules++] = handle;
	Permit();

	return (SDL_SharedObject *)handle;
}

SDL_FunctionPointer SDL_LoadFunction(SDL_SharedObject *handle, const char *name)
{
	void *symbol;

	if (!handle || !DynLoadBase)
	{
		SDL_SetError("Failed loading %s: no module", name);
		return NULL;
	}

	symbol = dlsym(handle, name);
	if (!symbol)
	{
		/* prepend an underscore, as the generic dlopen backend does */
		char _name[256];

		_name[0] = '_';
		SDL_strlcpy(&_name[1], name, sizeof(_name) - 1);
		symbol = dlsym(handle, _name);
		if (!symbol)
		{
			const char *err = dlerror();
			SDL_SetError("Failed loading %s: %s", name, err ? err : "symbol not found");
		}
	}
	return (SDL_FunctionPointer)symbol;
}

void SDL_UnloadObject(SDL_SharedObject *handle)
{
	int i;

	if (!handle || !DynLoadBase)
		return;

	Forbid();
	for (i = __sdl3_num_modules - 1; i >= 0; i--)
	{
		if (__sdl3_modules[i] == handle)
		{
			__sdl3_modules[i] = __sdl3_modules[--__sdl3_num_modules];
			break;
		}
	}
	Permit();

	dlclose(handle);
}

static void __attribute__((unused)) __SDL3_CloseModules(void)
{
	/* last loaded first */
	while (__sdl3_num_modules > 0)
		dlclose(__sdl3_modules[--__sdl3_num_modules]);

	if (__sdl3_modules)
	{
		FreeVec(__sdl3_modules);
		__sdl3_modules = NULL;
		__sdl3_max_modules = 0;
	}
	if (DynLoadBase)
	{
		CloseLibrary(DynLoadBase);
		DynLoadBase = NULL;
	}
}

#include "sdl-stubs.c"
