/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2025 Sam Lantinga <slouken@libsdl.org>

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

#include <exec/execbase.h>
#include <proto/commodities.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/icon.h>

static bool
MOS_VideoInit(SDL_VideoDevice *_this)
{
	D(kprintf("[%s]\n", __FUNCTION__));

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

	return true;
}

static void MOS_DeleteDevice(SDL_VideoDevice *_this)
{
	D(kprintf("[%s]\n", __FUNCTION__));
	SDL_VideoData *data = (SDL_VideoData *) _this->internal;

	if (data->inputReq) {
		CloseDevice((struct IORequest *)data->inputReq);
	}
	if (data->inputPort) {
		DeleteMsgPort(data->inputPort);
	}
	
	FreeSignal(data->ScreenNotifyPort.mp_SigBit);
	FreeSignal(data->BrokerPort.mp_SigBit);
	FreeSignal(data->appMsgPort.mp_SigBit);
	FreeSignal(data->userPort.mp_SigBit);

	if (data->BrokerRef)
		DeleteCxObjAll(data->BrokerRef);

	if (data->AppIcon)
		FreeDiskObject(data->AppIcon);

	SDL_free(data);
	SDL_free(_this);
}

bool MOS_SuspendScreenSaver(SDL_VideoDevice *_this)
{
	SDL_VideoData *data = (SDL_VideoData *) _this->internal;
	LONG suspend = _this->suspend_screensaver;
	
	D(kprintf("[%s] screen 0x%08lx, suspend %ld\n", __FUNCTION__, data->PublicScreen, suspend));

	if (suspend == 0 && data->ScreenSaverSuspendCount == 0)
		return true;

	data->ScreenSaverSuspendCount += suspend ? 1 : -1;

	if (data->PublicScreen)
		SetAttrs(data->PublicScreen, SA_StopBlanker, suspend, TAG_DONE);
	
	return true;
}

static CONST_STRPTR MOS_GetTaskName()
{
	struct Process *task = (struct Process *)FindTask(NULL);
	STRPTR name = "SDL";

	if (task->pr_Task.tc_Node.ln_Type == NT_PROCESS || task->pr_Task.tc_Node.ln_Type == NT_TASK) {
		if (task->pr_Task.tc_Node.ln_Type == NT_PROCESS && task->pr_CLI) {
			struct CommandLineInterface *cli = (struct CommandLineInterface *)BADDR(task->pr_CLI);

			if (cli->cli_Module && cli->cli_CommandName) {
				CONST_STRPTR src = (CONST_STRPTR)BADDR(cli->cli_CommandName);
				size_t len = *src + 1;

				if (len <= 1) {
					src = "SDL";
					len = sizeof("SDL");
				} else {
					if (src[1] == '"' && src[len] == '"')
						len -= 2;

					src++;
				}

				name = SDL_malloc(len);
				if (name)
					stccpy(name, src, len);
			}
		} else {
			size_t len = strlen(task->pr_Task.tc_Node.ln_Name) + sizeof("PROGDIR:");
			name = SDL_malloc(len);
			if (name) {
				strcpy(name, "PROGDIR:");
				strcpy(name+8, task->pr_Task.tc_Node.ln_Name);
			}
		}
	}

	D(kprintf("[%s] '%s'\n", __FUNCTION__, name));
	return name;
}

static void MOS_InitPort(struct MsgPort *port)
{
	port->mp_Node.ln_Name = "SDL";
	port->mp_Flags = PA_SIGNAL;
	port->mp_SigTask = SysBase->ThisTask;
	NEWLIST(&port->mp_MsgList);
	port->mp_SigBit = AllocSignal(-1);
}

static void MOS_InitBroker(SDL_VideoData *data)
{
	D(kprintf("[%s]\n", __FUNCTION__));
	
	STRPTR name = FilePart(data->FullAppName);
	data->AppBroker.nb_Version = NB_VERSION;
	data->AppBroker.nb_Name = name;
	data->AppBroker.nb_Title = name;
	data->AppBroker.nb_Descr = "SDL";
	data->AppBroker.nb_Unique = NBU_DUPLICATE;
	data->AppBroker.nb_Flags = COF_SHOW_HIDE;
	data->AppBroker.nb_Pri = 0;
	data->AppBroker.nb_Port = &data->BrokerPort;
	data->AppBroker.nb_ReservedChannel = 0;
	data->BrokerRef = CxBroker(&data->AppBroker, NULL);
	if (data->BrokerRef)
		ActivateCxObj(data->BrokerRef, 1);
}

static void MOS_VideoQuit(SDL_VideoDevice *_this)
{
	D(kprintf("[%s]\n", __FUNCTION__));

	MOS_CloseWindows(_this);
	MOS_CloseDisplay(_this);
	MOS_QuitKeyboard(_this);
	MOS_QuitMouse(_this);
	
}

static SDL_VideoDevice *MOS_CreateDevice()
{
	/* Initialize all variables that we clean on shutdown */
	SDL_VideoDevice *device = (SDL_VideoDevice *) SDL_calloc(1, sizeof(SDL_VideoDevice));
	D(kprintf("[%s]\n", __FUNCTION__));

	if (device) {
		SDL_VideoData *data = (struct SDL_VideoData *) SDL_calloc(1, sizeof(SDL_VideoData));

		device->internal = data;

		if (data) {
			
			 if (!(data->inputPort = CreateMsgPort())) {
				SDL_SetError("Couldn't allocate input port");
				return false;
			}
			
			if (!(data->inputReq = CreateIORequest(data->inputPort, sizeof(*data->inputReq)))) {
				SDL_SetError("Couldn't allocate input request");
				return false;
			}
			
			if (OpenDevice("input.device", 0, (struct IORequest *)data->inputReq, 0)) {
				SDL_SetError("Couldn't open input.device");
				return false;
			}

			MOS_InitPort(&data->ScreenNotifyPort);
			MOS_InitPort(&data->BrokerPort);
			MOS_InitPort(&data->appMsgPort);
			MOS_InitPort(&data->userPort);

			data->ScrNotifySig = 1 << data->ScreenNotifyPort.mp_SigBit;
			data->BrokerSig = 1 << data->BrokerPort.mp_SigBit;
			data->WBSig = 1 << data->appMsgPort.mp_SigBit;
			data->WinSig = 1 << data->userPort.mp_SigBit;

			NEWLIST(&data->windowlist);

			data->FullAppName = MOS_GetTaskName();
			data->AppIcon = GetDiskObject((STRPTR)data->FullAppName);

			if (data->AppIcon == NULL)
				data->AppIcon = GetDiskObject((STRPTR)"ENVARC:Sys/def_SDL");

			if (data->AppIcon) {
				data->AppIcon->do_CurrentX = NO_ICON_POSITION;
				data->AppIcon->do_CurrentY = NO_ICON_POSITION;
				data->AppIcon->do_Type = 0;
			}

			MOS_InitBroker(data);

			/* Set the function pointers */
			device->VideoInit = MOS_VideoInit;
			device->VideoQuit = MOS_VideoQuit;
			
			device->GetDisplayBounds = MOS_GetDisplayBounds;
			device->GetDisplayModes = MOS_GetDisplayModes;
			device->SetDisplayMode = MOS_SetDisplayMode;
			
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
			//device->SetWindowKeyboardGrab = MOS_SetWindowGrab;
			
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

			return device;
		}

		SDL_free(device);
	}

	SDL_OutOfMemory();
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
