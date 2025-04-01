/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2023 Sam Lantinga <slouken@libsdl.org>

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

/* An implementation of semaphores using mutexes and condition variables */

#include "SDL_timer.h"
#include "SDL_thread.h"
#include "SDL_systhread_c.h"

#include <devices/timer.h>
#include <dos/dos.h>
#include <exec/execbase.h>
#include <proto/exec.h>

struct waitnode
{
	struct Message msg;
	struct MsgPort port;
};

struct SDL_Semaphore
{
    Uint32 sem_value;
    struct MinList waitlist;
    struct SignalSemaphore sem;
};

extern void InitQPort(struct MsgPort *port);

static
void mywaitinit(struct timerequest *r, Uint32 timeout, struct waitnode *wn)
{
    extern struct timerequest GlobalTimeReq;
    struct timerequest *req = &GlobalTimeReq;

    r->tr_node.io_Message.mn_Node.ln_Type = NT_REPLYMSG;
    r->tr_node.io_Message.mn_ReplyPort    = &wn->port;
    r->tr_node.io_Device                  = req->tr_node.io_Device;
    r->tr_node.io_Unit                    = req->tr_node.io_Unit;
}

SDL_Semaphore *
SDL_CreateSemaphore(Uint32 initial_value)
{
    SDL_Semaphore *sem;

    sem = (SDL_Semaphore *) SDL_malloc(sizeof(*sem));
    if (sem != NULL) {
		sem->sem_value = initial_value;

		NEWLIST(&sem->waitlist);
		memset(&sem->sem, 0, sizeof(sem->sem));
		InitSemaphore(&sem->sem);
	} else {
		SDL_OutOfMemory();      
    }
    return sem;
}

/* WARNING:
   You cannot call this function when another thread is using the semaphore.
*/
void
SDL_DestroySemaphore(SDL_Semaphore * sem)
{
    if (sem != NULL) {
        ObtainSemaphore(&sem->sem);

        sem->sem_value = (Uint32)-1;

        while (!IsListEmpty((struct List *) &sem->waitlist)) {
            struct waitnode *wn;

            for (wn = (struct waitnode *) sem->waitlist.mlh_Head; wn->msg.mn_Node.ln_Succ; wn = (struct waitnode *) wn->msg.mn_Node.ln_Succ)
                ReplyMsg(&wn->msg);

            if (!SDL_WaitSemaphoreTimeout(sem, 10))
                break;
        }

        ReleaseSemaphore(&sem->sem);

        SDL_free(sem);
    }
}

bool
SDL_WaitSemaphoreTimeoutNS(SDL_Semaphore * sem, Sint64 timeoutNS)
{
    int retval = false;
    struct waitnode wn;

    if (sem == NULL) {
        return SDL_InvalidParamError("sem");
	}

    /* Try semaphore */
    ObtainSemaphore(&sem->sem);

    if (sem->sem_value > 0) {
        --sem->sem_value;
        retval = true;
    }
    else if (timeoutNS > 0) {
        InitQPort(&wn.port);
        wn.msg.mn_Node.ln_Type = NT_MESSAGE;
        wn.msg.mn_ReplyPort = &wn.port;
        ADDTAIL(&sem->waitlist, &wn);
    }

    ReleaseSemaphore(&sem->sem);

    /* Sem not available and we have timeout */
    if (retval == false && timeoutNS > 0) {
        struct timerequest req;
        struct Message *msg;

        mywaitinit(&req, timeoutNS, &wn);

        req.tr_node.io_Command = TR_ADDREQUEST;
        req.tr_time.tv_secs  = timeoutNS / SDL_NS_PER_SECOND;
        req.tr_time.tv_micro = (timeoutNS % SDL_NS_PER_SECOND) * SDL_NS_PER_SECOND;
        SendIO((struct IORequest *) &req);

        msg = WaitPort(&wn.port);
        retval = true;

        if (msg != &wn.msg) {
            ObtainSemaphore(&sem->sem);
            REMOVE(&wn);
            retval = wn.msg.mn_Node.ln_Type == NT_REPLYMSG ?  true : false;
            ReleaseSemaphore(&sem->sem);
        }

        AbortIO((struct IORequest *) &req);
        WaitIO((struct IORequest *) &req);
    }

    return retval;
}

Uint32
SDL_GetSemaphoreValue(SDL_Semaphore * sem)
{
    if (sem == NULL) {
        SDL_InvalidParamError("sem");
        return 0;
    }

    return (Uint32)sem->sem_value;
}

void SDL_SignalSemaphore(SDL_Semaphore *sem)
{
    if (!sem) {
        return;
    }
	struct waitnode *wn;

    ObtainSemaphore(&sem->sem);

    sem->sem_value++;

   	/* Wake whatever task happens to be first in the waitlist */
    if ((wn = (APTR)REMHEAD(&sem->waitlist))) {
        sem->sem_value--;
        ReplyMsg(&wn->msg);
    }

    ReleaseSemaphore(&sem->sem);

}
