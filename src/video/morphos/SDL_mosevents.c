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
#include "../../events/SDL_events_c.h"
#include "../../events/scancodes_morphos.h"

#include "SDL_misc.h"
#include "SDL_mosvideo.h"
#include "SDL_moswindow.h"
#include "SDL_mosopengl.h"
#include "SDL_mosmouse.h"
#include "SDL_moskeyboard.h"
#include "SDL_mosmodes.h" 
#include "SDL_moswindow.h" 
#include "SDL_mosevents.h"

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
MOS_DispatchMouseButtons(const struct IntuiMessage *m, SDL_WindowData *data)
{
    int state = (m->Code & IECODE_UP_PREFIX) ? false : true;
    int button = MOS_GetButton(m->Code & ~(IECODE_UP_PREFIX));

    if (button > 0) {
		if (!SDL_GetRelativeMouseMode()) {
			int x = m->IDCMPWindow->MouseX - m->IDCMPWindow->BorderLeft;
			int y = m->IDCMPWindow->MouseY - m->IDCMPWindow->BorderTop;
			SDL_SendMouseMotion(0, data->window, 0, 0, x, y);
		}

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
					SDL_SendKeyboardKey(0, SDL_GLOBAL_KEYBOARD_ID, 0, s, true);

					if (SDL_TextInputActive(data->window)) {
						char text[5] = {0};
						int length = MOS_TranslateUnicode(m, text);
						if (length > 0) {
							text[length] = '\0';
							SDL_SendKeyboardText(text);
						}
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
MOS_HandleActivation(SDL_VideoDevice *_this, SDL_WindowData *data, bool activated)
{
	if (data) {
		SDL_Window *sdlwin = data->window;
		if (sdlwin) {	
			D("Window %p activation %d relative=%d", sdlwin, activated, SDL_GetRelativeMouseMode());
			if (activated) {
				
				SDL_SendWindowEvent(sdlwin, SDL_EVENT_WINDOW_SHOWN, 0, 0);
				MOS_SyncKeyModifiers(_this);
				
				if (SDL_GetKeyboardFocus() != sdlwin)
					SDL_SetKeyboardFocus(sdlwin);
				
				//SDL_SetMouseFocus(sdlwin);
				
			} else {
				
				if (SDL_GetKeyboardFocus() == sdlwin) {
					if (!SDL_GetRelativeMouseMode()) {
						SDL_SetKeyboardFocus(NULL);
					}
					
				}
			}
		}
	}
}
static SDL_DisplayID 
MOS_GetDisplayForScreen(SDL_VideoDevice *_this, struct Screen *s)
{
    if (!_this || !s) return 0;

    for (int i = 0; i < _this->num_displays; i++) {
        SDL_VideoDisplay *d = _this->displays[i];
        if (!d) continue;
        SDL_DisplayData *dd = (SDL_DisplayData *) d->internal;
        if (dd && dd->screen == s) {
            return d->id;
        }
    }
    return 0;
}

void 
MOS_FocusAndWarpIfNeeded(SDL_VideoDevice *_this, SDL_WindowData *data)
{
	if (!data || !data->win || !data->window) return;
	if (!data->warp_pending || SDL_GetRelativeMouseMode()) return;

	struct Window *newwin = data ? data->win : NULL;

	if (newwin && newwin->WScreen) {
		ScreenToFront(newwin->WScreen);
		WindowToFront(newwin);
		ActivateWindow(newwin);

		const LONG cx = newwin->LeftEdge + newwin->BorderLeft +
			(newwin->Width - newwin->BorderLeft - newwin->BorderRight) / 2;
		const LONG cy = newwin->TopEdge + newwin->BorderTop +
			(newwin->Height - newwin->BorderTop - newwin->BorderBottom) / 2;

		MOS_WarpMouseInternal(newwin->WScreen, (float)cx, (float)cy);
	}

	data->warp_pending = false;
 
}

static void
MOS_ChangeWindow(SDL_VideoDevice *_this, const struct IntuiMessage *m, SDL_WindowData *data)
{
    if (!data || !data->win || !data->window) {
        return;
    }

    struct Window *syswin = data->win;
    SDL_Window *w = data->window;

    const int local_x = syswin->LeftEdge;
    const int local_y = syswin->TopEdge;

    SDL_DisplayID did = MOS_GetDisplayForScreen(_this, syswin->WScreen);
    if (!did) did = w->pending_displayID;
    if (!did) did = w->displayID;

    if (did && did != w->displayID) {
        w->displayID = did;
        if (w->pending_displayID == did) {
            w->pending_displayID = 0;
        }
        SDL_SendWindowEvent(w, SDL_EVENT_WINDOW_DISPLAY_CHANGED, (int)did, 0);
    }

    SDL_Rect b;
    SDL_zero(b);
    if (did) SDL_GetDisplayBounds(did, &b);

    const int global_x = b.x + local_x;
    const int global_y = b.y + local_y;
	
	int x, y;
    SDL_GlobalToRelativeForWindow(data->window, local_x, syswin->TopEdge, &x, &y);
    SDL_SendWindowEvent(w, SDL_EVENT_WINDOW_MOVED, global_x, global_y);

    int width  = syswin->Width  - syswin->BorderLeft - syswin->BorderRight;
    int height = syswin->Height - syswin->BorderTop  - syswin->BorderBottom;

    if (width != w->w || height != w->h) {
		if (data->__tglContext) {
			MOS_GL_ResizeContext(_this, w);
		}
        SDL_SendWindowEvent(w, SDL_EVENT_WINDOW_RESIZED, width, height);
    }

}

static void 
MOS_GadgetEvent(SDL_VideoDevice *_this, const struct IntuiMessage *m)
{
    SDL_WindowData *wd = (SDL_WindowData *)m->IDCMPWindow->UserData;
    if (!wd) return;

    switch (((struct Gadget *)m->IAddress)->GadgetID) {
        case ETI_Iconify:
            MOS_IconifyWindow(_this, true, wd->window);
            break;

			case ETI_Jump: {
				SDL_Window *w = wd->window;

				int num = 0;
				SDL_DisplayID *displays = SDL_GetDisplays(&num);
				if (displays && num > 0) {
					SDL_DisplayID cur = SDL_GetDisplayForWindow(w);
					if (!cur) cur = w->pending_displayID;

					int cur_index = -1;
					for (int i = 0; i < num; i++) {
						if (displays[i] == cur) { cur_index = i; break; }
					}

					const int next_index = (num > 1) ? ((cur_index >= 0) ? ((cur_index + 1) % num) : 0) : 0;
					const SDL_DisplayID next = displays[next_index];

					if (next && next != cur) {
						w->pending_displayID = next;

						const int pos = SDL_WINDOWPOS_CENTERED_DISPLAY(next);
						SDL_SetWindowPosition(w, pos, pos);
						SDL_SyncWindow(w);
						
					}
				}

				SDL_free(displays);
				break;
			}


    }
}

static void
MOS_AboutSDL(struct Window *window)
{
	struct EasyStruct es;
	es.es_StructSize   = sizeof(struct EasyStruct);
	es.es_Flags        = 0;
	es.es_Title        = (unsigned char *)"About SDL";
	es.es_TextFormat   = (unsigned char *)"SDL %ld.%ld.%ld -MorphOS-\nCompiled on " __AMIGADATE__ "\n\nPort by BeWorld\nwww.libsdl.org";
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

static void MOS_forcevsync(SDL_Window *window, bool val) 
{
	SDL_Renderer *renderer = SDL_GetRenderer(window);
	if (renderer) {
		if (!SDL_SetRenderVSync(renderer, val)) {
			SDL_Log("Couldn't enable vsync: %s", SDL_GetError());
		}
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
						MOS_forcevsync(data->window, 1);
						break;
					case MID_RVDISABLE:
						MOS_GlobalMenu(data->menu, 1, 4, 0, 0);
						MOS_GlobalMenu(data->menu, 1, 4, 1, 0);
						SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");
						MOS_setenv("SDL3_HINT_RENDER_VSYNC", "0", true);
						MOS_forcevsync(data->window, 0);					
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
    SDL_Window *win = data ? data->window : NULL;
	const bool minimized = (win && (win->flags & SDL_WINDOW_MINIMIZED));
	
	switch (m->Class) {
		case IDCMP_MENUPICK: 
			MOS_HandleMenu(_this, m);
			break;
			
		case IDCMP_REFRESHWINDOW:
			BeginRefresh(m->IDCMPWindow);
			EndRefresh(m->IDCMPWindow, TRUE);
			break;

		case IDCMP_CLOSEWINDOW:
			SDL_SendWindowEvent(data->window, SDL_EVENT_WINDOW_CLOSE_REQUESTED, 0, 0);
         break;

		case IDCMP_MOUSEMOVE:
			if (!minimized) MOS_MouseMove(_this, m, data);
			break;

		case IDCMP_MOUSEBUTTONS:
			if (!minimized) MOS_DispatchMouseButtons(m, data);
			break;

		case IDCMP_RAWKEY:
			if (!minimized) MOS_DispatchRawKey(m, data);
			break;

		case IDCMP_ACTIVEWINDOW:
			MOS_HandleActivation(_this, data, true);
			break;

		case IDCMP_INACTIVEWINDOW:
			MOS_HandleActivation(_this, data, false);
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

void 
MOS_HideApp(SDL_VideoDevice *_this)
{
	D("");
    SDL_VideoData *data = (SDL_VideoData *)_this->internal;
    if (data->in_hide_show || data->app_hidden) return;
    data->in_hide_show = true;

	MOS_ClearPointerIfApplied();
    MOS_CloseWindows(_this);
    MOS_CloseDisplay(_this, true);

    data->app_hidden = true;
    data->in_hide_show = false;
}

void 
MOS_ShowApp(SDL_VideoDevice *_this)
{
	D("");
    SDL_VideoData *data = (SDL_VideoData *)_this->internal;
    if (data->in_hide_show || !data->app_hidden) return;
    data->in_hide_show = true;

    if (!MOS_OpenDisplay(_this)) {
        data->in_hide_show = false;
        return;
    }
		
    MOS_OpenWindows(_this);

    data->app_hidden = false;
    data->in_hide_show = false;
	
	data->displays_dirty = true;
	MOS_RefreshDisplays(_this);
		
	if (__tglContext)  {
		SDL_SendWindowEvent(_this->current_glwin, SDL_EVENT_WINDOW_SHOWN, 0, 0);
		SDL_SendWindowEvent(_this->current_glwin, SDL_EVENT_WINDOW_EXPOSED, 0, 0);
		SDL_SendWindowEvent(_this->current_glwin, SDL_EVENT_WINDOW_RESIZED, _this->current_glwin->w, _this->current_glwin->h);
    }
}

static void
MOS_CheckScreenEvent(SDL_VideoDevice *_this)
{
	SDL_VideoData *data = (SDL_VideoData *) _this->internal;

	for (;;) {
		struct ScreenNotifyMessage *snm;

		while ((snm = (struct ScreenNotifyMessage *)GetMsg(&data->ScreenNotifyPort)) != NULL) {
			switch ((size_t)snm->snm_Value) {
				case FALSE:
					MOS_HideApp(_this);
					break;

				case TRUE:
					MOS_ShowApp(_this);
					break;
			}
			ReplyMsg((struct Message *)snm);
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
							D("SDL_SendDropfile : '%s'", filename);
							SDL_SendDropPosition(window, (float)msg->am_MouseX, (float)msg->am_MouseY);
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
				break;
		}
		ReplyMsg((struct Message *)msg);
	}
}

static struct {
    struct Window *win;
    LONG mx, my;
    bool inside;
    bool relative;
    SDL_Cursor *cursor;
    bool rmbtrap;
} g_ptrcache = { 0 };

void MOS_ClearPointerIfApplied(void)
{
    if (g_ptrcache.win) {
        g_ptrcache.win->Flags &= ~WFLG_RMBTRAP;
        ClearPointer(g_ptrcache.win);
        g_ptrcache.win = NULL;
    } else {
		g_ptrcache.win = NULL;
	}
}

void MOS_InvalidatePointerCacheForWindow(struct Window *win)
{
    if (g_ptrcache.win == win) {
        g_ptrcache.win = NULL;
        g_ptrcache.cursor = NULL;
        g_ptrcache.inside = false;
        g_ptrcache.relative = false;
        g_ptrcache.rmbtrap = false;
        g_ptrcache.mx = g_ptrcache.my = 0;
    }
}

void 
MOS_UpdatePointerIfNeeded(SDL_VideoData *vd, SDL_WindowData *wdata)
{
    if (!wdata || !wdata->win) {
        return;
    }

    struct Window *w = wdata->win;
    struct Screen *s = w->WScreen;
    if (!s) {
        return;
    }

    const LONG mx = s->MouseX;
    const LONG my = s->MouseY;

    const bool relative = SDL_GetRelativeMouseMode();
    SDL_Cursor *cursor = vd->CurrentPointer;

    bool inside = g_ptrcache.inside;

    const bool mouse_same =
        (g_ptrcache.win == w &&
         g_ptrcache.mx == mx &&
         g_ptrcache.my == my);

    if (!mouse_same || g_ptrcache.win != w) {
        const LONG ws  = w->LeftEdge + w->BorderLeft;
        const LONG wy  = w->TopEdge  + w->BorderTop;
        const LONG wx2 = w->LeftEdge + w->Width  - w->BorderRight;
        const LONG wy2 = w->TopEdge  + w->Height - w->BorderBottom;
        inside = (mx >= ws && my >= wy && mx <= wx2 && my <= wy2);
    }

    const bool want_rmbtrap = inside;

    if (g_ptrcache.win == w &&
        mouse_same &&
        g_ptrcache.inside == inside &&
        g_ptrcache.relative == relative &&
        g_ptrcache.cursor == cursor &&
        g_ptrcache.rmbtrap == want_rmbtrap) {
        return;
	}

    if (g_ptrcache.win != w || g_ptrcache.rmbtrap != want_rmbtrap) {
        if (want_rmbtrap) {
            w->Flags |= WFLG_RMBTRAP;
        } else {
            w->Flags &= ~WFLG_RMBTRAP;
        }
    }

    if (inside) {
        if (cursor) {
            SDL_CursorData *cd = (SDL_CursorData *)cursor->internal;
            if (cd) {
                if (cd->mouseptr) {
                    SetWindowPointer(w, WA_Pointer, (IPTR)(size_t)cd->mouseptr, TAG_DONE);
                } else {
                    const IPTR type = (IPTR)(size_t)(cd->type ? cd->type : (APTR)POINTERTYPE_NORMAL);
                    SetWindowPointer(w, WA_PointerType, type, TAG_DONE);
                }
            } else {
                SetWindowPointer(w, WA_PointerType, POINTERTYPE_NORMAL, TAG_DONE);
            }
        } else {
            SetWindowPointer(w, WA_PointerType, POINTERTYPE_INVISIBLE, TAG_DONE);
        }
    } else {
        if (relative) {
            SetWindowPointer(w, WA_PointerType, POINTERTYPE_INVISIBLE, TAG_DONE);
        } else {
            ClearPointer(w);
        }
    }

    g_ptrcache.win      = w;
    g_ptrcache.mx       = mx;
    g_ptrcache.my       = my;
    g_ptrcache.inside   = inside;
    g_ptrcache.relative = relative;
    g_ptrcache.cursor   = cursor;
    g_ptrcache.rmbtrap  = want_rmbtrap;
}

void MOS_PumpEvents(SDL_VideoDevice *_this)
{
    SDL_VideoData *data = (SDL_VideoData *)_this->internal;
    struct IntuiMessage *m;
    const ULONG mask = data->ScrNotifySig | data->BrokerSig | data->WBSig | data->WinSig | BREAKMASK;
    const ULONG pending = SetSignal(0, 0) & mask;

    if (data->app_hidden) {
        if ((pending & data->ScrNotifySig) && data->ScreenNotifyHandle) MOS_CheckScreenEvent(_this);
        if (pending & data->BrokerSig) MOS_CheckBrokerMsg(_this);
        if (pending & data->WBSig) MOS_CheckWBEvents(_this);

        if (data->break_armed) {
            const ULONG brk = pending & BREAKMASK;
            if (brk && !data->break_prev) {
                SetSignal(0, BREAKMASK);
                SDL_SendAppEvent(SDL_EVENT_QUIT);
            }
            data->break_prev = brk;
        }
        return;
    }

	if (pending & data->WinSig) {
        while ((m = (struct IntuiMessage *)GetMsg(&data->userPort))) {
            MOS_DispatchEvent(_this, m);
            ReplyMsg((struct Message *)m);
        }

		SDL_Window *focus = SDL_GetMouseFocus();
		if (focus) {
			SDL_WindowData *wdata = (SDL_WindowData *)focus->internal;
			MOS_UpdatePointerIfNeeded(data, wdata);
		} else {
			MOS_ClearPointerIfApplied();
		}
	}

    if ((pending & data->ScrNotifySig) && data->ScreenNotifyHandle) MOS_CheckScreenEvent(_this);
    if (pending & data->BrokerSig) MOS_CheckBrokerMsg(_this);
    if (pending & data->WBSig) MOS_CheckWBEvents(_this);

    if (data->break_armed) {
        const ULONG brk = pending & BREAKMASK;

        if (brk && !data->break_prev) {
            SetSignal(0, BREAKMASK);
            SDL_SendAppEvent(SDL_EVENT_QUIT);
        }
        data->break_prev = brk;
    }

}
