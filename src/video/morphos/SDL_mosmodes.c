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

void MOS_CloseScreen(SDL_VideoDevice *_this)
{
	SDL_VideoData *data = (SDL_VideoData *) _this->internal;
    if (data->CustomScreen && data->CustomScreen != data->PublicScreen) {
        D("Closing screen %p", data->CustomScreen);
        if (!CloseScreen(data->CustomScreen)) {
            D("Screen has open window(s), cannot close");
        } else {
            D("Screen closed successfully");
			data->CustomScreen = NULL;
        }
    }
}

void MOS_CloseDisplay(SDL_VideoDevice *_this)
{
	SDL_VideoData *data = (SDL_VideoData *) _this->internal;
	D("CustomScreen=0x%08lx  PublicScreen=0x%08lx", data->CustomScreen, data->PublicScreen);

	MOS_CloseScreen(_this);
	
	if (data->PublicScreen) {
		if (data->ScreenSaverSuspendCount) {
			size_t i;

			for (i = data->ScreenSaverSuspendCount; i > 0; i--)
				SetAttrs(data->PublicScreen, SA_StopBlanker, FALSE, TAG_DONE);
		}

		UnlockPubScreen(NULL, data->PublicScreen);
		data->PublicScreen = NULL;
		
		if (data->ScreenNotifyHandle) {
			while (!RemWorkbenchClient(data->ScreenNotifyHandle))
				Delay(10);

			data->ScreenNotifyHandle = NULL;
		}
	}
}

static bool MOS_GetDisplayMode(ULONG id, SDL_DisplayMode * mode)
{
    SDL_DisplayModeData *data;
    APTR handle;
    struct DimensionInfo diminfo;
    struct DisplayInfo dispinfo;
	struct MonitorInfo moninfo;

    handle = FindDisplayInfo(id);
    if (!handle) {
        return false;
    }

    if (!GetDisplayInfoData(handle, (UBYTE *)&diminfo, sizeof(diminfo), DTAG_DIMS, 0)) {
        D("Failed to get dim info");
        return false;
    }

    if (!GetDisplayInfoData(handle, (UBYTE *)&dispinfo, sizeof(dispinfo), DTAG_DISP, 0)) {
        D("Failed to get disp info");
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
	float freq = 60.0f;
	freq = (float)(1.0 / ((double)moninfo.TotalRows * (double)moninfo.TotalColorClocks * 280.0e-9) + 0.5);
    mode->refresh_rate = freq;
    mode->format = SDL_PIXELFORMAT_UNKNOWN;
	
	//D("RTG mode %lu: w=%d, h=%d, bits=%d\n", id, mode->w, mode->h, diminfo.MaxDepth);
	
	switch (diminfo.MaxDepth) {
	case 32:
		mode->format = SDL_PIXELFORMAT_ARGB8888;
		break;
	case 24:
		mode->format = SDL_PIXELFORMAT_XRGB8888;
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

	mode->internal = SDL_malloc(4);
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
				// ok limit 32 bits fullscreen mode only.....
				if (displaymode.format == SDL_PIXELFORMAT_ARGB8888/*!= SDL_PIXELFORMAT_UNKNOWN*/) {
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

bool
MOS_InitModes(SDL_VideoDevice *_this)
{
	D("");
	SDL_VideoData *data = (SDL_VideoData *) _this->internal;
	SDL_VideoDisplay display;
	SDL_DisplayMode mode;
	SDL_DisplayData *displaydata;

	STRPTR monitorname = NULL;
	APTR mon;
	
	displaydata = (SDL_DisplayData *) SDL_malloc(sizeof(*displaydata));
	if (displaydata == NULL) {
		return SDL_OutOfMemory();
	}

	SDL_zero(display);

	data->CustomScreen = NULL;
	data->PublicScreen = LockPubScreen(NULL);
	
	// Add Ambient Screen/Monitor
	if (data->PublicScreen) {

		ULONG modeid = (ULONG)getv(data->PublicScreen, SA_DisplayID);	
		mon = (APTR)getv(data->PublicScreen, SA_MonitorObject);
		displaydata->monitor = mon;
		displaydata->screen = NULL;
		
		if (!MOS_GetDisplayMode(modeid, &mode)) {
				D("Failed to get display mode for %lu", modeid); 
				SDL_free(displaydata);
				return SDL_SetError("Couldn't get display mode");
		}
		display.desktop_mode = mode;
		display.internal = displaydata;

		GetAttr(MA_MonitorName, displaydata->monitor, (ULONG*)&monitorname);
		display.name = monitorname;

		SDL_AddVideoDisplay(&display, false);
		D("Added Workbench screen - monitor = %d - refresh=%f", displaydata->monitor, mode.refresh_rate);
		
		data->ScreenNotifyHandle = AddWorkbenchClient(&data->ScreenNotifyPort, -20);
		
	
	} else {
		SDL_free(displaydata);
		return SDL_OutOfMemory();
	}

	SDL_zero(mode);

	Object **monitors;
	if ((monitors = GetMonitorList(NULL))) {
		APTR m;
		int i;

		for (i = 0; (m = monitors[i]); i++) {
			
			if (m != mon) {
				D("Add other monitors = %d", monitors[i]);
				displaydata = (SDL_DisplayData *) SDL_malloc(sizeof(*displaydata));
				if (displaydata) {
					ULONG modeid = INVALID_ID;
					GetAttr(MA_Mode_ModeID, m, (ULONG*)&modeid);
					displaydata->monitor = m;
					displaydata->screen = NULL;
					Object **modes = GetMonitorModesList(m, NULL);
					if (modes)
					{
						int modei = 0;
						while (modes[modei])
						{
							Boopsiobject *bmode = modes[modei++];
							ULONG depth = 8;
							GetAttr(MA_Mode_Depth, bmode, (ULONG*)&depth);
							// find first 32bit mode on second monitor
							if (depth == 32)
							{
								ULONG newmodeid = INVALID_ID;
								GetAttr(MA_Mode_ModeID, bmode, (ULONG*)&newmodeid);
								
								if (!MOS_GetDisplayMode(newmodeid, &mode)) {
									D("Failed to get display mode for %lu - old=%lu", newmodeid, modeid); 
								}
								break;
							}
						}
						FreeMonitorModesList(modes);					
					}
					display.desktop_mode = mode;
					display.internal = displaydata;
					GetAttr(MA_MonitorName, m, (ULONG*)&monitorname);
					display.name = monitorname;
					D("Add video display '%s'", display.name);
					SDL_AddVideoDisplay(&display, false);
				}
			}
		}
		FreeMonitorList(monitors);
	}
	return true;
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
		// close windows....
		MOS_CloseWindows(_this);
		// close screen
		MOS_CloseScreen(_this);
	}
	
	ULONG openError = 0;
	int bpp = SDL_BITSPERPIXEL(mode->format);
	
	displaydata->screen = OpenScreenTags(NULL,
				SA_Width, mode->w,
				SA_Height, mode->h,
				SA_Depth, bpp,
				//SA_DisplayID, data->modeid,
				SA_MonitorName, display->name,
				SA_Quiet, TRUE,
				SA_ShowTitle, FALSE,
				SA_Title, (IPTR)FilePart(driverdata->FullAppName),
				//SA_AdaptSize, TRUE,
				SA_ErrorCode, (ULONG)&openError,
				TAG_DONE);

#ifdef __SDL_DEBUG				
    SDL_DisplayModeData *data = (SDL_DisplayModeData *) mode->internal;				
	D("Opened screen '%s' id %lu: %d*%d*%d (address %p) on monitor=%s", FilePart(driverdata->FullAppName), 
		data->modeid, mode->w, mode->h, bpp, displaydata->screen, display->name);
#endif
		
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
