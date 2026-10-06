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

/* Per-opener setup, runs with the opener's r13 (data copy) */

#include <exec/types.h>
#include <proto/exec.h>

#include "SDL_library.h"
#include "SDL_startup.h"

/*********************************************************************/

struct SDL_Library *SDL3Base;

int ThisRequiresConstructorHandling = 0;

/* libnix malloc() pool of this opener: created lazily by malloc(), deleted
   by the libnix __exitmalloc destructor (run by MOS_Cleanup) */
APTR libnix_mempool;

/* This function must preserve all registers except r13 */
asm
("\n"
"	.section \".text\"\n"
"	.align 2\n"
"	.type __restore_r13, @function\n"
"__restore_r13:\n"
"	lwz 13, 36(3)\n"
"	blr\n"
"__end__restore_r13:\n"
"	.size __restore_r13, __end__restore_r13 - __restore_r13\n"
);

/* src/SDL.c: what the static library runs from its constructor/destructor */
extern void MorphOS_LibStartup(void);
extern void MorphOS_LibCleanup(void);
extern void SDL_Quit(void);
/* src/loadso/morphos/libdll/dll.c: DLLs still loaded by this opener */
extern void dllLibCleanup(void);

/**********************************************************************
	Startup/Cleanup
**********************************************************************/

int SAVEDS MOS_Startup(struct SDL_Library *LibBase)
{
	struct CTDT *ctdt = LibBase->ctdtlist, *last_ctdt = LibBase->last_ctdt;

	SDL3Base = LibBase;

	// Run constructors
	while (ctdt < last_ctdt)
	{
		if (ctdt->priority >= 0 && ctdt->fp != (int (*)(void)) -1)
		{
			if (ctdt->fp() != 0)
				return 0;
		}

		ctdt++;
	}

	MorphOS_LibStartup();

	return 1;
}

VOID SAVEDS MOS_Cleanup(struct SDL_Library *LibBase)
{
	struct CTDT *ctdt = LibBase->ctdtlist, *last_ctdt = LibBase->last_ctdt;

	SDL_Quit();

	dllLibCleanup();
	MorphOS_LibCleanup();

	// Run destructors
	while (ctdt < last_ctdt)
	{
		if (ctdt->priority < 0 && ctdt->fp != (int (*)(void)) -1)
		{
			ctdt->fp();
		}

		ctdt++;
	}
}

/**********************************************************************
	Library setup
**********************************************************************/

void (*morphos_exit)(int exitcode);

void SAVEDS LIB_SetExitPointer(struct SDL_Library *base, void (*exitfunc)(int))
{
	morphos_exit = exitfunc;
}

/**********************************************************************
	LIB_InitTGL

	Called by the -lSDL3 glue constructor: where to mirror TinyGLBase and
	the current GL context for the caller.
**********************************************************************/

VOID LIB_InitTGL(struct SDL_Library *base, void **glc, struct Library **tgl, unsigned int (*getmaximumcontextversion)(struct Library *TinyGLBase))
{
	if (base->MyTinyGLBase == NULL)
	{
		base->MyTinyGLBase = tgl;
		base->MyGLContext = glc;
		base->MyGetMaximumContextVersion = getmaximumcontextversion;
	}
}

/* Slots of functions SDL doesn't build on MorphOS (see SDL_stubs.h) */
int LIB_Unsupported(void)
{
	return 0;
}

void __chkabort(void) { }
void abort(void) { for (;;) Wait(0); }
