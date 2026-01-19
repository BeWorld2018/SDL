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

#include "../SDL_sysvideo.h"

#include "SDL_mosframebuffer.h"
#include "SDL_moskeyboard.h"
#include "SDL_mosmodes.h"
#include "SDL_mosmouse.h"

#include "SDL_mosvideo.h"
#include "SDL_moswindow.h"
#include "SDL_mosmessagebox.h"
#include "SDL_mosopengl.h"
#include "SDL_mosevents.h"
#include "SDL_misc.h"

#include <exec/execbase.h>
#include <proto/commodities.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/icon.h>

static bool
MOS_VideoInit(SDL_VideoDevice *_this)
{
	D("");

	if (!MOS_InitModes(_this))
		return SDL_SetError("Failed to initialize modes");

	MOS_InitKeyboard(_this);
	MOS_InitMouse(_this);

	SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
	SDL_SetHint(SDL_HINT_GAMECONTROLLERCONFIG_FILE, "ENV:gamecontrollerdb.txt");
	SDL_SetHint(SDL_HINT_POLL_SENTINEL, "0");
	
	if (!SDL_GetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION)) {
		// to force software renderer use framebuffer and not opengl
        SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "0");
    }
	
	char *val = MOS_getenv("SDL3_THREAD_PRIORITY_POLICY");
	if (val && strlen(val)>0 && strcmp(val, "-1")==0) {
		SDL_SetCurrentThreadPriority(SDL_THREAD_PRIORITY_LOW);
	}
	val = MOS_getenv("SDL3_HINT_RENDER_DRIVER");
	if (val && strlen(val)>0) {	
		SDL_SetHint(SDL_HINT_RENDER_DRIVER, (strcmp(val, "opengl")==0 ? "opengl" : "software"));
	}
	val = MOS_getenv("SDL3_HINT_RENDER_VSYNC");
	if (val && strlen(val)>0) {
		SDL_SetHint(SDL_HINT_RENDER_VSYNC, (strcmp(val, "1")==0 ? "1" :"0"));
	}
	
	return true;
}

static void MOS_FreePortSignal(struct MsgPort *port)
{
    if (port && port->mp_SigBit != -1) {
        FreeSignal(port->mp_SigBit);
        port->mp_SigBit = -1;
    }
}

static void MOS_DeleteDevice(SDL_VideoDevice *_this)
{
    D("");
    if (!_this) return;

    SDL_VideoData *data = (SDL_VideoData *)_this->internal;
    if (data) {

        /* 1) Stop Commodities FIRST (it uses BrokerPort) */
        if (data->BrokerRef) {
            DeleteCxObjAll(data->BrokerRef);
            data->BrokerRef = NULL;
        }

        /* 2) input.device */
        if (data->inputReq) {
            CloseDevice((struct IORequest *)data->inputReq);
            DeleteIORequest((struct IORequest *)data->inputReq);
            data->inputReq = NULL;
        }
        if (data->inputPort) {
            DeleteMsgPort(data->inputPort);
            data->inputPort = NULL;
        }

        /* 3) Free signals only if allocated */
        MOS_FreePortSignal(&data->ScreenNotifyPort);
        MOS_FreePortSignal(&data->BrokerPort);
        MOS_FreePortSignal(&data->appMsgPort);
        MOS_FreePortSignal(&data->userPort);

        /* 4) Icon + name */
        if (data->AppIcon) {
            FreeDiskObject(data->AppIcon);
            data->AppIcon = NULL;
        }

        if (data->FullAppName) {
            SDL_free(data->FullAppName);
            data->FullAppName = NULL;
        }

        SDL_free(data);
        _this->internal = NULL;
    }

    SDL_free(_this);
}

bool MOS_SuspendScreenSaver(SDL_VideoDevice *_this)
{
	SDL_VideoData *data = (SDL_VideoData *) _this->internal;
	LONG suspend = _this->suspend_screensaver;
	
	D("Screen 0x%08lx, suspend %ld", data->PublicScreen, suspend);

	if (suspend == 0 && data->ScreenSaverSuspendCount == 0)
		return true;

	data->ScreenSaverSuspendCount += suspend ? 1 : -1;

	if (data->PublicScreen)
		SetAttrs(data->PublicScreen, SA_StopBlanker, suspend, TAG_DONE);
	
	return true;
}

static char *MOS_GetTaskName(void)
{
    struct Task *task = FindTask(NULL);
    if (!task) {
        return SDL_strdup("SDL3");
    }

    const char *src = (const char *)task->tc_Node.ln_Name;
    if (!src || !src[0]) {
        return SDL_strdup("SDL3");
    }

    return SDL_strdup(src);
}

static bool MOS_InitPort(struct MsgPort *port)
{
    port->mp_Node.ln_Name = (STRPTR)"SDL3";
    port->mp_Node.ln_Type = NT_MSGPORT;
    port->mp_Flags = PA_SIGNAL;
    port->mp_SigTask = FindTask(NULL);    /* or SysBase->ThisTask */
    NEWLIST(&port->mp_MsgList);

    const LONG bit = AllocSignal(-1);
    if (bit == -1) {
        port->mp_SigBit = -1;
        return false;
    }

    port->mp_SigBit = (BYTE)bit;
    return true;
}


static void MOS_InitBroker(SDL_VideoData *data)
{
    D("");

    STRPTR name = (STRPTR)"SDL3";
    if (data->FullAppName) {
        STRPTR fp = FilePart((STRPTR)data->FullAppName);
        if (fp && fp[0]) name = fp;
    }

    data->AppBroker.nb_Version = NB_VERSION;
    data->AppBroker.nb_Name = name;
    data->AppBroker.nb_Title = name;
    data->AppBroker.nb_Descr = (STRPTR)"SDL3";
    data->AppBroker.nb_Unique = NBU_DUPLICATE;
    data->AppBroker.nb_Flags = COF_SHOW_HIDE;
    data->AppBroker.nb_Pri = 0;
    data->AppBroker.nb_Port = &data->BrokerPort;
    data->AppBroker.nb_ReservedChannel = 0;

    data->BrokerRef = CxBroker(&data->AppBroker, NULL);
    if (data->BrokerRef) {
        ActivateCxObj(data->BrokerRef, 1);
    }
}

static void MOS_VideoQuit(SDL_VideoDevice *_this)
{
	D("");

	MOS_CloseWindows(_this);
	MOS_CloseDisplay(_this, false);
	MOS_QuitKeyboard(_this);
	MOS_QuitMouse(_this);
	
}

static SDL_VideoDevice *MOS_CreateDevice(void)
{
    D("");

    SDL_VideoDevice *device = (SDL_VideoDevice *)SDL_calloc(1, sizeof(*device));
    if (!device) {
        SDL_OutOfMemory();
        return NULL;
    }

    SDL_VideoData *data = (SDL_VideoData *)SDL_calloc(1, sizeof(*data));
    if (!data) {
        SDL_free(device);
        SDL_OutOfMemory();
        return NULL;
    }

    device->internal = data;

    /* calloc => 0, mais 0 est un sigbit valide : on force -1 */
    data->ScreenNotifyPort.mp_SigBit = -1;
    data->BrokerPort.mp_SigBit       = -1;
    data->appMsgPort.mp_SigBit       = -1;
    data->userPort.mp_SigBit         = -1;

    data->inputPort = NULL;
    data->inputReq  = NULL;
    data->BrokerRef = NULL;
    data->AppIcon   = NULL;
    data->FullAppName = NULL;

    /* input.device */
    data->inputPort = CreateMsgPort();
    if (!data->inputPort) {
        SDL_SetError("Couldn't allocate input port");
        goto fail;
    }

    data->inputReq = (struct IOStdReq *)CreateIORequest(data->inputPort, sizeof(*data->inputReq));
    if (!data->inputReq) {
        SDL_SetError("Couldn't allocate input request");
        goto fail;
    }

    if (OpenDevice("input.device", 0, (struct IORequest *)data->inputReq, 0) != 0) {
        SDL_SetError("Couldn't open input.device");

        /* IMPORTANT: make Delete safe without a flag */
        DeleteIORequest((struct IORequest *)data->inputReq);
        data->inputReq = NULL;

        goto fail;
    }

    /* Ports */
    if (!MOS_InitPort(&data->ScreenNotifyPort)) {
        SDL_SetError("Couldn't create ScreenNotifyPort");
        goto fail;
    }
    if (!MOS_InitPort(&data->BrokerPort)) {
        SDL_SetError("Couldn't create BrokerPort");
        goto fail;
    }
    if (!MOS_InitPort(&data->appMsgPort)) {
        SDL_SetError("Couldn't create appMsgPort");
        goto fail;
    }
    if (!MOS_InitPort(&data->userPort)) {
        SDL_SetError("Couldn't create userPort");
        goto fail;
    }

    data->ScrNotifySig = (ULONG)1u << (ULONG)data->ScreenNotifyPort.mp_SigBit;
    data->BrokerSig    = (ULONG)1u << (ULONG)data->BrokerPort.mp_SigBit;
    data->WBSig        = (ULONG)1u << (ULONG)data->appMsgPort.mp_SigBit;
    data->WinSig       = (ULONG)1u << (ULONG)data->userPort.mp_SigBit;

    NEWLIST(&data->windowlist);

    data->FullAppName = MOS_GetTaskName();

    data->AppIcon = GetDiskObject((STRPTR)data->FullAppName);
    if (!data->AppIcon) {
        data->AppIcon = GetDiskObject((STRPTR)"ENVARC:Sys/def_SDL");
    }
    if (data->AppIcon) {
        data->AppIcon->do_CurrentX = NO_ICON_POSITION;
        data->AppIcon->do_CurrentY = NO_ICON_POSITION;
        data->AppIcon->do_Type = 0;
    }

    /* Broker AFTER BrokerPort ok */
    MOS_InitBroker(data);

    /* Set function pointers */
    device->VideoInit = MOS_VideoInit;
    device->VideoQuit = MOS_VideoQuit;

    device->GetDisplayBounds = MOS_GetDisplayBounds;
    device->GetDisplayModes = MOS_GetDisplayModes;
    device->SetDisplayMode = MOS_SetDisplayMode;
    device->GetDisplayForWindow = MOS_GetDisplayForWindow;

    device->SuspendScreenSaver = MOS_SuspendScreenSaver;
    device->PumpEvents = MOS_PumpEvents;

    device->CreateSDLWindow = MOS_CreateWindow;
    device->SetWindowTitle = MOS_SetWindowTitle;
    device->SetWindowIcon = MOS_SetWindowIcon;
    device->SetWindowPosition = MOS_SetWindowPosition;
    device->SetWindowSize = MOS_SetWindowSize;
    device->SetWindowMinimumSize = MOS_SetWindowMinMaxSize;
    device->SetWindowMaximumSize = MOS_SetWindowMinMaxSize;
    device->ShowWindow = MOS_ShowWindow;
    device->HideWindow = MOS_HideWindow;
    device->RaiseWindow = MOS_RaiseWindow;
    device->MaximizeWindow = MOS_MaximizeWindow;
    device->MinimizeWindow = MOS_MinimizeWindow;
    device->RestoreWindow = MOS_RestoreWindow;
    device->SetWindowBordered = MOS_SetWindowBordered;
    device->SetWindowAlwaysOnTop = MOS_SetWindowAlwaysOnTop;
    device->SetWindowFullscreen = MOS_SetWindowFullscreen;
    device->SetWindowMouseGrab = MOS_SetWindowGrab;

    device->DestroyWindow = MOS_DestroyWindow;
    device->CreateWindowFramebuffer = MOS_CreateWindowFramebuffer;
    device->UpdateWindowFramebuffer = MOS_UpdateWindowFramebuffer;
    device->DestroyWindowFramebuffer = MOS_DestroyWindowFramebuffer;

    device->GL_LoadLibrary = MOS_GL_LoadLibrary;
    device->GL_GetProcAddress = MOS_GL_GetProcAddress;
    device->GL_UnloadLibrary = MOS_GL_UnloadLibrary;
    device->GL_CreateContext = MOS_GL_CreateContext;
    device->GL_MakeCurrent = MOS_GL_MakeCurrent;
    device->GL_SetSwapInterval = MOS_GL_SetSwapInterval;
    device->GL_GetSwapInterval = MOS_GL_GetSwapInterval;
    device->GL_SwapWindow = MOS_GL_SwapWindow;
    device->GL_DestroyContext = MOS_GL_DestroyContext;

    device->SetClipboardText = MOS_SetClipboardText;
    device->GetClipboardText = MOS_GetClipboardText;
    device->HasClipboardText = MOS_HasClipboardText;

    device->SetWindowResizable = MOS_SetWindowResizable;
    device->GetWindowBordersSize = MOS_GetWindowBordersSize;
    device->SetWindowOpacity = MOS_SetWindowOpacity;
    device->FlashWindow = MOS_FlashWindow;

    device->SetWindowHitTest = MOS_SetWindowHitTest;

    device->free = MOS_DeleteDevice;

    SetSignal(0, BREAKMASK);
    data->break_prev = 0;
    data->break_armed = true;
    data->displays_dirty = false;

    return device;

fail:
    MOS_DeleteDevice(device);
    return NULL;
}

VideoBootStrap MORPHOS_bootstrap = {
	"MorphOS", 
	"SDL MorphOS video driver",
	MOS_CreateDevice,
	MOS_ShowMessageBox
};


/* Native window apps may be interested in calling this */
struct MsgPort *MOS_GetSharedMessagePort()
{
    SDL_VideoDevice *video = SDL_GetVideoDevice();
	
    if (video) {
		SDL_VideoData *data = (SDL_VideoData *) video->internal;
		if (data)
			return &data->userPort;
    }

    return NULL;
}
