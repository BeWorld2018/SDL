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

#include "SDL_misc.h"
#include "../../events/SDL_keyboard_c.h"
#include "../../events/SDL_events_c.h"

#include "SDL_mosvideo.h"
#include "SDL_mosmodes.h"
#include "SDL_moswindow.h"
#include "SDL_mosopengl.h"
#include "SDL_mosevents.h"
#include "SDL_mosmouse.h"

#include <intuition/extensions.h>
#include <libraries/charsets.h>
#include <proto/alib.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/wb.h>
#include <proto/dos.h>

#define MIN_WINDOW_SIZE 32
#define SDL_PROP_WINDOW_CREATE_MORPHOS_MENU_BOOLEAN "SDL.window.create.morphos.menu"
#define SDL_PROP_WINDOW_MORPHOS_MENU_BOOLEAN        "SDL.window.morphos.menu"

struct NewMenu SDL_NewMenu[] =
{
	{ NM_TITLE, (char *)"Project", 0, 0, 0, (APTR)MID_PROJECT },
	{ NM_ITEM , (char *)"About SDL3", (const STRPTR)"A", 0, 0, (APTR)MID_ABOUT },
	{ NM_ITEM, NM_BARLABEL, NULL, 0, 0, NULL },
	{ NM_ITEM , (char *)"Iconify", (const STRPTR)"I", 0, 0, (APTR)MID_HIDE },
	{ NM_ITEM, NM_BARLABEL, NULL, 0, 0, NULL },
	{ NM_ITEM , (char *)"Quit", (const STRPTR)"Q", 0, 0, (APTR)MID_QUIT},
	{ NM_TITLE, (char *)"Options", 0, 0, 0, (APTR)MID_OPTIONS },
	{ NM_ITEM , (char *)"Mute Sound", (const STRPTR)"M", (CHECKIT | MENUTOGGLE), 0, (APTR)MID_MUTE},
	{ NM_ITEM , (char *)"Low CPU Priority", (const STRPTR)"P", (CHECKIT | MENUTOGGLE), 0, (APTR)MID_PRIORITY},
	{ NM_ITEM, NM_BARLABEL, NULL, 0, 0, NULL },
	{ NM_ITEM, (char *)"HINT RENDER DRIVER", 0, 0, 0, (APTR)MID_RRENDER },
	{ NM_SUB, (char *)"Default", 0, (CHECKED | CHECKIT | MENUTOGGLE), 0, (APTR)MID_RRAUTO },
	{ NM_SUB, (char *)"OpenGL", 0, (CHECKIT | MENUTOGGLE), 0, (APTR)MID_RRGL },
	{ NM_SUB, (char *)"Software", 0, (CHECKIT | MENUTOGGLE), 0, (APTR)MID_RRSOFT },
	{ NM_ITEM, (char *)"HINT RENDER VSYNC", 0, 0, 0, (APTR)MID_RVSYNC },
	{ NM_SUB, (char *)"Default", 0, (CHECKED | CHECKIT | MENUTOGGLE), 0, (APTR)MID_RVAUTO },
	{ NM_SUB, (char *)"Enabled", 0, (CHECKIT | MENUTOGGLE), 0, (APTR)MID_RVENABLE },
	{ NM_SUB, (char *)"Disabled", 0, (CHECKIT | MENUTOGGLE), 0, (APTR)MID_RVDISABLE },
	{ NM_END , NULL, NULL, 0, 0, NULL }
};

void
MOS_GetWindowSize(struct Window *w, int *width, int *height)
{
	*width = getv(w, WA_InnerWidth);
	*height = getv(w, WA_InnerHeight);
}

static void
MOS_RemoveAppWindow(SDL_WindowData *data)
{
    if (data->appWin) {
		D("Removing AppWindow");
			
		if (RemoveAppWindow(data->appWin)== FALSE) {
			D("Failed to remove AppWindow");
        }
        data->appWin = NULL;
    }
}

static void 
MOS_RemoveAppIcon(SDL_WindowData *data)
{
	if (data->appIcon) {
        D("Removing AppIcon");
		RemoveAppIcon(data->appIcon);
		data->appIcon = NULL;
    }	
}

void MOS_RemoveMenuObject(SDL_WindowData *data) 
{

	if (data->menu) {
		D("ClearMenuStrip and FreeMenus");
		ClearMenuStrip(data->win);
		FreeMenus(data->menu);
		data->menu = NULL;
	}
	if (data->menuvisualinfo) {
		D("FreeVisualInfo");
		FreeVisualInfo(data->menuvisualinfo);
		data->menuvisualinfo = NULL;
	}
}


static void 
MOS_CreateAppWindow(SDL_VideoDevice *_this, SDL_Window * window)
{
    SDL_VideoData *videodata = (SDL_VideoData *) _this->internal;
    SDL_WindowData *data = window->internal;

    if (data->appWin) {
        D("AppWindow already exists for window '%s'", window->title);
        return;
    }

    data->appWin = AddAppWindow(0, (ULONG)window, data->win, &videodata->appMsgPort, TAG_DONE);

    if (!data->appWin) {
        D("Couldn't create AppWindow");
    } else {
		D("AddAppWindow OK");
	}
}

static void 
MOS_CreateMenu(SDL_VideoDevice *_this, SDL_Window * window)
{
	SDL_WindowData *data = window->internal;
	SDL_VideoData *videodata = (SDL_VideoData *) _this->internal;

    MOS_RemoveMenuObject(data);
	
	if (!(window->flags & SDL_WINDOW_FULLSCREEN)) {
		data->menuvisualinfo = GetVisualInfoA(videodata->PublicScreen, NULL);
		if (data->menuvisualinfo) {
			data->menu = CreateMenusA(SDL_NewMenu, NULL);
			if (data->menu) {
				if (!LayoutMenusA(data->menu, data->menuvisualinfo, NULL)) {
					D("Menu ERROR");
					MOS_RemoveMenuObject(data);
				} else {
					D("Menu OK");
					
					if (SetMenuStrip(data->win, data->menu)) {

						char *val = MOS_getenv("SDL3_THREAD_PRIORITY_POLICY");
						if (val && strlen(val)>0 && strcmp(val, "-1")==0) {
							MOS_GlobalMenu(data->menu, 1, 1, 0, 1);
						}
						val = MOS_getenv("SDL3_HINT_RENDER_DRIVER");
						if (val && strlen(val)>0) {					
							MOS_GlobalMenu(data->menu, 1, 3, 0, 0);
							MOS_GlobalMenu(data->menu, 1, 3, 1, (strcmp(val, "opengl")==0 ? 1 : 0));
							MOS_GlobalMenu(data->menu, 1, 3, 2, (strcmp(val, "opengl")==0 ? 0 : 1));
						}
						val = MOS_getenv("SDL3_HINT_RENDER_VSYNC");
						if (val && strlen(val)>0) {
							MOS_GlobalMenu(data->menu, 1, 4, 0, 0);
							MOS_GlobalMenu(data->menu, 1, 4, 1, (strcmp(val, "1")==0 ? 1 : 0));
							MOS_GlobalMenu(data->menu, 1, 4, 2, (strcmp(val, "1")==0 ? 0 : 1));
						}
					} else {
						D("Failed SetMenuStrip");
					}	
				}
			} else {
				MOS_RemoveMenuObject(data);
			}
		} else {
			D("Failed GetVisualInfoA");
		}
	}
}

static void 
MOS_CloseWindowSafely(SDL_VideoDevice *_this, SDL_Window *window, struct Window *win)
{
	D("");
	
	if (SDL_GetKeyboardFocus() == window)
		SDL_SetKeyboardFocus(NULL);

	if (SDL_GetMouseFocus() == window)
		SDL_SetMouseFocus(NULL);
		
	if (window) {
		
		if ((window->flags & SDL_WINDOW_EXTERNAL) == 0) {
			SDL_WindowData *data = (SDL_WindowData *) window->internal;
			
			struct IntuiMessage *msg, *tmp;
			Forbid();

			if (data->grabbed) {
				data->grabbed = TRUE;
				DoMethod((Object *)data->win, WM_ReleaseEvents);
			}
		
			ForeachNodeSafe(&win->UserPort->mp_MsgList, msg, tmp) {
				if (msg->IDCMPWindow == win) {
					REMOVE(&msg->ExecMessage.mn_Node);
					ReplyMsg(&msg->ExecMessage);
				}
			}
			MOS_RemoveAppWindow(data);
			MOS_RemoveAppIcon(data);
			MOS_RemoveMenuObject(data);
			
			CloseWindow(win);
			
			Permit();
		}
	}
}

void
MOS_CloseWindows(SDL_VideoDevice *_this)
{
	D("");
	SDL_VideoData *data = (SDL_VideoData *) _this->internal;
	SDL_WindowData *wd = NULL;

	ForeachNode(&data->windowlist, wd) {
		struct Window *win = wd->win;

		if (win) {
			wd->win = NULL;
			MOS_CloseWindowSafely(_this, wd->window, win);
		}
	}

}

void 
MOS_OpenWindows(SDL_VideoDevice *_this)
{
	SDL_VideoData *data = (SDL_VideoData *) _this->internal;
	SDL_WindowData *wd;
	D("");

	ForeachNode(&data->windowlist, wd) {
		if ((wd->window->flags & SDL_WINDOW_EXTERNAL) == 0) {
			MOS_RecreateWindow(_this, wd->window);
		}
	}
}

static void 
MOS_CloseWindow(SDL_VideoDevice *_this, SDL_Window *window) 
{
    SDL_WindowData *data = (SDL_WindowData *)window->internal;
    struct Window *win = data ? data->win : NULL;

    D("data->win=0x%08lx", win);
    if (!win) {
        return;
    }

    MOS_CloseWindowSafely(_this, window, win);
    data->win = NULL;
}

void
MOS_DestroyWindow(SDL_VideoDevice *_this, SDL_Window * window)
{
	D("");
	SDL_WindowData *data = (SDL_WindowData *) window->internal;

	if (data) {
	
		if (data->__tglContext)
			MOS_GL_DestroyContext(_this, (SDL_GLContext)data->__tglContext);
		
		REMOVE(&data->node);

		if (data->win) {
			if (!(window->flags & SDL_WINDOW_EXTERNAL)) {
				MOS_CloseWindowSafely(_this, window, data->win);
				data->win = NULL;
			} else {
				D("Ignored for native window");
			}
		}
		
		SDL_free(data->window_title);
		SDL_free(data);
		window->internal = NULL;
	}
}

void 
MOS_RecreateWindow(SDL_VideoDevice *_this, SDL_Window *window)
{
    if (window->flags & SDL_WINDOW_EXTERNAL) {
        D("Cannot modify native window '%s'", window->title);
        return;
    }

    SDL_WindowData *data = (SDL_WindowData *)window->internal;
    if (data && data->win) {
        D("Closing system window '%s' before re-creation", window->title);
        MOS_CloseWindow(_this, window);
    }

    MOS_CreateSystemWindow(_this, window);

    data = (SDL_WindowData *)window->internal;

    if (data && data->win) {
        D("window '%s' OK", window->title);
        MOS_ShowWindow(_this, window);
		MOS_WindowToFront(data->win);
    } else {
        D("Failed to re-create window '%s'", window->title);
    }
}

static bool
MOS_SetupWindowData(SDL_VideoDevice *_this, SDL_Window *window, struct Window *syswin)
{
	D("");
	SDL_WindowData *data = (SDL_WindowData *) SDL_calloc(1, sizeof(*data));
	SDL_VideoData *ddata = (SDL_VideoData *) _this->internal;
	
	if (data) {
		window->internal = data;
		ADDHEAD(&ddata->windowlist, data);
		data->fb = NULL;
		data->window = window;
		data->win = syswin;
		data->__tglContext = NULL;
		data->grabbed = FALSE;
		data->window_title = NULL;
		data->videodata = ddata;
		data->first_deltamove = FALSE;
		data->appWin = NULL;
		data->menu = NULL;
		data->menuvisualinfo = NULL;
		if (syswin)
			syswin->UserData = (APTR)data;
		else {
			data->old_w = 0;
			data->old_h = 0;
			data->old_x = 0;
			data->old_y = 0;
		}
		
	} else {
		
		return SDL_OutOfMemory();
	}

	return true;
}

bool
MOS_CreateWindow(SDL_VideoDevice *_this, SDL_Window * window, SDL_PropertiesID create_props)
{
	D("");
	struct Window *win = (struct Window *)SDL_GetPointerProperty(create_props,"morphos.window",SDL_GetPointerProperty(create_props, "sdl2-compat.external_window", NULL));
	bool ok;
	bool withmenu = SDL_GetBooleanProperty(
		create_props,
		SDL_PROP_WINDOW_CREATE_MORPHOS_MENU_BOOLEAN,
		true
	);

	if (win) {
		
		size_t flags;

		if (win->Title && win->Title != (APTR)-1)
			window->title = MOS_ConvertText((const char *)win->Title, MIBENUM_SYSTEM, MIBENUM_UTF_8);

		flags = (window->flags | SDL_WINDOW_EXTERNAL) & ~SDL_WINDOW_MINIMIZED;

		if (win->Flags & WFLG_SIZEGADGET)
			flags |= SDL_WINDOW_RESIZABLE;
		else
			flags &= ~SDL_WINDOW_RESIZABLE;

		if (win->Flags & WFLG_BORDERLESS)
			flags |= SDL_WINDOW_BORDERLESS;
		else
			flags &= ~SDL_WINDOW_BORDERLESS;

		if (win->Flags & WFLG_WINDOWACTIVE) {
			flags |= SDL_WINDOW_INPUT_FOCUS;
		}

		window->flags = flags;
		
		ok = MOS_SetupWindowData(_this, window, win);
	
	} else {
	
		ok = MOS_SetupWindowData(_this, window, NULL);
		
	}
	if (!ok) {
		return false;
	}
	
	SDL_PropertiesID props = SDL_GetWindowProperties(window);
	SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_MORPHOS_MENU_BOOLEAN, withmenu);

	return MOS_CreateSystemWindow(_this, window);
}

void
MOS_SetWindowTitle(SDL_VideoDevice *_this, SDL_Window * window)
{
	SDL_WindowData *data = (SDL_WindowData *) window->internal;

	if (data->win) {
		APTR old = data->window_title;
		APTR title = MOS_ConvertText(window->title, MIBENUM_UTF_8, MIBENUM_SYSTEM);
		D("%s to %s (0x%08lx)", window->title, title, data->win);
		SetWindowTitles(data->win, title, title);
		data->window_title = title;
		SDL_free(old);
	}
}

bool
MOS_SetWindowIcon(SDL_VideoDevice *_this, SDL_Window * window, SDL_Surface * icon)
{
	return SDL_Unsupported();
}

bool
MOS_SetWindowPosition(SDL_VideoDevice *_this, SDL_Window * window)
{
	SDL_WindowData *data = (SDL_WindowData *) window->internal;
	//D("0x%08lx position %d, %d (pending_displayID=%d)", data->win, window->pending.x, window->pending.y, (int)window->pending_displayID);

	if (data->win) {
		SDL_DisplayID target_did = window->pending_displayID ? window->pending_displayID : SDL_GetDisplayForWindow(window);
		SDL_DisplayID current_did = SDL_GetDisplayForWindow(window);
		SDL_Rect bounds;
		SDL_zero(bounds);

		if (target_did) {
			SDL_GetDisplayBounds(target_did, &bounds);
		}

		const int local_x = window->pending.x - bounds.x;
		const int local_y = window->pending.y - bounds.y;

		if (target_did && current_did && (target_did != current_did) &&
		    !(window->flags & SDL_WINDOW_FULLSCREEN) &&
		    !(window->flags & SDL_WINDOW_EXTERNAL) &&
		    !data->videodata->CustomScreen) {

			window->pending_displayID = target_did;

			window->windowed.x = local_x;
			window->windowed.y = local_y;
			window->x = window->pending.x;
			window->y = window->pending.y;

			MOS_RecreateWindow(_this, window);

			SDL_SendWindowEvent(window, SDL_EVENT_WINDOW_MOVED, window->pending.x, window->pending.y);
			
			// grab mouse !
			data = (SDL_WindowData *)window->internal;
			data->warp_pending = true;
			MOS_FocusAndWarpIfNeeded(_this, data);
			return true;
		}

		SDL_Rect r;
		r.x = local_x;
		r.y = local_y;
		r.w = window->w;
		r.h = window->h;

		MOS_SetWindowBox(_this, window, &r);
		if (target_did && current_did && (target_did != current_did)) {
			SDL_SendWindowEvent(window, SDL_EVENT_WINDOW_DISPLAY_CHANGED, (Sint32)target_did, 0);
		}
		SDL_SendWindowEvent(window, SDL_EVENT_WINDOW_MOVED, window->pending.x, window->pending.y);
		/*
		const bool cross_display = (target_did && current_did && (target_did != current_did));
		const bool should_warp = cross_display || (window == SDL_GetMouseFocus());
		if ( !(window->flags & SDL_WINDOW_FULLSCREEN) && should_warp) {
			data->warp_pending = true;
			MOS_FocusAndWarpIfNeeded(_this, data);
		}
		*/
		
	}
	return true;
}

static int max(int a, int b)
{
    return (a > b) ? a : b;
}

static void
MOS_SetWindowLimits(SDL_Window * window, struct Window * syswin)
{
    const int borderWidth = syswin->BorderLeft + syswin->BorderRight;
    const int borderHeight = syswin->BorderTop + syswin->BorderBottom;

    const int minW = borderWidth + (window->min_w ? max(MIN_WINDOW_SIZE, window->min_w) : MIN_WINDOW_SIZE);
    const int minH = borderHeight + (window->min_h ? max(MIN_WINDOW_SIZE, window->min_h) : MIN_WINDOW_SIZE);

    const int maxW = window->max_w ? (borderWidth + window->max_w) : -1;
    const int maxH = window->max_h ? (borderHeight + window->max_h) : -1;

    BOOL ret = WindowLimits(syswin, minW, minH, maxW, maxH);

    if (!ret) {
        D("Setting window limits failed");
    }
}

bool
MOS_GetWindowBordersSize(SDL_VideoDevice *_this, SDL_Window * window, int *top, int *left, int *bottom, int *right)
{
	SDL_WindowData *data = (SDL_WindowData *) window->internal;
	
	if (!data || !data->win) {
		if (top) *top = 0;
		if (left) *left = 0;
		if (bottom) *bottom = 0;
		if (right) *right = 0;
		return true;
	}
	
	if (top)
		*top = data->win->BorderTop;
	
	if (left)
		*left = data->win->BorderLeft;
	
	if (bottom)
		*bottom = data->win->BorderBottom;
	
	if (right)
		*right = data->win->BorderRight;	
	
	return true;
}

void
MOS_SetWindowMinMaxSize(SDL_VideoDevice *_this, SDL_Window * window)
{
	if (window->flags & SDL_WINDOW_RESIZABLE) {
        SDL_WindowData *data = window->internal;
        MOS_SetWindowLimits(window, data->win);
    }
}

void
MOS_SetWindowSize(SDL_VideoDevice *_this, SDL_Window * window)
{
	D("");
	SDL_WindowData *data = (SDL_WindowData *) window->internal;

	if (data->win) {
		int width = 0, height = 0;
		MOS_GetWindowSize(data->win, &width, &height);
		if (width != window->pending.w || height != window->pending.h) {
			SDL_DisplayID current_did = SDL_GetDisplayForWindow(window);
			SDL_Rect bounds;
			SDL_zero(bounds);
			SDL_GetDisplayBounds(current_did, &bounds);
			const int local_x = window->pending.x - bounds.x;
			const int local_y = window->pending.y - bounds.y;
			
			SDL_Rect r;
			r.x = (window->pending.x != 0 ? local_x :  window->windowed.x);
			r.y = (window->pending.y != 0 ? local_y :  window->windowed.y);
			r.w = window->pending.w;
			r.h = window->pending.h;

			MOS_SetWindowBox(_this, window, &r);			
		}
	}
}

void
MOS_WindowToFront(struct Window *win)
{
	D("wnd 0x%08lx", win);
	WindowToFront(win);
	ActivateWindow(win);
}

static void
MOS_CenterWindow(struct Screen * screen, SDL_Window * window)
{
    if (SDL_WINDOWPOS_ISCENTERED(window->windowed.x) ||
        SDL_WINDOWPOS_ISUNDEFINED(window->windowed.x)) {

        window->x = window->windowed.x = (screen->Width - window->windowed.w) / 2;
        //D("X centered %d", window->x);
    }

    if (SDL_WINDOWPOS_ISCENTERED(window->windowed.y) ||
        SDL_WINDOWPOS_ISUNDEFINED(window->windowed.y)) {

        window->y = window->windowed.y = (screen->Height - window->windowed.h) / 2;
        //D("Y centered %d", window->y);
    }
}

static void
MOS_DefineWindowBox(SDL_Window * window, struct Screen * screen, bool fullscreen, SDL_Rect * box)
{

    if (screen) {
        if (fullscreen) {
            box->x = 0;
            box->y = 0;
            box->w = screen->Width;
            box->h = screen->Height;
        } else {
            MOS_CenterWindow(screen, window);

            box->x = window->windowed.x;
            box->y = window->windowed.y;
            box->w = window->windowed.w;
            box->h = window->windowed.h;
        }
    } else {
        box->x = window->windowed.x;
        box->y = window->windowed.y;
        box->w = window->windowed.w;
        box->h = window->windowed.h;
    }
}

static ULONG
MOS_GetIDCMPFlags(SDL_Window * window, bool fullscreen)
{
    ULONG IDCMPFlags = IDCMP_RAWKEY 
						| IDCMP_MOUSEMOVE 
						| IDCMP_DELTAMOVE 
						| IDCMP_MOUSEBUTTONS 
						| IDCMP_REFRESHWINDOW 
						| IDCMP_ACTIVEWINDOW 
						| IDCMP_INACTIVEWINDOW;
	
	if (!fullscreen) {
		if (!(window->flags & SDL_WINDOW_BORDERLESS)) {
			IDCMPFlags |= IDCMP_CLOSEWINDOW | IDCMP_GADGETUP | IDCMP_CHANGEWINDOW | IDCMP_MENUPICK;
		}
		if (window->flags & SDL_WINDOW_RESIZABLE) {
			IDCMPFlags |= IDCMP_NEWSIZE;
		}
	}

    return IDCMPFlags;
}

static ULONG
MOS_GetWindowFlags(SDL_Window * window, bool fullscreen)
{
    ULONG windowFlags = WFLG_SIMPLE_REFRESH/* WFLG_SMART_REFRESH | WFLG_NOCAREREFRESH*/ | WFLG_RMBTRAP | WFLG_REPORTMOUSE | WFLG_ACTIVATE;

	if (fullscreen) {
		windowFlags |= WFLG_BORDERLESS /*| WFLG_BACKDROP*/;
	} else {
		if (window->flags & SDL_WINDOW_BORDERLESS) {
			windowFlags |= WFLG_BORDERLESS;
		} else {
			windowFlags |= WFLG_DRAGBAR | WFLG_DEPTHGADGET | WFLG_CLOSEGADGET;
		}
		if (window->flags & SDL_WINDOW_RESIZABLE) {
			windowFlags |= WFLG_SIZEGADGET | WFLG_SIZEBBOTTOM;
		}
	}

    return windowFlags;
}

static SDL_DisplayID MOS_GetFallbackDisplay(SDL_VideoDevice *_this, SDL_VideoData *vd, SDL_Window *window, SDL_DisplayID failed_did)
{
	
	/*static int g_recreate_serial = 0;
	D("Recreate #%d title='%s' flags=0x%08x pending_displayID=%d pending=(%d,%d %dx%d)",
		  ++g_recreate_serial, window->title, (unsigned)window->flags,
		  (int)window->pending_displayID,
		  window->pending.x, window->pending.y, window->pending.w, window->pending.h);*/
	
    SDL_Window *focus = SDL_GetKeyboardFocus();
    if (focus) {
        SDL_DisplayID did = SDL_GetDisplayForWindow(focus);
        if (did && did != failed_did) return did;
    }

    SDL_DisplayID did2 = SDL_GetDisplayForWindow(window);
    if (did2 && did2 != failed_did) return did2;
    SDL_DisplayID primary = SDL_GetPrimaryDisplay();
    if (primary && primary != failed_did) return primary;
    return 0;
}

bool MOS_CreateSystemWindow(SDL_VideoDevice *_this, SDL_Window *window)
{
    SDL_WindowData *wd = (SDL_WindowData *)window->internal;
    SDL_VideoData *videodata = wd->videodata;

    if (!videodata->PublicScreen && !videodata->CustomScreen) {
        return false;
    }

    if (wd->win || (window->flags & SDL_WINDOW_MINIMIZED)) {
        if (wd->win) {
            MOS_WindowToFront(wd->win);
        }
        return true;
    }

    const bool fullscreen = (window->flags & SDL_WINDOW_FULLSCREEN) != 0;

    if (!wd->window_title) {
        wd->window_title = MOS_ConvertText(window->title, MIBENUM_UTF_8, MIBENUM_SYSTEM);
    }

    float op = window->opacity;
    if (op < 0.0f) op = 0.0f;
    if (op > 1.0f) op = 1.0f;
    ULONG opacity_value = (ULONG)(op * (float)ULONG_MAX);
    if (window->flags & SDL_WINDOW_HIDDEN) {
        opacity_value = 0;
    }

    const ULONG windowFlags = MOS_GetWindowFlags(window, fullscreen);
    const ULONG IDCMPFlags  = MOS_GetIDCMPFlags(window, fullscreen);

    SDL_DisplayID failed_did = 0;
    for (int attempt = 0; attempt < 2; attempt++) {

        SDL_DisplayID did = 0;
        SDL_VideoDisplay *vdpy = NULL;
        SDL_DisplayData *dd = NULL;
        const char *pubname = NULL;

        if (!videodata->CustomScreen) {

			if (attempt == 0) {
				did = window->pending_displayID ? window->pending_displayID : SDL_GetDisplayForWindow(window);
			} else {
				did = MOS_GetFallbackDisplay(_this, videodata, window, failed_did);
				window->pending_displayID = 0;
			}

            if (did) {
                vdpy = SDL_GetVideoDisplay(did);
                dd = vdpy ? (SDL_DisplayData *)vdpy->internal : NULL;
                pubname = (dd && dd->pubscreen_name[0]) ? dd->pubscreen_name : NULL;
            }
        }

        SDL_Rect box;
        SDL_zero(box);

		struct Screen *ref_screen = NULL;
		struct Screen *locked = NULL;

		if (videodata->CustomScreen) {
			ref_screen = videodata->CustomScreen;
		} else if (pubname) {
			locked = LockPubScreen((CONST_STRPTR)pubname);
			if (locked) {
				ref_screen = locked;
			}
		}

		if (!ref_screen) {
			ref_screen = videodata->PublicScreen;
		}

		MOS_DefineWindowBox(window, ref_screen, fullscreen, &box);

		if (locked) {
			UnlockPubScreen(NULL, locked);
		}
		
		// count Display available
		int numDisplay = 0;
		SDL_DisplayID *displays = SDL_GetDisplays(&numDisplay);
		if (displays) SDL_free(displays);
		ULONG extragadget = ETG_ICONIFY;
		if (numDisplay > 1) {
			extragadget |= ETG_JUMP;
		}
		
        wd->win = OpenWindowTags(NULL,
            WA_Left, box.x,
            WA_Top, box.y,
            WA_InnerWidth, box.w,
            WA_InnerHeight, box.h,
            WA_Flags, windowFlags,

            videodata->CustomScreen ? WA_CustomScreen : (pubname ? WA_PubScreenName : WA_PubScreen),
            videodata->CustomScreen ? (IPTR)videodata->CustomScreen : (pubname ? (IPTR)pubname : (IPTR)videodata->PublicScreen),

            WA_ScreenTitle, wd->window_title,
            (window->flags & SDL_WINDOW_BORDERLESS || fullscreen) ? TAG_IGNORE : WA_Title, wd->window_title,

            WA_UserPort, &videodata->userPort,
            WA_Opacity, opacity_value,
            WA_FrontWindow, (window->flags & SDL_WINDOW_ALWAYS_ON_TOP) ? TRUE : FALSE,
            WA_IDCMP, IDCMPFlags,
            WA_ExtraTitlebarGadgets, extragadget,
            TAG_DONE);

        if (wd->win) {
            SDL_PropertiesID props = SDL_GetWindowProperties(window);
            SDL_SetPointerProperty(props, "SDL.window.morphos.window", wd->win);

            if ((window->flags & SDL_WINDOW_RESIZABLE) && !fullscreen) {
                MOS_SetWindowLimits(window, wd->win);
            }

            wd->first_deltamove = TRUE;
            wd->win->UserData = (APTR)wd;
			
			window->pending_displayID = 0;
			
            MOS_CreateAppWindow(_this, window);
            bool withmenu = SDL_GetBooleanProperty(props, SDL_PROP_WINDOW_MORPHOS_MENU_BOOLEAN, true);
			if (withmenu) {
				MOS_CreateMenu(_this, window);
			}

            if (wd->grabbed) {
                DoMethod((Object *)wd->win, WM_ObtainEvents);
            }

            return true;
        }


        if (!videodata->CustomScreen) {
            D("Failed to create Window attempt=%d (pubname=%s did=%lu box=%ld,%ld %ldx%ld flags=0x%08lx idcmp=0x%08lx)",
              attempt,
              pubname ? pubname : "<none>",
              (ULONG)did,
              (LONG)box.x, (LONG)box.y, (LONG)box.w, (LONG)box.h,
              (ULONG)windowFlags,
              (ULONG)IDCMPFlags);

            failed_did = did;
        } else {
            D("Failed to create Window attempt=%d (CustomScreen=%p box=%ld,%ld %ldx%ld flags=0x%08lx idcmp=0x%08lx)",
              attempt,
              videodata->CustomScreen,
              (LONG)box.x, (LONG)box.y, (LONG)box.w, (LONG)box.h,
              (ULONG)windowFlags,
              (ULONG)IDCMPFlags);

            break;
        }

        if (attempt == 1) {
            break;
        }
    }

    return false;
}

void
MOS_ShowWindow(SDL_VideoDevice *_this, SDL_Window * window)
{
	D("");
	SDL_WindowData *data = (SDL_WindowData *) window->internal;
	if (data->appIcon) {
        D("Window '%s' is in iconified (minimized) stated, ignoring show request", window->title);
        return;
    }
	
	if (data->win) {
		ULONG value = ((1.0) * (ULONG_MAX));
		MOS_SetWindowOpacityPrivate(_this, window, value);
		SDL_SendWindowEvent(data->window, SDL_EVENT_WINDOW_SHOWN, 0, 0);
		if (window->flags & SDL_WINDOW_FULLSCREEN) {
			ScreenToFront(data->win->WScreen);
		}

		ActivateWindow(data->win);

		SDL_SetKeyboardFocus(window);	
		
		SDL_Mouse *mouse = SDL_GetMouse();
		if (mouse) {
			SDL_SetCursor(NULL);
		}

	}
}

void
MOS_HideWindow(SDL_VideoDevice *_this, SDL_Window * window)
{
	D("");
	SDL_WindowData *data = (SDL_WindowData *) window->internal;
	if (window->is_destroying) {
        return;
    }
	if (data->win) {
		ULONG value = ((0.0) * (ULONG_MAX));
		if (MOS_SetWindowOpacityPrivate(_this, window, value)) {
			SDL_SendWindowEvent(data->window, SDL_EVENT_WINDOW_HIDDEN, 0, 0);
		}
	}
	
}

void
MOS_IconifyWindow(SDL_VideoDevice *_this, bool with_appicon, SDL_Window * window)
{
	SDL_VideoData *videodata = (SDL_VideoData *) _this->internal;
	SDL_WindowData *data = window->internal;

	if (window->flags & SDL_WINDOW_MINIMIZED) {
		D("Window '%s' is already iconified", window->title);
	} else if ((window->flags & SDL_WINDOW_FULLSCREEN)) {
		D("Window '%s' is into fullscreen", window->title);
	} else {
		if (videodata->AppIcon) {
			data->appIcon = AddAppIconA(0, (ULONG)window, (UBYTE*)FilePart(videodata->FullAppName), &videodata->appMsgPort, 0, videodata->AppIcon, NULL);	

			if (data->appIcon) {
				D("iconifying '%s' - window=%p",  window->title, window);
				MOS_HideWindow(_this, window);
				SDL_SendWindowEvent(window, SDL_EVENT_WINDOW_MINIMIZED, 0, 0);
			} else {
				D("[Failed to add AppIcon");
			}
		} else {
			D("Failed to load icon");
		}
	}

}

void
MOS_UniconifyWindow(SDL_VideoDevice *_this, SDL_Window * window)
{
	D("");
	SDL_WindowData *data = window->internal;

	if (data->appIcon) {
		MOS_RemoveAppIcon(data);
		MOS_ShowWindow(_this, window);
        SDL_SendWindowEvent(window, SDL_EVENT_WINDOW_RESTORED, 0, 0);	
	} else {
		D("Window '%s' isn't in iconified (minimized) state", window->title);
	}
}

void
MOS_RaiseWindow(SDL_VideoDevice *_this, SDL_Window * window)
{
	SDL_WindowData *data = (SDL_WindowData *) window->internal;
	D("wnd 0x%08lx", data->win);

	if (data->win) {
		if (window->flags & SDL_WINDOW_FULLSCREEN) {
			ScreenToFront(data->win->WScreen);
		}
		MOS_WindowToFront(data->win);
	}
	
}

void
MOS_SetWindowBox(SDL_VideoDevice *_this, SDL_Window * window, SDL_Rect * rect)
{
    SDL_WindowData *data = window->internal;

    if (data->win) {
		//D("wnd 0x%08lx Resize x=%d, y=%d, w=%d, h=%d", data->win, rect->x, rect->y, rect->w, rect->h);

        SetAttrs(data->win,
			WA_Left, rect->x,
			WA_Top, rect->y,
			rect->w == 0 ? TAG_IGNORE : WA_InnerWidth, rect->w,
			rect->h == 0 ? TAG_IGNORE : WA_InnerHeight, rect->h,
			TAG_DONE);
			
		//if (data->__tglContext && (rect->w > 0 && rect->h > 0)) {
		//	MOS_GL_ResizeContext(_this, window);
		//}

    }
}

void
MOS_MaximizeWindow(SDL_VideoDevice *_this, SDL_Window * window)
{
	D("");
	SDL_WindowData *data = (SDL_WindowData *) window->internal;
	
	if (data->win) {
		if (window->flags & SDL_WINDOW_FULLSCREEN) {
			D("What ?? you are with fullscreen mode! why ask MAXIMIZE?");
			return;
		}
		
		struct Window* syswin = data->win;
		struct Screen* screen = syswin->WScreen;
		const int borderWidth = syswin->BorderLeft + syswin->BorderRight;
		const int borderHeight = syswin->BorderTop + syswin->BorderBottom;
		
		const int width = window->max_w ? window->max_w : (screen->Width - borderWidth);
		const int height = window->max_h ? window->max_h : (screen->Height - borderHeight);
	
		if (window->flags & SDL_WINDOW_MINIMIZED) {
			MOS_UniconifyWindow(_this, window);
		}
	
		int top = 0;
		APTR di = GetScreenDrawInfo(screen);
		top += GetSkinInfoAttrA(di, SI_ScreenTitlebarHeight, NULL) +1;
		FreeScreenDrawInfo(screen, di);
		
		SDL_Rect rect;
		rect.x = 0;
		rect.y = top + syswin->BorderTop;
		rect.w = width;
		rect.h = height - syswin->BorderTop;
		
		D("Maximizing '%s' at %d %d to w=%d h=%d", window->title, 0, top, width, height);
		
		MOS_SetWindowBox(_this, window, &rect);
		
		SDL_SendWindowEvent(window, SDL_EVENT_WINDOW_MAXIMIZED, 0, 0);
	}	

}

void
MOS_MinimizeWindow(SDL_VideoDevice *_this, SDL_Window * window)
{
	D("");
	
	MOS_IconifyWindow(_this, true, window);
}

void
MOS_RestoreWindow(SDL_VideoDevice *_this, SDL_Window * window)
{
#ifdef __SDL_DEBUG
	SDL_WindowData *data = (SDL_WindowData *) window->internal;
	D("wnd 0x%08lx", data->win);
#endif

	if (window->flags & SDL_WINDOW_MINIMIZED) {
		D("Restoring iconified '%s'", window->title);
		MOS_UniconifyWindow(_this, window);		
	} else if (window->flags & SDL_WINDOW_MAXIMIZED) {
		D("Restoring '%s' to x %d, y %d, w %d, h %d", window->title,
		window->floating.x, window->floating.y, window->floating.w, window->floating.h);

		MOS_SetWindowBox(_this, window, &window->floating);
		
		SDL_SendWindowEvent(window, SDL_EVENT_WINDOW_RESTORED, 0, 0);
	} else {
		D("Don't know what to do");
	}
}

#ifdef __SDL_DEBUG	
static const char*
MOS_DecodeFullscreenOp(SDL_FullscreenOp fullscreen)
{
    switch(fullscreen) {
        case SDL_FULLSCREEN_OP_LEAVE:
            return "leave fullscreen";
        case SDL_FULLSCREEN_OP_ENTER:
            return "enter fullscreen";
        case SDL_FULLSCREEN_OP_UPDATE:
            return "update";
    }

    return "unknown";
}
#endif

SDL_FullscreenResult
MOS_SetWindowFullscreen(SDL_VideoDevice *_this, SDL_Window *window,
                        SDL_VideoDisplay *display, SDL_FullscreenOp fullscreen)
{
    if (window->is_destroying) {
        D("Window '%s' is being destroyed, mode change ignored", window->title);
        return SDL_FULLSCREEN_SUCCEEDED;
    }

    SDL_WindowData *data = (SDL_WindowData *)window->internal;
	
    if (window->flags & SDL_WINDOW_EXTERNAL) {
        D("Native window '%s' (%p), mode change ignored", window->title, data->win);
        return SDL_FULLSCREEN_SUCCEEDED;
    }

    /* If LEAVE requested but we are already windowed, ignore */
    if (fullscreen == SDL_FULLSCREEN_OP_LEAVE && (window->flags & SDL_WINDOW_FULLSCREEN) == 0) {
        return SDL_FULLSCREEN_SUCCEEDED;
    }
		
    SDL_VideoDisplay *orig = display;
    SDL_VideoDisplay *target_display = display;
    SDL_DisplayID target_did = 0;

    if (fullscreen == SDL_FULLSCREEN_OP_ENTER || fullscreen == SDL_FULLSCREEN_OP_UPDATE) {
        /* Honor pending_displayID first (classic: move/center then fullscreen) */
		if (window->pending_displayID) {
			target_did = window->pending_displayID;
		} else {
			SDL_DisplayID where = SDL_GetDisplayForWindow(window);
			if (where) {
				target_did = where;
			} else if (window->requested_fullscreen_mode.displayID) {
				target_did = window->requested_fullscreen_mode.displayID;
			} else if (window->current_fullscreen_mode.displayID) {
				target_did = window->current_fullscreen_mode.displayID;
			} else if (display) {
				target_did = display->id;
			}
		}

        if (target_did) {
            SDL_VideoDisplay *d = SDL_GetVideoDisplay(target_did);
            if (d) {
                target_display = d;
            }
        }
    } else {
        /* LEAVE: keep whatever SDL/core passes, we will restore windowed geom below */
        if (window->current_fullscreen_mode.displayID) {
			target_did = window->current_fullscreen_mode.displayID;
		} else if (window->requested_fullscreen_mode.displayID) {
			target_did = window->requested_fullscreen_mode.displayID;
		} else if (display) {
			target_did = display->id;
		}

		if (target_did) {
			SDL_VideoDisplay *d = SDL_GetVideoDisplay(target_did);
			if (d) {
				target_display = d;
			}
		} else {
			target_display = display;
		}
    }

    if (target_display) {
        target_did = target_display->id;
    }

    D("FS: op=%s, orig_display=%d forced_display=%d (req=%d cur=%d pending=%d target=%d)",
      MOS_DecodeFullscreenOp(fullscreen),
      orig ? (int)orig->id : 0,
      target_display ? (int)target_display->id : 0,
      (int)window->requested_fullscreen_mode.displayID,
      (int)window->current_fullscreen_mode.displayID,
      (int)window->pending_displayID,
      (int)target_did);
	  
    /* Save/restore windowed geometry (and keep window->windowed in sync for MOS_DefineWindowBox) */
    if (fullscreen == SDL_FULLSCREEN_OP_ENTER) {
        if ((window->flags & SDL_WINDOW_FULLSCREEN) == 0 && data->win) {
            int ww = 0, hh = 0;
            MOS_GetWindowSize(data->win, &ww, &hh);

			data->old_x = window->x;   // global SDL !
			data->old_y = window->y;
			data->old_w = window->w;
			data->old_h = window->h;

            D("Save windowed position %d,%d and size %d,%d",
              data->old_x, data->old_y, data->old_w, data->old_h);
        }
    } else if (fullscreen == SDL_FULLSCREEN_OP_LEAVE) {
        if (data->old_w && data->old_h) {
			SDL_DisplayID did = window->current_fullscreen_mode.displayID;
			if (!did) did = window->requested_fullscreen_mode.displayID;
			if (!did && target_did) did = target_did;

			SDL_Rect bounds;
			SDL_zero(bounds);
			if (did) SDL_GetDisplayBounds(did, &bounds);

			window->x = data->old_x;
			window->y = data->old_y;
			window->w = data->old_w;
			window->h = data->old_h;

			window->windowed.x = data->old_x - bounds.x;
			window->windowed.y = data->old_y - bounds.y;
			window->windowed.w = data->old_w;
			window->windowed.h = data->old_h;

			window->pending.x = data->old_x;
			window->pending.y = data->old_y;
			window->pending.w = data->old_w;
			window->pending.h = data->old_h;

			data->old_w = data->old_h = data->old_x = data->old_y = 0;
        }
    }

    if (fullscreen == SDL_FULLSCREEN_OP_LEAVE) {
        SDL_SendWindowEvent(window, SDL_EVENT_WINDOW_LEAVE_FULLSCREEN, 0, 0);
    } else if (fullscreen == SDL_FULLSCREEN_OP_ENTER) {
        SDL_SendWindowEvent(window, SDL_EVENT_WINDOW_ENTER_FULLSCREEN, 0, 0);
    }

    if ((fullscreen == SDL_FULLSCREEN_OP_ENTER || fullscreen == SDL_FULLSCREEN_OP_UPDATE) && target_did) {
        window->pending_displayID = target_did;
        window->requested_fullscreen_mode.displayID = target_did;
        window->current_fullscreen_mode.displayID   = target_did;
        D("ENTER/UPDATE: forcing pending_displayID=%d", (int)window->pending_displayID);
    }

    if (fullscreen == SDL_FULLSCREEN_OP_LEAVE) {
        SDL_DisplayID did = 0;

        /* prefer the fullscreen display (current/req), then orig/target display */
        if (window->current_fullscreen_mode.displayID) did = window->current_fullscreen_mode.displayID;
        if (!did && window->requested_fullscreen_mode.displayID) did = window->requested_fullscreen_mode.displayID;
        if (!did && target_did) did = target_did;
        if (!did && orig) did = orig->id;

        if (did) {
            window->pending_displayID = did;
        }

        D("LEAVE: forcing pending_displayID=%d", (int)window->pending_displayID);
		
		if (data->win) {
			MOS_CloseWindow(_this, window);
		}

        MOS_CloseScreen(_this);
    }

    MOS_RecreateWindow(_this, window);

    if (data->win) {
        //MOS_ShowWindow(_this, window);

        int width = 0, height = 0;
        MOS_GetWindowSize(data->win, &width, &height);
		if (data->__tglContext) {
            MOS_GL_ResizeContext(_this, window);
        }
		
        D("Force to inform SDL about window size %d x %d", width, height);
        SDL_SendWindowEvent(window, SDL_EVENT_WINDOW_RESIZED, width, height);

        if (fullscreen == SDL_FULLSCREEN_OP_ENTER) {
            if (window->flags & SDL_WINDOW_MAXIMIZED) {
                data->wasMaximized = TRUE;
                window->flags &= ~SDL_WINDOW_MAXIMIZED;
            }
            SDL_SetMouseFocus(window);
        } else if (fullscreen == SDL_FULLSCREEN_OP_LEAVE) {
            if (data->wasMaximized) {
                window->flags |= SDL_WINDOW_MAXIMIZED;
                data->wasMaximized = FALSE;
            }
			SDL_zero(window->requested_fullscreen_mode);
			SDL_zero(window->current_fullscreen_mode);
        }
    }

	
    return SDL_FULLSCREEN_SUCCEEDED;
}

bool
MOS_SetWindowGrab(SDL_VideoDevice *_this, SDL_Window *window, bool grabbed)
{
	SDL_WindowData *data = (SDL_WindowData *) window->internal;

	if (data->win && data->grabbed != grabbed) {
		D("grabbed=%d", grabbed);

		data->grabbed = grabbed;
		if ((window->flags & SDL_WINDOW_FULLSCREEN) == 0) {
			if (grabbed && (data->win->Flags & WFLG_WINDOWACTIVE) == 0) {
				ActivateWindow(data->win);
			}

			DoMethod((Object *)data->win, grabbed ? WM_ObtainEvents : WM_ReleaseEvents);
		} else {
			ActivateWindow(data->win);
		}
	}
	
	return true;
}

void
MOS_SetWindowAlwaysOnTop(SDL_VideoDevice *_this, SDL_Window * window, bool on_top)
{
	D("");
	SDL_WindowData *data = window->internal;
	if (data->win) {
		MOS_RecreateWindow(_this, window);
	}
}

bool
MOS_SetWindowHitTest(SDL_Window *window, bool enabled)
{
    return true;
}

void
MOS_SetWindowResizable(SDL_VideoDevice *_this, SDL_Window * window, bool resizable)
{
	D("");
	MOS_RecreateWindow(_this, window);
}

void
MOS_SetWindowBordered(SDL_VideoDevice *_this, SDL_Window * window, bool bordered)
{
	D("");
	MOS_RecreateWindow(_this, window);
}

bool
MOS_SetWindowOpacityPrivate(SDL_VideoDevice *_this, SDL_Window * window, ULONG value)
{
	SDL_WindowData *data = (SDL_WindowData *) window->internal;
	
	LONG ret = SetAttrs(data->win, WA_Opacity, value, TAG_DONE);
	if (ret) {
		D("Failed to set window opaqueness to %lu", value);
		return false;
	}
	
	return true;
}

bool
MOS_SetWindowOpacity(SDL_VideoDevice *_this, SDL_Window * window, float opacity)
{
    SDL_WindowData *data = (SDL_WindowData *) window->internal;
	if (!data->win) 
		return true;
	
    ULONG value = ((opacity) * (ULONG_MAX));

	D("set window opaqueness to %lu", value);
	
    return MOS_SetWindowOpacityPrivate(_this, window, value) ? true : false;
}

static void
MOS_FlashOperationPrivate(SDL_VideoDevice *_this, SDL_Window * window)
{
	SDL_WindowData *data = (SDL_WindowData *) window->internal;

    if (data->win) {

		ULONG value_old = getv(data->win, WA_Opacity);
	
		WindowToFront(data->win);
		const Uint32 start = SDL_GetTicks();
		ULONG elapsed = 0;
		ULONG value = 0;
		
		while (TRUE) {
			elapsed = SDL_GetTicks() - start;
			if (elapsed > 200) break;
			value = 128 + 127 * sinf(elapsed * 3.14159f / 50.0f) * (ULONG_MAX);
			MOS_SetWindowOpacityPrivate(_this, window, value);	
			SDL_Delay(1);
		}

		MOS_SetWindowOpacityPrivate(_this, window, value_old);
	}
}

bool
MOS_FlashWindow(SDL_VideoDevice *_this, SDL_Window * window, SDL_FlashOperation operation)
{
	D("");
	
	SDL_WindowData *data = (SDL_WindowData *) window->internal;
	if (!data->win) return 0;
	WindowToFront(data->win);
	switch(operation)
	{
		case SDL_FLASH_BRIEFLY:
		case SDL_FLASH_UNTIL_FOCUSED:
			MOS_FlashOperationPrivate(_this, window);
			break;
		case SDL_FLASH_CANCEL:
			break;
	}
	ActivateWindow(data->win);
	
	return true;
}
