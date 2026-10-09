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
    SDL_AtomicInt waiters;     /* tasks inside SDL_WaitSemaphoreTimeoutNS() that still use sem */
};

extern void MorphOS_InitQPort(struct MsgPort *port);
extern void MorphOS_FreeQPort(struct MsgPort *port);
extern struct timerequest GlobalTimeReq;

static
void mywaitinit(struct timerequest *r, Uint32 timeout, struct waitnode *wn)
{
    struct timerequest *req = &GlobalTimeReq;

    r->tr_node.io_Message.mn_Node.ln_Type = NT_REPLYMSG;
    r->tr_node.io_Message.mn_ReplyPort    = &wn->port;
    r->tr_node.io_Device                  = req->tr_node.io_Device;
    r->tr_node.io_Unit                    = req->tr_node.io_Unit;
}

SDL_Semaphore *SDL_CreateSemaphore(Uint32 initial_value)
{
    SDL_Semaphore *sem;

    sem = (SDL_Semaphore *) SDL_malloc(sizeof(*sem));
    if (sem != NULL) {
		sem->sem_value = initial_value;

		NEWLIST(&sem->waitlist);
		memset(&sem->sem, 0, sizeof(sem->sem));
		InitSemaphore(&sem->sem);
		SDL_SetAtomicInt(&sem->waiters, 0);
	} else {
		SDL_OutOfMemory();      
    }
    return sem;
}

void SDL_DestroySemaphore(SDL_Semaphore *sem)
{
    if (sem) {
        struct waitnode *wn;

        ObtainSemaphore(&sem->sem);

        /* Wake up every waiter (they return true, as with the generic
           implementation), and let anyone arriving late through */
        sem->sem_value = (Uint32)-1;

        while ((wn = (struct waitnode *)REMHEAD(&sem->waitlist)) != NULL) {
            ReplyMsg(&wn->msg);
        }

        ReleaseSemaphore(&sem->sem);

        /* They still touch sem (lock, timer cleanup) before returning */
        while (SDL_GetAtomicInt(&sem->waiters) > 0) {
            SDL_Delay(1);
        }

        SDL_free(sem);
    }
}

bool SDL_WaitSemaphoreTimeoutNS(SDL_Semaphore *sem, Sint64 timeoutNS)
{
    bool retval = false;
    struct waitnode wn;
	SDL_zero(wn);

    if (sem == NULL) {
        return SDL_InvalidParamError("sem");
	}

    ObtainSemaphore(&sem->sem);

    if (sem->sem_value > 0) {
        --sem->sem_value;
        retval = true;
    }
    else if (timeoutNS != 0) {
        MorphOS_InitQPort(&wn.port);
        wn.msg.mn_Node.ln_Type = NT_MESSAGE;
        wn.msg.mn_ReplyPort = &wn.port;
        ADDTAIL(&sem->waitlist, &wn);
        SDL_AddAtomicInt(&sem->waiters, 1);
    }

    ReleaseSemaphore(&sem->sem);

    if (retval || timeoutNS == 0) {
        return retval;
    }

    if (timeoutNS < 0) {
        /* Infinite wait */
        WaitPort(&wn.port);
        GetMsg(&wn.port);
        retval = true;
    } else if (!GlobalTimeReq.tr_node.io_Device || !GlobalTimeReq.tr_node.io_Unit) {
        /* No timer.device: can't wait for a bounded time */
        ObtainSemaphore(&sem->sem);
        if (wn.msg.mn_Node.ln_Type == NT_REPLYMSG) {
            retval = true;  /* signalled meanwhile */
        } else {
            REMOVE(&wn);
        }
        ReleaseSemaphore(&sem->sem);
    } else {
        /* Sem not available and we have a bounded timeout */
        struct timerequest req;
        struct Message *msg;

		SDL_zero(req);
        mywaitinit(&req, timeoutNS, &wn);

        req.tr_node.io_Command = TR_ADDREQUEST;
		req.tr_time.tv_secs =
			timeoutNS / SDL_NS_PER_SECOND;

		req.tr_time.tv_micro =
			(timeoutNS / SDL_NS_PER_US) % 1000000;
        SendIO((struct IORequest *) &req);

        msg = WaitPort(&wn.port);
        retval = true;

        if (msg != &wn.msg) {
            /* Timer first: still waiting, unless signalled meanwhile (then
               wn.msg is in wn.port, which goes away with this frame) */
            ObtainSemaphore(&sem->sem);
            if (wn.msg.mn_Node.ln_Type == NT_REPLYMSG) {
                retval = true;
            } else {
                REMOVE(&wn);
                retval = false;
            }
            ReleaseSemaphore(&sem->sem);
        }

        AbortIO((struct IORequest *) &req);
        WaitIO((struct IORequest *) &req);
    }

    /* Nothing can be replied to wn.port any more */
    MorphOS_FreeQPort(&wn.port);

    /* Last access to sem: SDL_DestroySemaphore() may free it from now on */
    SDL_AddAtomicInt(&sem->waiters, -1);

    return retval;
}

Uint32 SDL_GetSemaphoreValue(SDL_Semaphore *sem)
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

    if ((wn = (APTR)REMHEAD(&sem->waitlist))) {
        sem->sem_value--;
        ReplyMsg(&wn->msg);
    }

    ReleaseSemaphore(&sem->sem);

}
