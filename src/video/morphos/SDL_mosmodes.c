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

#include "SDL_mosmodes.h"
#include "SDL_mosvideo.h"
#include "SDL_moswindow.h"
#include "SDL_misc.h"

#include <cybergraphx/cybergraphics.h>
#include <intuition/extensions.h>
#include <intuition/monitorclass.h>
#include <proto/alib.h>
#include <proto/cybergraphics.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/screennotify.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <graphics/rpattr.h>

typedef struct PubScreenInfo {
    APTR  monitor;
    ULONG modeid;
	char  name[MAXPUBSCREENNAME + 1];
} PubScreenInfo;

static void MOS_SetDisplayName(SDL_VideoDisplay *d, APTR monitor)
{
    STRPTR sysname = NULL;

    if (!d || !monitor) {
        return;
    }

    GetAttr(MA_MonitorName, monitor, (ULONG *)&sysname);
    if (!sysname || !sysname[0]) {
        return;
    }

    if (d->name && SDL_strcmp(d->name, (const char *)sysname) == 0) {
        return;
    }

    char *dup = SDL_strdup((const char *)sysname);
    if (!dup) {
        return;
    }

    SDL_free(d->name);
    d->name = dup;
}


void MOS_CloseScreen(SDL_VideoDevice *_this)
{
	D("");
	SDL_VideoData *data = (SDL_VideoData *) _this->internal;
    if (data->CustomScreen && data->CustomScreen != data->PublicScreen) {
        D("Trying to closing custom screen %p", data->CustomScreen);
        if (!CloseScreen(data->CustomScreen)) {
            D("Screen has open window(s), cannot close");
        } else {
			data->CustomScreen = NULL;
        }
    }
}

void MOS_CloseDisplay(SDL_VideoDevice *_this, bool hide)
{
	D("");
    SDL_VideoData *data = (SDL_VideoData *) _this->internal;
    MOS_CloseScreen(_this);

    if (data->ScreenNotifyHandle && hide == false) {
        int tries = 200; // ~2s avec Delay(1)
        while (!RemWorkbenchClient(data->ScreenNotifyHandle) && --tries > 0) {
            Delay(1);
        }
        if (tries == 0) {

        } else {
            data->ScreenNotifyHandle = NULL;
        }
    }

    if (data->PublicScreen) {
        if (data->ScreenSaverSuspendCount) {
            for (size_t i = data->ScreenSaverSuspendCount; i > 0; i--) {
                SetAttrs(data->PublicScreen, SA_StopBlanker, FALSE, TAG_DONE);
            }
            data->ScreenSaverSuspendCount = 0;
        }
        UnlockPubScreen(NULL, data->PublicScreen);
        data->PublicScreen = NULL;
    }
	

    struct ScreenNotifyMessage *snm;
    while ((snm = (struct ScreenNotifyMessage *)GetMsg(&data->ScreenNotifyPort)) != NULL) {
        ReplyMsg((struct Message *)snm);
    }
}

bool MOS_OpenDisplay(SDL_VideoDevice *_this)
{
	D("");
    SDL_VideoData *data = (SDL_VideoData *) _this->internal;
    if (data->PublicScreen) {
        return true;
    }

    struct Screen *s = LockPubScreen(NULL);
    if (!s) {
        D("LockPubScreen failed");
        return false;
    }

    data->PublicScreen = s;
	
    return true;
}

static bool MOS_GetDisplayMode(ULONG id, SDL_DisplayMode * mode)
{
    SDL_DisplayModeData *data;
    APTR handle;
    struct DimensionInfo diminfo;
	struct MonitorInfo moninfo;

    handle = FindDisplayInfo(id);
    if (!handle) {
        return false;
    }

    if (!GetDisplayInfoData(handle, (UBYTE *)&diminfo, sizeof(diminfo), DTAG_DIMS, 0)) {
        D("Failed to get dim info");
        return false;
    }

    if (!GetDisplayInfoData(NULL, (UBYTE *)&moninfo, sizeof(moninfo), DTAG_MNTR, id)) {
        D("Failed to get monitor info");
        return false;
    }

    data = (SDL_DisplayModeData *) SDL_malloc(sizeof(*data));
    if (!data) {
    	D("Failed Out Of memory\n");
        return false;
    }

    SDL_zero(*mode);
    data->modeid = id;
    data->x = diminfo.Nominal.MinX;
    data->y = diminfo.Nominal.MinY;
    mode->w = diminfo.Nominal.MaxX - diminfo.Nominal.MinX + 1;
    mode->h = diminfo.Nominal.MaxY - diminfo.Nominal.MinY + 1;
	mode->pixel_density = 1.0f;
    mode->refresh_rate = 0.0f;
    if (moninfo.TotalRows > 0 && moninfo.TotalColorClocks > 0) {
        const double denom = (double)moninfo.TotalRows * (double)moninfo.TotalColorClocks * 280.0e-9;
        if (denom > 0.0) {
            mode->refresh_rate = (float)(1.0 / denom);
        }
    }
    mode->format = SDL_PIXELFORMAT_UNKNOWN;
		
	switch (diminfo.MaxDepth) {
	case 32:
		mode->format = SDL_PIXELFORMAT_ARGB8888;
		break;
	case 24:
		mode->format = SDL_PIXELFORMAT_RGB24;
		break;
	case 16:
		mode->format = SDL_PIXELFORMAT_RGB565;
		break;
	case 15:
		mode->format = SDL_PIXELFORMAT_XRGB1555;
		break;
	case 8:
		mode->format = SDL_PIXELFORMAT_INDEX8;
		break;
	}

    mode->internal = data;

    return true;
}

bool
MOS_GetDisplayModes(SDL_VideoDevice *_this, SDL_VideoDisplay * display)
{
	SDL_DisplayData *displaydata = (SDL_DisplayData *) display->internal;
	D("Get mode from monitor %lu", displaydata->monitor);
	
    SDL_DisplayMode displaymode;

	Object **lmodes = GetMonitorModesList(displaydata->monitor, NULL);
	if (lmodes)
	{
		int modei = 0;
		while (lmodes[modei])
		{
			Boopsiobject *mode = lmodes[modei++];
			ULONG modeid = INVALID_ID;
			GetAttr(MA_Mode_ModeID, mode, (ULONG*)&modeid);
			if (MOS_GetDisplayMode(modeid, &displaymode)) {
				if (displaymode.format != SDL_PIXELFORMAT_UNKNOWN
					&& displaymode.format != SDL_PIXELFORMAT_INDEX8) { // TODO
					if (!SDL_AddFullscreenDisplayMode(display, &displaymode)) {
						SDL_free(displaymode.internal);
					}
				} else {
					SDL_free(displaymode.internal);
				}
			} else {
				FreeMonitorModesList(lmodes);
				D("Failed to get display mode for %lu",  modeid);
				return false;
			}
						
		}
		FreeMonitorModesList(lmodes);
	} 
	else 
	{
		D("Failed to get display mode");
		return false;
	}   
    
    return true;
}

static const PubScreenInfo *
MOS_FindPubScreenForMonitor(APTR monitor, const PubScreenInfo *list, int count)
{
    if (!monitor || !list || count <= 0) {
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        if (list[i].monitor == monitor) {
            return &list[i];
        }
    }
    return NULL;
}

static int
MOS_CollectPublicScreens(PubScreenInfo *out, int max)
{
    if (!out || max <= 0) return 0;

    char names[64][MAXPUBSCREENNAME + 1];
    int count = 0;

    struct List *pslist = LockPubScreenList();
    if (!pslist) return 0;

    for (struct Node *node = pslist->lh_Head;
         node && node->ln_Succ && count < (int)SDL_arraysize(names) && count < max;
         node = node->ln_Succ)
    {
        const char *name = node->ln_Name;
        if (!name || !name[0]) continue;

        SDL_strlcpy(names[count], name, sizeof(names[count]));
        count++;
    }

    UnlockPubScreenList();

    int n = 0;
    for (int i = 0; i < count && n < max; i++) {
        struct Screen *s = LockPubScreen((CONST_STRPTR)names[i]);
        if (!s) continue;

        SDL_strlcpy(out[n].name, names[i], sizeof(out[n].name));
        out[n].monitor = (APTR)getv(s, SA_MonitorObject);
        out[n].modeid  = (ULONG)getv(s, SA_DisplayID);

        UnlockPubScreen(NULL, s);
        n++;
    }

    return n;
}

SDL_DisplayID 
MOS_GetDisplayForWindow(SDL_VideoDevice *_this, SDL_Window *window)
{
    SDL_WindowData *wd = (SDL_WindowData *)window->internal;
    if (!wd || !wd->win || !wd->win->WScreen) {
        return 0;
    }

    APTR mon = (APTR)getv(wd->win->WScreen, SA_MonitorObject);

    for (int i = 0; i < _this->num_displays; i++) {
        SDL_VideoDisplay *d = _this->displays[i];
        if (!d) continue;
        SDL_DisplayData *dd = (SDL_DisplayData *)d->internal;
        if (dd && dd->monitor == mon) {
            return d->id;
        }
    }
    return 0;
}

bool 
MOS_InitModes(SDL_VideoDevice *_this)
{
    D("");

    SDL_VideoData *data = (SDL_VideoData *) _this->internal;

    PubScreenInfo pubs[16];
    const int pubCount = MOS_CollectPublicScreens(pubs, 16);

    data->CustomScreen = NULL;
    data->PublicScreen = LockPubScreen(NULL);
    if (!data->PublicScreen) {
        D("LockPubScreen failed");
        return SDL_OutOfMemory();
    }

    APTR default_mon = (APTR)getv(data->PublicScreen, SA_MonitorObject);
    const ULONG default_modeid = (ULONG)getv(data->PublicScreen, SA_DisplayID);

    {
        SDL_DisplayMode mode;
        SDL_VideoDisplay display;
        SDL_DisplayData *dd = (SDL_DisplayData *) SDL_calloc(1, sizeof(*dd));
        if (!dd) {
            UnlockPubScreen(NULL, data->PublicScreen);
            data->PublicScreen = NULL;
            return SDL_OutOfMemory();
        }

        dd->monitor = default_mon;
        dd->screen = NULL;
	
        const PubScreenInfo *ps = MOS_FindPubScreenForMonitor(default_mon, pubs, pubCount);
        const char *pubname = (ps && ps->name[0]) ? ps->name : NULL;
        if (pubname) {
            SDL_strlcpy(dd->pubscreen_name, pubname, sizeof(dd->pubscreen_name));
        }

        if (!MOS_GetDisplayMode(default_modeid, &mode)) {
            SDL_free(dd);
            UnlockPubScreen(NULL, data->PublicScreen);
            data->PublicScreen = NULL;
            D("MOS_GetDisplayMode failed (display0)");
            return SDL_SetError("Couldn't get display mode");
        }

        SDL_zero(display);
		MOS_SetDisplayName(&display,dd->monitor);
        display.desktop_mode = mode;
        display.internal = dd;

        SDL_AddVideoDisplay(&display, false);
    }

    data->ScreenNotifyHandle = AddWorkbenchClient(&data->ScreenNotifyPort, -20);

    Object **monitors = GetMonitorList(NULL);
    if (monitors) {
        for (int i = 0; monitors[i]; i++) {
            APTR m = monitors[i];
            if (m == default_mon) {
                continue;
            }

            const PubScreenInfo *ps = MOS_FindPubScreenForMonitor(m, pubs, pubCount);
            const char *pubname = (ps && ps->name[0]) ? ps->name : NULL;

            SDL_DisplayMode mode;
            bool have_mode = false;

            if (ps && ps->modeid != INVALID_ID && MOS_GetDisplayMode(ps->modeid, &mode)) {
                have_mode = true;
            }

            if (!have_mode) {
                Object **modes = GetMonitorModesList(m, NULL);
                if (modes) {
                    for (int mi = 0; modes[mi]; mi++) {
                        Object *bmode = modes[mi];
                        ULONG depth = 0;
                        ULONG newmodeid = INVALID_ID;

                        GetAttr(MA_Mode_Depth, bmode, &depth);
                        GetAttr(MA_Mode_ModeID, bmode, &newmodeid);

                        if (depth == 32 && newmodeid != INVALID_ID) {
                            if (MOS_GetDisplayMode(newmodeid, &mode)) {
                                have_mode = true;
                            }
                            break;
                        }
                    }
                    FreeMonitorModesList(modes);
                }
            }

            if (!have_mode) {
                continue;
            }

            SDL_DisplayData *dd = (SDL_DisplayData *) SDL_calloc(1, sizeof(*dd));
            if (!dd) {
                continue;
            }

            dd->monitor = m;
            dd->screen = NULL;

            if (pubname) {
                SDL_strlcpy(dd->pubscreen_name, pubname, sizeof(dd->pubscreen_name));
            }

            SDL_VideoDisplay display;
            SDL_zero(display);
			MOS_SetDisplayName(&display,m);
            display.desktop_mode = mode;
            display.internal = dd;

            SDL_AddVideoDisplay(&display, false);
        }

        FreeMonitorList(monitors);
    }

    return true;
}

static SDL_VideoDisplay *
MOS_FindDisplayByMonitor(SDL_VideoDevice *_this, APTR monitor)
{
    for (int i = 0; i < _this->num_displays; i++) {
        SDL_VideoDisplay *d = _this->displays[i];
        if (!d) continue;
        SDL_DisplayData *dd = (SDL_DisplayData *) d->internal;
        if (dd && dd->monitor == monitor) {
            return d;
        }
    }
    return NULL;
}

static void
MOS_FreeDesktopModeInternal(SDL_VideoDisplay *d)
{
    /* MOS_GetDisplayMode() alloue mode.internal => à libérer quand on remplace/supprime */
    if (d->desktop_mode.internal) {
        SDL_free(d->desktop_mode.internal);
        d->desktop_mode.internal = NULL;
    }
}

void
MOS_RefreshDisplays(SDL_VideoDevice *_this)
{
	D("");
    SDL_VideoData *vd = (SDL_VideoData *)_this->internal;

    if (!vd->displays_dirty) {
        return;
    }
    vd->displays_dirty = false;

    PubScreenInfo pubs[32];
    const int pubCount = MOS_CollectPublicScreens(pubs, SDL_arraysize(pubs));

    /* 1) Mark all unseen */
    for (int i = 0; i < _this->num_displays; i++) {
        SDL_VideoDisplay *d = _this->displays[i];
        if (!d) continue;
        SDL_DisplayData *dd = (SDL_DisplayData *)d->internal;
        if (dd) dd->seen = false;
    }

    /* 2) Enumerate monitors and update/add */
    Object **monitors = GetMonitorList(NULL);
    if (!monitors) {
        return;
    }

    for (int i = 0; monitors[i]; i++) {
        APTR m = monitors[i];

        /* Desktop mode: on essaye via PubScreenList (rapide) */
        SDL_DisplayMode mode;
        bool have_mode = false;

		const PubScreenInfo *ps = MOS_FindPubScreenForMonitor(m, pubs, pubCount);
		const char *pubname = (ps && ps->name[0]) ? ps->name : NULL;
		ULONG modeid = ps ? ps->modeid : INVALID_ID;
		
        if (modeid != INVALID_ID && MOS_GetDisplayMode(modeid, &mode)) {
            have_mode = true;
        }

        if (!have_mode) {
            continue;
        }

        SDL_VideoDisplay *d = MOS_FindDisplayByMonitor(_this, m);
        if (d) {
            SDL_DisplayData *dd = (SDL_DisplayData *)d->internal;
            dd->seen = true;

            if (pubname) {
                SDL_strlcpy(dd->pubscreen_name, pubname, sizeof(dd->pubscreen_name));
            } else {
                dd->pubscreen_name[0] = 0;
            }

            /* Update display name (monitorname) */
			MOS_SetDisplayName(d,m);

            /* Remplace desktop_mode sans fuite */
            MOS_FreeDesktopModeInternal(d);
			
            d->desktop_mode = mode;

        } else {
            /* Nouveau monitor => nouveau display SDL */
            SDL_DisplayData *dd = (SDL_DisplayData *)SDL_calloc(1, sizeof(*dd));
            if (!dd) {
                if (mode.internal) {
                    SDL_free(mode.internal);
                    mode.internal = NULL;
                }
                continue;
            }

            dd->monitor = m;
            dd->screen  = NULL;
            dd->seen    = true;

            if (pubname) {
                SDL_strlcpy(dd->pubscreen_name, pubname, sizeof(dd->pubscreen_name));
            }

            SDL_VideoDisplay nd;
            SDL_zero(nd);
            MOS_SetDisplayName(&nd, m);
            nd.desktop_mode = mode;   /* contient internal alloué */
            nd.internal = dd;

            SDL_AddVideoDisplay(&nd, true);
        }
    }

    FreeMonitorList(monitors);

    /* 3) Remove displays non vus */
    for (int i = _this->num_displays - 1; i >= 0; i--) {
        SDL_VideoDisplay *d = _this->displays[i];
        if (!d) continue;

        SDL_DisplayData *dd = (SDL_DisplayData *)d->internal;
        if (!dd) continue;
        if (!dd->seen) {
            SDL_DelVideoDisplay(d->id, true);
        }
    }
}

bool
MOS_SetDisplayMode(SDL_VideoDevice *_this, SDL_VideoDisplay *display, SDL_DisplayMode *mode)
{
	
	SDL_VideoData *driverdata = (SDL_VideoData *) _this->internal;
	SDL_DisplayData *displaydata = (SDL_DisplayData *) display->internal;
	
	if (display->fullscreen_active == 0) {
		D("Not a fullscreen exclusive");
		return true;
	}
	
	D("display %p, screen %p", display, displaydata->screen);
	if (driverdata->CustomScreen) {
		D("screen exist... close all");
		MOS_CloseWindows(_this);
		MOS_CloseScreen(_this);
	}
	
	ULONG openError = 0;
	int bpp = SDL_BITSPERPIXEL(mode->format);
			
    SDL_DisplayModeData *data = (SDL_DisplayModeData *) mode->internal;				

    displaydata->screen = OpenScreenTags(NULL,
        SA_Width,      (IPTR)mode->w,
        SA_Height,     (IPTR)mode->h,
        SA_Depth,      (IPTR)bpp,
        SA_DisplayID,  (IPTR)data->modeid,
        SA_MonitorName,(IPTR)display->name,
        SA_Quiet,      (IPTR)TRUE,
        SA_ShowTitle,  (IPTR)FALSE,
        SA_Title,      (IPTR)FilePart(driverdata->FullAppName),
		SA_AdaptSize,  TRUE,
        SA_ErrorCode,  (IPTR)&openError,
        TAG_DONE);
		
	D("Opened screen '%s' id %lu: %d*%d*%d (address %p) on monitor=%s", FilePart(driverdata->FullAppName), 
		data->modeid, mode->w, mode->h, bpp, displaydata->screen, display->name);
			
    if (!displaydata->screen) {
        switch (openError) {
            case OSERR_NOMONITOR:
                SDL_SetError("Monitor for display mode not available");
                break;
            case OSERR_NOCHIPS:
                SDL_SetError("Newer custom chips required");
                break;
            case OSERR_NOMEM:
            case OSERR_NOCHIPMEM:
                SDL_OutOfMemory();
                break;
            case OSERR_PUBNOTUNIQUE:
                SDL_SetError("Public screen name not unique");
                break;
            case OSERR_UNKNOWNMODE:
            case OSERR_TOODEEP:
                SDL_SetError("Unknown display mode");
                break;
            case OSERR_ATTACHFAIL:
                SDL_SetError("Attachment failed");
                break;
            default:
                SDL_SetError("OpenScreen failed");
                break;
        }
        return false;
    } else {
		driverdata->CustomScreen = displaydata->screen;
	}
	
	SetRPAttrs(&displaydata->screen->RastPort, RPTAG_PenMode, FALSE, RPTAG_FgColor, 0xFF000000, TAG_DONE);
	RectFill(&displaydata->screen->RastPort, 0, 0, mode->w - 1, mode->h - 1);
		
	return true;
}

bool
MOS_GetDisplayBounds(SDL_VideoDevice *device, SDL_VideoDisplay * display, SDL_Rect * rect)
{
	SDL_DisplayModeData *data = (SDL_DisplayModeData *) display->current_mode->internal;

    rect->x = data->x;
    rect->y = data->y;
	rect->w = display->current_mode->w;
    rect->h = display->current_mode->h;

    return true;
}
