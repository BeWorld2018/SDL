/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2026 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include <constructor.h>
#include <stddef.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/muimaster.h>
#include <proto/openurl.h>
#include <proto/sdl3.h>
#include <proto/tinygl.h>
#include <tgl/gl.h>

#include "../SDL_mosversion.h"

static const char libname[] = "sdl3.library";

#if defined(__NO_SDL_CONSTRUCTORS)

extern struct Library *SDL3Base;

#else

struct Library *SDL3Base = NULL;

struct Library *TinyGLBase;
GLContext      *__tglContext;

int  _INIT_4_SDL3Base(void) __attribute__((alias("__CSTP_init_SDL3Base")));
void _EXIT_4_SDL3Base(void) __attribute__((alias("__DSTP_cleanup_SDL3Base")));

static BPTR OldDirLock, ProgDirLock;

static void SDL3_OpenLibError(void)
{
	struct Library *MUIMasterBase = OpenLibrary("muimaster.library", 0);

	if (MUIMasterBase) {
		size_t args[1] = { (size_t)libname };
		LONG ret = MUI_RequestA(NULL, NULL, 0, "SDL3 startup message",
			"_Ok|_MorphOS-Storage",
			"This program needs %s version "
			MOS_XSTR(SDL_MAJOR_VERSION) "." MOS_XSTR(SDL_MINOR_VERSION) "." MOS_XSTR(SDL_MICRO_VERSION)
			" or newer.\n"
			"Get the latest SDL3 package on MorphOS-Storage.net.",
			&args);
		if (ret == 0) {
			static const struct TagItem URLTags[] = { { TAG_DONE, (ULONG)NULL } };
			struct Library *OpenURLBase = OpenLibrary("openurl.library", 0);
			if (OpenURLBase) {
				URL_OpenA((STRPTR)"https://www.morphos-storage.net/?find=SDL_3",
				          (struct TagItem *)URLTags);
				CloseLibrary(OpenURLBase);
			}
		}
		CloseLibrary(MUIMasterBase);
	}
}

static CONSTRUCTOR_P(init_SDL3Base, 100)
{
	struct Library *base = OpenLibrary((STRPTR)libname, SDL_MAJOR_VERSION);

	if (!base) {
		SDL3_OpenLibError();
		return 1;
	}

	SDL3Base = base;

	if (SDL_GetVersion() < SDL_VERSION) {
		SDL3Base = NULL;
		CloseLibrary(base);
		SDL3_OpenLibError();
		return 1;
	}

	SDL_InitTGL((void **)&__tglContext, (struct Library **)&TinyGLBase);

	ProgDirLock = Lock("PROGDIR:", ACCESS_READ);
	if (ProgDirLock) {
		OldDirLock = CurrentDir(ProgDirLock);
	}

	return 0;
}

static DESTRUCTOR_P(cleanup_SDL3Base, 100)
{
	if (SDL3Base) {
		if (ProgDirLock) {
			UnLock(CurrentDir(OldDirLock));
			ProgDirLock = 0;
		}
		CloseLibrary(SDL3Base);
		SDL3Base = NULL;
	}
}

#endif /* !__NO_SDL_CONSTRUCTORS */

#include "sdl-stubs.c"
