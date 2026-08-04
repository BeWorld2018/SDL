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
extern void MorphOS_OpenThreadPool(void);

static void RunThread(APTR data, struct MsgPort *port)
{
    SDL_Thread *thread = (SDL_Thread *)data;
    SDL_RunThread(thread);
}

bool SDL_SYS_CreateThread(SDL_Thread *thread, SDL_FunctionPointer pfnBeginThread,
                                 SDL_FunctionPointer pfnEndThread)
{
    if (!threadpool) {
        return SDL_SetError("threadpool.library is not available");
    }

    thread->handle = QueueWorkItem(threadpool, (APTR)RunThread, thread);
    if (thread->handle == WORKITEM_INVALID) {
        return SDL_SetError("Not enough resources to create thread");
    }

    return true;
}

void SDL_SYS_SetupThread(const char *name)
{
	struct Task *t = FindTask(NULL);
	if (name) {
        t->tc_Node.ln_Name = (STRPTR)name;
    }
}

SDL_ThreadID SDL_GetCurrentThreadID(void)
{
    if (!threadpool) {
        MorphOS_OpenThreadPool();
    }

    return (SDL_ThreadID)GetCurrentWorkItem(threadpool);
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
