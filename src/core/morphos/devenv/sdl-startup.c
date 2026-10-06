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
		/* Functions are added without bumping VERSION: an older 53.x would
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

#include "sdl-stubs.c"
