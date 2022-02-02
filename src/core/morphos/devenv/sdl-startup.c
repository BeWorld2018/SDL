/*
 * Copyright (C) 2004-2017 MorphOS Development Team
 *
 * $Id: sdl-startup.c,v 1.2 2017/11/25 21:56:56 itix Exp $
 */

#include <constructor.h>
#include <stdio.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/muimaster.h>
#include <proto/sdl2.h>
#include <proto/tinygl.h>
#include <proto/openurl.h>

#include "SDL_error.h"
#include "SDL_rwops.h"
#include "SDL_version.h"

#include "../SDL_amigaversion.h"

#if defined(__NO_SDL_CONSTRUCTORS)
extern struct Library *SDL2Base;
extern struct Library *OpenURLBase;
#else
int _INIT_4_SDL2Base(void) __attribute__((alias("__CSTP_init_SDL2Base")));
void _EXIT_4_SDL2Base(void) __attribute__((alias("__DSTP_cleanup_SDL2Base")));
UWORD TinyGl_NewVersion = 0;

struct Library *SDL2Base;
struct Library *TinyGLBase;
struct Library *OpenURLBase;
GLContext      *__tglContext;

void __SDL2_OpenLibError(ULONG version, const char *name)
{
	struct Library *MUIMasterBase = OpenLibrary("muimaster.library", 0);

	if (MUIMasterBase)
	{
		size_t args[5] = { version, (size_t)name, SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL};
		LONG ret = MUI_RequestA(NULL, NULL, 0, "SDL2 startup message", "_Ok|_MorphOS-Storage", "You need version %.10ld of %s.\nYou can find %.10ld.%.10ld.%.10ld package on MorphOS-Storage.org.", &args);
		if (ret == 0){
			static const struct TagItem URLTags[] = {{TAG_DONE, (ULONG) NULL}};
			if (OpenURLBase) {
				URL_OpenA((STRPTR)"https://www.morphos-storage.net/?find=SDL_2", (struct TagItem*) URLTags);
			}
		}
		CloseLibrary(MUIMasterBase);
	}
}

static const char libname[] = "sdl2.library";
static BPTR OldLock, NewLock;

static CONSTRUCTOR_P(init_SDL2Base, 100)
{
	struct Library *base = (void *) OpenLibrary((STRPTR)libname, VERSION);

	if (base)
	{
		NewLock = Lock("PROGDIR:", ACCESS_READ); /* we let libauto open doslib */

		if(NewLock)
		{
			SDL2Base = base;
			SDL_InitTGL((void **) &__tglContext, (struct Library **) &TinyGLBase);
			OldLock = CurrentDir(NewLock);
		}
		else
		{
			CloseLibrary(base);
		}
	}
	else
	{
		__SDL2_OpenLibError(VERSION, libname);
	}

	return (base == NULL);
}

static DESTRUCTOR_P(cleanup_SDL2Base, 100)
{
	struct Library *base = SDL2Base;

	if (base)
	{
		if (NewLock)
		{
			//CurrentDir(OldLock);
			//UnLock(NewLock);
			UnLock(CurrentDir(OldLock));
		}

		CloseLibrary(base);
	}
}
#endif

#include "sdl-stubs.c"

/* SDL_RWops() replacement
 *
 * Functions to read/write stdio file pointers. Will link with libc stdio.
 */

static Sint64 stdio_size(struct SDL_RWops *context)
{
	Sint64 size, offset;

	offset = ftello64(context->hidden.amigaio.fp.libc);
	fseeko64(context->hidden.amigaio.fp.libc, 0, SEEK_END);
	size = ftello64(context->hidden.amigaio.fp.libc);
	fseeko64(context->hidden.amigaio.fp.libc, offset, SEEK_SET);

	return size;
}

static Sint64 stdio_seek(SDL_RWops *context, Sint64 offset, int whence)
{
	if (fseeko64(context->hidden.amigaio.fp.libc, offset, whence) == 0 )
	{
		return ftello64(context->hidden.amigaio.fp.libc);
	}
	else
	{
		SDL_Error(SDL_EFSEEK);
		return -1;
	}
}
static size_t stdio_read(SDL_RWops *context, void *ptr, size_t size, size_t maxnum)
{
	size_t nread = fread(ptr, size, maxnum, context->hidden.amigaio.fp.libc);

	if (nread == 0 && ferror((FILE *)context->hidden.amigaio.fp.libc) )
	{
		SDL_Error(SDL_EFREAD);
	}

	return nread;
}
static size_t stdio_write(SDL_RWops *context, const void *ptr, size_t size, size_t num)
{
	size_t nwrote = fwrite(ptr, size, num, context->hidden.amigaio.fp.libc);

	if (nwrote == 0 && ferror((FILE *)context->hidden.amigaio.fp.libc) )
	{
		SDL_Error(SDL_EFWRITE);
	}

	return nwrote;
}
static int stdio_close(struct SDL_RWops *context)
{
	if ( context )
	{
		if ( context->hidden.amigaio.autoclose )
		{
			/* WARNING:  Check the return value here! */
			fclose(context->hidden.amigaio.fp.libc);
		}
		SDL_FreeRW(context);
	}
	return 0;
}

SDL_RWops *SDL_RWFromFP(void *fp, SDL_bool autoclose)
{
	return SDL_RWFromFP_clib(fp, autoclose, stdio_size, stdio_seek, stdio_read, stdio_write, stdio_close);
}

extern void *AmiGetGLProc(const char *proc);
void *SDL_GL_GetProcAddress(const char *proc)
{
	void *func = NULL;

	func = AmiGetGLProc(proc);

	D("[%s] proc %s func 0x%08lx\n", __FUNCTION__, proc, func);

	return func;
}
