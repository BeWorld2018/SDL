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

#ifndef SDL_CORE_MORPHOS_LIBRARY_H
#define SDL_CORE_MORPHOS_LIBRARY_H

#ifndef DOS_DOS_H
#include <dos/dos.h>
#endif

#ifndef EXEC_LIBRARIES_H
#include <exec/libraries.h>
#endif

#ifndef EXEC_SEMAPHORES_H
#include <exec/semaphores.h>
#endif

#define SAVEDS __saveds

#if defined(__PPC__)
#define __TEXTSEGMENT__ __attribute__((section(".text")))
#endif

struct CTDT
{
	int	(*fp)(void);
	long	priority;
};

struct HunkSegment
{
	unsigned int Size;
	struct HunkSegment *Next;
};

struct TaskNode
{
	struct MinNode Node;
	struct Task *Task;
};

struct SDL_Library
{
	struct Library Library;
	UWORD          Alloc;
	APTR           DataSeg;	   /* DON'T CHANGE POSITION: offset 36, read by __restore_r13 */

	ULONG                  DataSize;
	struct SDL_Library    *Parent;
	BPTR                   SegList;
	struct ExecBase       *MySysBase;

	/* Caller's TinyGL globals (set by SDL_InitTGL from the -lSDL3 glue):
	   the library writes its TinyGLBase/GL context there so that the
	   application's own gl*() calls reach the context SDL created. */
	struct Library       **MyTinyGLBase;
	void                 **MyGLContext;
	unsigned int		 (*MyGetMaximumContextVersion)(struct Library *TinyGLBase);

	union
	{
		struct MinList TaskList;
		struct TaskNode TaskNode;
	} TaskContext;

	// library management

	struct SignalSemaphore Semaphore;
	APTR ctdtlist;
	APTR last_ctdt;
};

/* The per-opener base (its DataSeg holds this opener's copy of the data) */
extern struct SDL_Library *SDL3Base;

extern BYTE HasAltiVec;

/* Keep the caller's TinyGL globals in sync with the library's own ones */
#define MOS_LIB_SET_TGLBASE(base)    do { if (SDL3Base && SDL3Base->MyTinyGLBase) *SDL3Base->MyTinyGLBase = (base); } while (0)
#define MOS_LIB_SET_TGLCONTEXT(ctx)  do { if (SDL3Base && SDL3Base->MyGLContext) *SDL3Base->MyGLContext = (void *)(ctx); } while (0)

#endif /* SDL_CORE_MORPHOS_LIBRARY_H */
