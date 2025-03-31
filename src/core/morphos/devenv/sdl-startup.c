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
#include <proto/sdl3.h>
#include <proto/tinygl.h>
#include <tgl/gl.h>
#include <proto/openurl.h>

#include "SDL_error.h"
#include "SDL_version.h"

#include "../SDL_mosversion.h"

#if defined(__NO_SDL_CONSTRUCTORS)
extern struct Library *SDL3Base;
#else
int _INIT_4_SDL3Base(void) __attribute__((alias("__CSTP_init_SDL3Base")));
void _EXIT_4_SDL3Base(void) __attribute__((alias("__DSTP_cleanup_SDL3Base")));

struct Library *SDL3Base;
struct Library *TinyGLBase;
GLContext      *__tglContext;

void __SDL3_OpenLibError(ULONG version, const char *name, ULONG revision)
{
	struct Library *MUIMasterBase = OpenLibrary("muimaster.library", 0);

	if (MUIMasterBase)
	{
		if (!revision) revision = 0;
		size_t args[3] = { version, revision, (size_t)name};
		LONG ret = MUI_RequestA(NULL, NULL, 0, "SDL3 startup message", "_Ok|_MorphOS-Storage", "You need minimum version %.10ld.%.10ld of %s .\nYou can find last SDL3 package on MorphOS-Storage.net.", &args);
		if (ret == 0){
			static const struct TagItem URLTags[] = {{TAG_DONE, (ULONG) NULL}};
			struct Library *OpenURLBase = OpenLibrary("openurl.library", 0);
			if (OpenURLBase) {
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
	struct Library *base = (void *) OpenLibrary((STRPTR)libname, VERSION);

	if (base)
	{
		if (!LIB_MINVER(base, VERSION, REVISION))
		{
			CloseLibrary(base);
			base = NULL;
			__SDL3_OpenLibError(VERSION, libname, REVISION);
			
		} 
		else 
		{	
			NewLock = Lock("PROGDIR:", ACCESS_READ); /* we let libauto open doslib */
			if(NewLock)
			{
				OldLock = CurrentDir(NewLock);
				SDL3Base = base;
				SDL_InitTGL((void **) &__tglContext, (struct Library **) &TinyGLBase, TGLGetMaximumContextVersion);
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
		}
		CloseLibrary(base);
		base = NULL;
	}
}
#endif

#include "sdl-stubs.c"

SDL_FunctionPointer SDL_GL_GetProcAddress(const char *proc)
{
	SDL_FunctionPointer func = NULL;

	func = tglGetProcAddress(proc); 
	
	return func;
}
