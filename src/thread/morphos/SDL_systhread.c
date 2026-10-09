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
#include "SDL_internal.h"

/* Thread management routines for SDL */

#include "SDL_thread.h"
#include "../SDL_systhread.h"

#include <exec/execbase.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <libraries/threadpool.h>
#include <proto/threadpool.h>

extern APTR threadpool;
extern struct Task *threadpool_owner;
extern void MorphOS_OpenThreadPool(void);

/* threadpool.library runs work items on the pool's own stack (32 KiB
   native): like the SDL2 port, threads get the stack size given to
   SDL_CreateThreadWithStackSize() (SDL_PROP_THREAD_CREATE_STACKSIZE_NUMBER)
   or MOS_THREAD_STACK_DEFAULT, through NewPPCStackSwap(). */
#define MOS_THREAD_STACK_MIN     32768
#define MOS_THREAD_STACK_DEFAULT (1024 * 1024)

/* What the worker needs, owned by RunThread() (thread->name may be freed
   by SDL_RunThread() for a detached thread, the task name uses a copy) */
typedef struct MOS_ThreadStart
{
    SDL_Thread *thread;
    BPTR        dir;        /* creator's current dir, for relative paths */
    char        name[64];
} MOS_ThreadStart;

static ULONG RunThreadOnStack(ULONG data)
{
    SDL_RunThread((SDL_Thread *)data);
    return 0;
}

static void RunThread(APTR data, struct MsgPort *port)
{
    MOS_ThreadStart *start = (MOS_ThreadStart *)data;
    SDL_Thread *thread = start->thread;
    struct Task *task = FindTask(NULL);
    size_t stacksize = thread->stacksize ? thread->stacksize : MOS_THREAD_STACK_DEFAULT;
    APTR stack = NULL;

    /* The worker is reused by the next work item: its name, priority and
       current dir are put back afterwards */
    STRPTR oldname = task->tc_Node.ln_Name;
    const LONG oldpri = task->tc_Node.ln_Pri;
    BPTR olddir = 0;

    if (start->name[0]) {
        task->tc_Node.ln_Name = (STRPTR)start->name;
    }
    if (start->dir) {
        olddir = CurrentDir(start->dir);
    }

    if (stacksize > MOS_THREAD_STACK_MIN) {
        stacksize = (stacksize + 15) & ~15;
        stack = AllocVecAligned(stacksize, MEMF_ANY, 16, 0);
    }
    if (stack) {
        struct StackSwapStruct sss;
        struct PPCStackSwapArgs args;

        SDL_zero(args);
        sss.stk_Lower   = stack;
        sss.stk_Upper   = (ULONG)stack + stacksize;
        sss.stk_Pointer = (APTR)sss.stk_Upper;
        args.Args[0]    = (ULONG)thread;
        NewPPCStackSwap(&sss, (APTR)RunThreadOnStack, &args);
        FreeVec(stack);
    } else {
        // small stack asked, or no memory: run on the pool stack
        SDL_RunThread(thread);
    }

    if (start->dir) {
        UnLock(CurrentDir(olddir));
    }
    SetTaskPri(task, oldpri);
    task->tc_Node.ln_Name = oldname;

    SDL_free(start);
}

bool SDL_SYS_CreateThread(SDL_Thread *thread, SDL_FunctionPointer pfnBeginThread,
                                 SDL_FunctionPointer pfnEndThread)
{
    MOS_ThreadStart *start;
    struct Task *me;

    if (!threadpool) {
        MorphOS_OpenThreadPool();
    }
    if (!threadpool) {
        return SDL_SetError("threadpool.library is not available");
    }

    start = (MOS_ThreadStart *)SDL_calloc(1, sizeof(*start));
    if (!start) {
        return false;
    }
    start->thread = thread;
    if (thread->name) {
        SDL_strlcpy(start->name, thread->name, sizeof(start->name));
    }

    // dos.library calls are for processes only
    me = FindTask(NULL);
    if (me->tc_Node.ln_Type == NT_PROCESS) {
        start->dir = DupLock(((struct Process *)me)->pr_CurrentDir);
    }

    thread->handle = QueueWorkItem(threadpool, (APTR)RunThread, start);
    if (thread->handle == WORKITEM_INVALID) {
        if (start->dir) {
            UnLock(start->dir);
        }
        SDL_free(start);
        return SDL_SetError("Not enough resources to create thread");
    }

    return true;
}

void SDL_SYS_SetupThread(const char *name)
{
    // Done by RunThread(), on a copy of the name
    (void)name;
}

SDL_ThreadID SDL_GetCurrentThreadID(void)
{
    /* The Task address for every task: GetCurrentWorkItem() is only valid
       on the pool's workers (called from the main task it was seen reading
       a bogus pointer), and the program can call SDL from tasks of its own.
       A worker reused by a later thread gets the same ID, as a reused
       pthread would. */
    return (SDL_ThreadID)(IPTR)FindTask(NULL);
}

bool SDL_SYS_SetThreadPriority(SDL_ThreadPriority priority)
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

	SetTaskPri(FindTask(NULL), pri);
	return true;
}

void SDL_SYS_WaitThread(SDL_Thread *thread)
{
    WaitWorkItem(threadpool, thread->handle);
}

void SDL_SYS_DetachThread(SDL_Thread *thread)
{
    /* Nothing to release: the pool reclaims the work item's resources on
       its own once RunThread() returns, whether or not anyone ever waits
       on it. */
}
