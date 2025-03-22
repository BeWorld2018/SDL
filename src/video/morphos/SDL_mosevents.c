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
#include "../../events/SDL_events_c.h"
#include "../../events/scancodes_morphos.h"

#include "SDL_misc.h"
#include "SDL_mosvideo.h"
#include "SDL_moswindow.h"
#include "SDL_mosopengl.h"
#include "SDL_mosmouse.h"
#include "SDL_moskeyboard.h"

#include <devices/rawkeycodes.h>
#include <intuition/extensions.h>
#include <intuition/intuimessageclass.h>
#include <libraries/screennotify.h>
#include <workbench/workbench.h>
#include <workbench/startup.h>

#include <proto/dos.h>
#include <proto/commodities.h>
#include <proto/exec.h>
#include <proto/locale.h>

void AHIAUD_Mute(ULONG mute);
extern MOS_GlobalMouseState globalMouseState;

static int
MOS_GetButton(int code)
{
    switch (code & ~IECODE_UP_PREFIX) {
        case IECODE_LBUTTON:
            return SDL_BUTTON_LEFT;
        case IECODE_RBUTTON:
            return SDL_BUTTON_RIGHT;
        case IECODE_MBUTTON:
            return SDL_BUTTON_MIDDLE;
        default:
            return 0;
    }
}

static void
MOS_DispatchMouseButtons(const struct IntuiMessage *m, const SDL_WindowData *data)
{
	SDL_WindowData *sdlwin = (SDL_WindowData *)m->IDCMPWindow->UserData;

    if (sdlwin) {
        int state = (m->Code & IECODE_UP_PREFIX) ? false : true;
        int button = MOS_GetButton(m->Code & ~(IECODE_UP_PREFIX));

        globalMouseState.buttonPressed[button] = state;
		SDL_SendMouseButton(0, data->window, 0, button, state);
	}
	
}

static int
MOS_TranslateUnicode(struct IntuiMessage *m, char *buffer)
{
	int length;
    WCHAR keycode = getv(m, IMSGA_UCS4);
	length = UTF8_Encode(keycode, buffer);

	return length;
}

static void
MOS_DispatchRawKey(struct IntuiMessage *m, const SDL_WindowData *data)
{
	SDL_Scancode s;
	UWORD code = m->Code;
	UWORD rawkey = m->Code & 0x7F;

	switch (code) {
		case RAWKEY_NM_WHEEL_UP:
			SDL_SendMouseWheel(0, data->window, 0, 0, 1, SDL_MOUSEWHEEL_NORMAL);
			break;

		case RAWKEY_NM_WHEEL_DOWN:
			SDL_SendMouseWheel(0, data->window, 0, 0, -1, SDL_MOUSEWHEEL_NORMAL);
			break;

		case RAWKEY_NM_WHEEL_LEFT:
			SDL_SendMouseWheel(0, data->window, 0, -1, 0, SDL_MOUSEWHEEL_NORMAL);
			break;

		case RAWKEY_NM_WHEEL_RIGHT:
			SDL_SendMouseWheel(0, data->window, 0, 1, 0,  SDL_MOUSEWHEEL_NORMAL);
			break;

		case RAWKEY_NM_BUTTON_FOURTH:
			SDL_SendMouseButton(0, data->window, 0, SDL_BUTTON_X1, 1);
			globalMouseState.buttonPressed[SDL_BUTTON_X1] = 1;
			break;

		case RAWKEY_NM_BUTTON_FOURTH | IECODE_UP_PREFIX:
			SDL_SendMouseButton(0, data->window, 0, SDL_BUTTON_X1, 0);
			globalMouseState.buttonPressed[SDL_BUTTON_X1] = 0;
			break;

		default:
			if (rawkey < sizeof(morphos_scancode_table) / sizeof(morphos_scancode_table[0])) {
				s = morphos_scancode_table[rawkey];
				if (m->Code < 128) {
					char text[5] = { 0 };
					int length = MOS_TranslateUnicode(m, text);
					SDL_SendKeyboardKey(0, SDL_GLOBAL_KEYBOARD_ID, 0, s, true);
					if (length > 0) {
						text[length] = '\0';
						SDL_SendKeyboardText(text);
					}
				} else {
					SDL_SendKeyboardKey(0, SDL_GLOBAL_KEYBOARD_ID, 0, s, false);
				}
			}
			break;
	}
}

static void
MOS_MouseMove(SDL_VideoDevice *_this, struct IntuiMessage *m, SDL_WindowData *data)
{
	if (data->win) {

		globalMouseState.x = m->IDCMPWindow->WScreen->MouseX;
		globalMouseState.y = m->IDCMPWindow->WScreen->MouseY;
		
		if (SDL_GetRelativeMouseMode()) {
			//D(kprintf("[%s] SDL_GetRelativeMouseMode first_deltamove=%d\n", __FUNCTION__, data->first_deltamove));
			if (data->first_deltamove) {
				data->first_deltamove = FALSE;
				return;
			}

			SDL_SendMouseMotion(0, data->window, 0, 1, m->MouseX, m->MouseY);
		} else {
			int x = m->IDCMPWindow->MouseX - m->IDCMPWindow->BorderLeft;
			int y = m->IDCMPWindow->MouseY - m->IDCMPWindow->BorderTop;
			SDL_SendMouseMotion(0, data->window, 0, 0, x, y);
		}
	}
}

static void
MOS_HandleActivation(SDL_VideoDevice *_this, struct IntuiMessage *m, bool activated)
{
	SDL_WindowData *data = (SDL_WindowData *)m->IDCMPWindow->UserData;
	
	if (data) {
		SDL_Window *sdlwin = data->window;
		if (sdlwin) {	
			D(kprintf("[%s] Window %p activation %d relative=%d \n", __FUNCTION__, sdlwin, activated, SDL_GetRelativeMouseMode()));
			if (activated) {
				
				SDL_SendWindowEvent(sdlwin, SDL_EVENT_WINDOW_SHOWN, 0, 0);
				MOS_SyncKeyModifiers(_this);
				
				if (SDL_GetKeyboardFocus() != sdlwin)
					SDL_SetKeyboardFocus(sdlwin);
				
			} else {
				
				if (SDL_GetKeyboardFocus() == sdlwin) {
					//D(kprintf("[%s][%d] - relative=%d \n", __FUNCTION__, __LINE__, SDL_GetRelativeMouseMode()));
					if (!SDL_GetRelativeMouseMode())
						SDL_SetKeyboardFocus(NULL);
					
				}

			}
		}
	}
}

static void
MOS_ChangeWindow(SDL_VideoDevice *_this, const struct IntuiMessage *m, SDL_WindowData *data)
{
	if (data->win) {
		struct Window *syswin = data->win;
		int x = 0, y = 0;
		SDL_GetWindowPosition(data->window, &x, &y);

		if (syswin->LeftEdge != x || syswin->TopEdge != y) {
			//D(kprintf("[%s] Move\n", __FUNCTION__));
			SDL_SendWindowEvent(data->window, SDL_EVENT_WINDOW_MOVED, syswin->LeftEdge, syswin->TopEdge);
		}
		int width = syswin->Width - syswin->BorderLeft - syswin->BorderRight;
		int height = syswin->Height - syswin->BorderTop - syswin->BorderBottom;
		if (width != data->window->w || height != data->window->h) {
			//D(kprintf("[%s] Resize\n", __FUNCTION__));
			SDL_SendWindowEvent(data->window, SDL_EVENT_WINDOW_RESIZED, width, height);
			if (__tglContext) MOS_GL_ResizeContext(_this, data->window);
		}
	}
}

static void MOS_GadgetEvent(SDL_VideoDevice *_this, const struct IntuiMessage *m)
{
	D(kprintf("[%s]\n", __FUNCTION__));
	SDL_WindowData *data = (SDL_WindowData *)m->IDCMPWindow->UserData;
	
	switch (((struct Gadget *)m->IAddress)->GadgetID) {
		case ETI_Iconify:
			MOS_IconifyWindow(_this, true, data->window);
			break;
		case ETI_Jump:
			// D(kprintf("[%s] ETI_JUMP\n", __FUNCTION__)/;
			break;
	}
}

static void
MOS_AboutSDL(struct Window *window)
{
	struct EasyStruct es;
	es.es_StructSize   = sizeof(struct EasyStruct);
	es.es_Flags        = 0;
	es.es_Title        = (unsigned char *)"About SDL";
	es.es_TextFormat   = (unsigned char *)"SDL %ld.%ld.%ld -MorphOS-\nCompiled on " __AMIGADATE__ "\n\n ** BETA VERSION **\n\nPort by BeWorld\nwww.libsdl.org";
	es.es_GadgetFormat = (unsigned char *)"Ok";

	EasyRequest(window, &es, NULL, SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
}

static void
MOS_Priority(ULONG prio)
{
	SDL_ThreadPriority Priority = prio ? SDL_THREAD_PRIORITY_LOW : SDL_THREAD_PRIORITY_NORMAL;
	SDL_SetCurrentThreadPriority(Priority);
	MOS_setenv("SDL3_THREAD_PRIORITY_POLICY", (prio ? "-1" : "0"), true);
}

void 
MOS_GlobalMenu(struct Menu * mymenu, UWORD menu, UWORD item, UWORD sub, UWORD check)
{
	struct MenuItem *subitem;
	subitem = ItemAddress(mymenu, FULLMENUNUM(menu, item, sub));
	if (subitem) {
		if (check)
			subitem->Flags |= CHECKED;
		else
			subitem->Flags &= ~CHECKED;
	}
}

static void
MOS_HandleMenu(SDL_VideoDevice *_this, struct IntuiMessage *m)
{
	SDL_WindowData *data = (SDL_WindowData *)m->IDCMPWindow->UserData;
	if (data->win) {
		if (data->menu) {
			struct MenuItem *item = ItemAddress(data->menu, m->Code);
			if (item) {
				switch ((ULONG)GTMENUITEM_USERDATA(item)) {
					case MID_ABOUT:
						MOS_AboutSDL(data->win);
						break;
					case MID_QUIT:
						SDL_SendWindowEvent(data->window, SDL_EVENT_WINDOW_CLOSE_REQUESTED, 0, 0);
						break;
					case MID_HIDE:
						MOS_IconifyWindow(_this, true, data->window);
						break;
					case MID_MUTE:
						AHIAUD_Mute(item->Flags & CHECKED);
						break;
					case MID_PRIORITY:
						MOS_Priority(item->Flags & CHECKED);
						break;
					case MID_RRAUTO:
						MOS_GlobalMenu(data->menu, 1, 3, 1, 0);
						MOS_GlobalMenu(data->menu, 1, 3, 2, 0);
						SDL_SetHint(SDL_HINT_RENDER_DRIVER, "");
						MOS_setenv("SDL3_HINT_RENDER_DRIVER", "", true);
						break;
					case MID_RRGL:
						MOS_GlobalMenu(data->menu, 1, 3, 0, 0);
						MOS_GlobalMenu(data->menu, 1, 3, 2, 0);
						SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
						MOS_setenv("SDL3_HINT_RENDER_DRIVER", "opengl", true);
						break;
					case MID_RRSOFT:
						MOS_GlobalMenu(data->menu, 1, 3, 0, 0);
						MOS_GlobalMenu(data->menu, 1, 3, 1, 0);
						SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
						MOS_setenv("SDL3_HINT_RENDER_DRIVER", "software", true);
						break;
					case MID_RVAUTO:
						MOS_GlobalMenu(data->menu, 1, 4, 1, 0);
						MOS_GlobalMenu(data->menu, 1, 4, 2, 0);
						SDL_SetHint(SDL_HINT_RENDER_VSYNC, "");
						MOS_setenv("SDL3_HINT_RENDER_VSYNC", "", true);
						break;
					case MID_RVENABLE:
						MOS_GlobalMenu(data->menu, 1, 4, 0, 0);
						MOS_GlobalMenu(data->menu, 1, 4, 2, 0);
						SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
						MOS_setenv("SDL3_HINT_RENDER_VSYNC", "1", true);
						break;
					case MID_RVDISABLE:
						MOS_GlobalMenu(data->menu, 1, 4, 0, 0);
						MOS_GlobalMenu(data->menu, 1, 4, 1, 0);
						SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");
						MOS_setenv("SDL3_HINT_RENDER_VSYNC", "0", true);
						break;
					default:
						break;
				}
			}
		}
	}
}

static void
MOS_DispatchEvent(SDL_VideoDevice *_this, struct IntuiMessage *m)
{
	SDL_WindowData *data = (SDL_WindowData *)m->IDCMPWindow->UserData;
	switch (m->Class) {
		case IDCMP_MENUPICK: 
			MOS_HandleMenu(_this, m);
			break;
			
		case IDCMP_REFRESHWINDOW:
			BeginRefresh(m->IDCMPWindow);
			//SDL_SendWindowEvent(data->window, SDL_EVENT_WINDOW_EXPOSED, 0, 0);
			EndRefresh(m->IDCMPWindow, TRUE);
			break;

		case IDCMP_CLOSEWINDOW:
			SDL_SendWindowEvent(data->window, SDL_EVENT_WINDOW_CLOSE_REQUESTED, 0, 0);
         break;

		case IDCMP_MOUSEMOVE:
			MOS_MouseMove(_this, m, data);
			break;

		case IDCMP_MOUSEBUTTONS:
			MOS_DispatchMouseButtons(m, data);
			break;

		case IDCMP_RAWKEY:
			MOS_DispatchRawKey(m, data);
			break;

		case IDCMP_ACTIVEWINDOW:
			MOS_HandleActivation(_this, m, true);
			break;

		case IDCMP_INACTIVEWINDOW:
			MOS_HandleActivation(_this, m, false);
			break;

		case IDCMP_CHANGEWINDOW:
			MOS_ChangeWindow(_this, m, data);
			break;

		case IDCMP_GADGETUP:
			MOS_GadgetEvent(_this, m);
			break;
			
		default:
        	break;
	}
}

static void
MOS_CheckBrokerMsg(SDL_VideoDevice *_this)
{
	SDL_VideoData *data = (SDL_VideoData *) _this->internal;
	CxMsg *msg;

	while ((msg = (CxMsg *)GetMsg(&data->BrokerPort))) {
		size_t id = CxMsgID(msg);
		size_t tp = CxMsgType(msg);

		//D(kprintf("[%s] check CxMsg\n", __FUNCTION__));
		ReplyMsg((APTR)msg);

		if (tp == CXM_COMMAND) {
			switch (id) {
				case CXCMD_KILL:
					SDL_SendAppEvent(SDL_EVENT_QUIT);
					break;

				case CXCMD_APPEAR:
					MOS_UniconifyWindow(_this, NULL);
					break;

				case CXCMD_DISAPPEAR:
					MOS_IconifyWindow(_this, true, NULL);
					break;
			}
		}
	}
}

static void
MOS_CheckScreenEvent(SDL_VideoDevice *_this)
{
	SDL_VideoData *data = (SDL_VideoData *) _this->internal;

	for (;;) {
		struct ScreenNotifyMessage *snm;

		while ((snm = (struct ScreenNotifyMessage *)GetMsg(&data->ScreenNotifyPort)) != NULL) {
			//D(kprintf("[%s] check ScreenNotifyMessage\n", __FUNCTION__));

			switch ((size_t)snm->snm_Value) {
				case FALSE:
					MOS_IconifyWindow(_this, false, NULL);
					break;

				case TRUE:
					MOS_UniconifyWindow(_this, NULL);
					break;
			}
		}

		if (data->PublicScreen)
			break;

		WaitPort(&data->ScreenNotifyPort);
	}
}

static void
MOS_CheckWBEvents(SDL_VideoDevice *_this)
{
	SDL_VideoData *data = (SDL_VideoData *) _this->internal;
	struct AppMessage *msg;
	
	while ((msg = (struct AppMessage *)GetMsg(&data->appMsgPort)) != NULL) {
		//D(kprintf("[%s] check AppMessage\n", __FUNCTION__));
		SDL_Window *window = (SDL_Window *)msg->am_UserData;
		switch (msg->am_Type) {
			case AMTYPE_APPWINDOW: 
				{
					char filename[1024];
					struct WBArg *argptr = msg->am_ArgList;
				    for (int i = 0; i < msg->am_NumArgs; i++) {
						if (argptr->wa_Lock) {
							NameFromLock(argptr->wa_Lock, filename, 1024);
							AddPart((STRPTR)filename, (STRPTR)argptr->wa_Name, 1024);
							D(kprintf("[%s] SDL_SendDropfile : '%s'\n", __FUNCTION__, filename));
							SDL_SendDropFile(window, NULL, filename);
							argptr++;
						}
					}
					SDL_SendDropComplete(window);
					SDL_RaiseWindow(window);
				}
				break;
			case AMTYPE_APPICON:
				MOS_UniconifyWindow(_this, window);
				break;
			default:
				//D(kprintf("[%s] Unknown AppMsg %d %p\n", __FUNCTION__, msg->am_Type, (APTR)msg->am_UserData));
				break;
		}

	}
}

void
MOS_PumpEvents(SDL_VideoDevice *_this)
{
	SDL_VideoData *data = (SDL_VideoData *) _this->internal;
	struct IntuiMessage *m;

	size_t sigs = SetSignal(0, data->ScrNotifySig | data->BrokerSig | data->WBSig | data->WinSig /*| SIGBREAKF_CTRL_C*/);

	if (sigs & data->WinSig) {
		SDL_WindowData *wdata = NULL;
		while ((m = (struct IntuiMessage *)GetMsg(&data->userPort))) {
			wdata = (SDL_WindowData *)m->IDCMPWindow->UserData;
			MOS_DispatchEvent(_this, m);
			ReplyMsg((struct Message *)m);
		}

		if (wdata && wdata->win) {
			struct Window *w = wdata->win;
            struct Screen *s = w->WScreen;
			if (s) {
				LONG mx = s->MouseX;
                LONG my = s->MouseY;
                LONG ws = w->LeftEdge + w->BorderLeft;
                LONG wy = w->TopEdge + w->BorderTop;
                LONG wx2 = w->LeftEdge + w->Width - w->BorderRight;
                LONG wy2 = w->TopEdge + w->Height - w->BorderBottom;
				if (mx >= ws && my >= wy && mx <= wx2 && my <= wy2) {
					w->Flags |= WFLG_RMBTRAP;
					if (data->CurrentPointer) {
						SDL_Cursor *cursor = data->CurrentPointer;
						SDL_CursorData *cursordata = cursor->internal;
						if (cursordata->mouseptr) {
							SetWindowPointer(w, WA_Pointer, (size_t)cursordata->mouseptr, TAG_DONE);
						} else {
							SetWindowPointer(w, WA_PointerType, (size_t)cursordata->type, TAG_DONE);
						}
					} else {
						size_t pointertags[] = { WA_PointerType, POINTERTYPE_INVISIBLE, TAG_DONE };
						SetAttrsA(w, (struct TagItem *)&pointertags);
					}
					
				} else {
					if (SDL_GetRelativeMouseMode()) {
						size_t pointertags[] = { WA_PointerType, POINTERTYPE_INVISIBLE, TAG_DONE };
						SetAttrsA(w, (struct TagItem *)&pointertags);
					}else{
						w->Flags &= ~WFLG_RMBTRAP;
						ClearPointer(w);
					}
				}
			}
		}
	}

	if (sigs & data->ScrNotifySig && data->ScreenNotifyHandle)
		MOS_CheckScreenEvent(_this);

	if (sigs & data->BrokerSig)
		MOS_CheckBrokerMsg(_this);

	if (sigs & data->WBSig)
		MOS_CheckWBEvents(_this);

	//if (sigs & SIGBREAKF_CTRL_C)
	//	SDL_SendAppEvent(SDL_EVENT_QUIT);
}
