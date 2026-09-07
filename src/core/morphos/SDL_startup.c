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

#include <exec/types.h>

#include "SDL_library.h"
#include "SDL_startup.h"
#include "SDL_mos_runtime.h"

int  ThisRequiresConstructorHandling = 0;
APTR libnix_mempool;

struct SDL_Library *SDL3Base;

__attribute__((noreturn)) void exit(int rc);
__attribute__((noreturn)) void exit(int rc)
{
    (void)rc;
    for (;;) { }
}

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

void LIB_InitTGL(struct SDL_Library *base, void **glcptr, struct Library **tglptr)
{
    if (base && base->MyGLContext == NULL) {
        base->MyGLContext  = glcptr;
        base->MyTinyGLBase = tglptr;
    }
}

int SAVEDS MOS_Startup(struct SDL_Library *LibBase)
{
    struct CTDT *ctdt      = LibBase->ctdtlist;
    struct CTDT *last_ctdt = LibBase->last_ctdt;

    SDL3Base = LibBase;

    MorphOS_OpenThreadPoolWithSegment(LibBase->DataSeg);

    while (ctdt < last_ctdt)
    {
        if (ctdt->priority >= 0 && ctdt->fp != (int (*)(void)) -1)
        {
            if (ctdt->fp() != 0)
                return 0;
        }
        ctdt++;
    }

    return 1;
}

VOID SAVEDS MOS_Cleanup(struct SDL_Library *LibBase)
{
    extern void SDL_Quit();
    struct CTDT *ctdt      = LibBase->ctdtlist;
    struct CTDT *last_ctdt = LibBase->last_ctdt;

    SDL_Quit();

    MorphOS_CloseThreadPool();

    while (ctdt < last_ctdt)
    {
        if (ctdt->priority < 0 && ctdt->fp != (int (*)(void)) -1)
            ctdt->fp();
        ctdt++;
    }
}

void __chkabort(void) { }
