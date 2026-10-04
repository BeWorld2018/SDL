/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2021 Sam Lantinga <slouken@libsdl.org>

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
#include "../../SDL_internal.h"

/* Thread management routines for SDL */

#include "SDL_thread.h"
#include "../SDL_systhread.h"

#include <exec/execbase.h>
#include <exec/memory.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/threadpool.h>

extern APTR threadpool;

/* threadpool.library runs work items on the minimum system stack (2 KiB 68k,
 * 32 KiB native) and tells to swap stacks when more is needed: threads get
 * the stack size given to SDL_CreateThreadWithStackSize() or
 * SDL_HINT_THREAD_STACK_SIZE, or MOS_THREAD_STACK_DEFAULT. */
#define MOS_THREAD_STACK_MIN     32768
#define MOS_THREAD_STACK_DEFAULT (1024 * 1024)

static ULONG
RunThreadOnStack(ULONG data)
{
	SDL_RunThread((SDL_Thread *)data);
	return 0;
}

static void
RunThread(APTR data, struct MsgPort *port)
{
	SDL_Thread *thread = data;
	ULONG stacksize = thread->stacksize ? thread->stacksize : MOS_THREAD_STACK_DEFAULT;
	APTR stack = NULL;

	BPTR lock = thread->status;
	thread->status = 0;
	BPTR oldDir = CurrentDir(lock);

	if (stacksize > MOS_THREAD_STACK_MIN) {
		stacksize = (stacksize + 15) & ~15;
		stack = AllocVecAligned(stacksize, MEMF_ANY, 16, 0);
	}
	if (stack) {
		struct StackSwapStruct sss;
		struct PPCStackSwapArgs args;

		sss.stk_Lower   = stack;
		sss.stk_Upper   = (ULONG)stack + stacksize;
		sss.stk_Pointer = (APTR)sss.stk_Upper;
		args.Args[0]    = (ULONG)thread;
		NewPPCStackSwap(&sss, (APTR)RunThreadOnStack, &args);
		FreeVec(stack);
	} else {
		/* small stack asked, or no memory: run on the pool stack */
		SDL_RunThread(thread);
	}

	UnLock(CurrentDir(oldDir));
}

int
SDL_SYS_CreateThread(SDL_Thread * thread)
{
	thread->status = Lock("", SHARED_LOCK);
	thread->handle = QueueWorkItem(threadpool, (APTR)RunThread, thread);
	return thread->handle == WORKITEM_INVALID ? -1 : 0;
}

void
SDL_SYS_SetupThread(const char *name)
{
	struct Task *t = SysBase->ThisTask;
	t->tc_Node.ln_Name = (STRPTR)name;
}

SDL_threadID
SDL_ThreadID(void)
{
	return GetCurrentWorkItem(threadpool);
}

int
SDL_SYS_SetThreadPriority(SDL_ThreadPriority priority)
{
	ssize_t pri = 0;

    switch (priority) {
        case SDL_THREAD_PRIORITY_LOW:
            pri = -1;
            break;
        case SDL_THREAD_PRIORITY_HIGH:
            pri = 5;
            break;
        case SDL_THREAD_PRIORITY_TIME_CRITICAL:
            pri = 10;
            break;
        default:
            pri = 0;
            break;
    }
	
	SetTaskPri(SysBase->ThisTask, pri);
	return 0;
}

void
SDL_SYS_WaitThread(SDL_Thread * thread)
{
	WaitWorkItem(threadpool, thread->handle);
}

void
SDL_SYS_DetachThread(SDL_Thread * thread)
{
	thread->handle = WORKITEM_INVALID;
}

/* vi: set ts=4 sw=4 expandtab: */
