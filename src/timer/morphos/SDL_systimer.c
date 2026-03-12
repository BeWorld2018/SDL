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

#ifdef SDL_TIMER_MORPHOS

#include <exec/execbase.h>
#include <devices/timer.h>
#include <proto/exec.h>
#include <proto/timer.h>

extern struct timerequest GlobalTimeReq;

Uint64 SDL_GetPerformanceCounter(void)
{
    Uint64 val = 0;
    ReadCPUClock(&val);
    return val;
}

Uint64 SDL_GetPerformanceFrequency(void)
{
    Uint64 val = 0;
    return ReadCPUClock(&val);
}

static void MorphOS_InitQPort(struct MsgPort *port)
{
    port->mp_SigBit = SIGB_SINGLE;
    port->mp_Flags = PA_SIGNAL;
    port->mp_SigTask = SysBase->ThisTask;
    NEWLIST(&port->mp_MsgList);
}

static void MorphOS_NormalizeTimeval(struct timeval *tv)
{
    if (!tv) {
        return;
    }

    while (tv->tv_micro >= 1000000) {
        tv->tv_secs++;
        tv->tv_micro -= 1000000;
    }
}

void SDL_SYS_DelayNS(Uint64 ns)
{
    struct timerequest req;
    struct MsgPort port;
    Uint64 us;

    if (ns == 0) {
        return;
    }

    us = (ns + 999ULL) / 1000ULL;
    if (us == 0) {
        us = 1;
    }

    MorphOS_InitQPort(&port);
    SDL_zerop(&req);

    req.tr_node.io_Message.mn_Node.ln_Type = NT_MESSAGE;
    req.tr_node.io_Message.mn_ReplyPort = &port;
    req.tr_node.io_Device = GlobalTimeReq.tr_node.io_Device;
    req.tr_node.io_Unit = GlobalTimeReq.tr_node.io_Unit;
    req.tr_node.io_Command = TR_ADDREQUEST;

    req.tr_time.tv_secs = (ULONG)(us / 1000000ULL);
    req.tr_time.tv_micro = (ULONG)(us % 1000000ULL);
    MorphOS_NormalizeTimeval(&req.tr_time);

    DoIO((struct IORequest *)&req);
}

#endif /* SDL_TIMER_MORPHOS */