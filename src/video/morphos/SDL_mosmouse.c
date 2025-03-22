/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2024 Sam Lantinga <slouken@libsdl.org>

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

#include "SDL_mosmouse.h"
#include "SDL_mosvideo.h"
#include "SDL_moswindow.h"

#include "../../events/SDL_mouse_c.h"

#include <cybergraphx/cybergraphics.h>
#include <devices/input.h>
#include <intuition/pointerclass.h>
#include <proto/cybergraphics.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/dos.h>

MOS_GlobalMouseState globalMouseState;

static Uint32
MOS_GetDoubleClickTimeInMillis(SDL_VideoDevice *_this)
{
	//D(kprintf("[%s]\n", __FUNCTION__));
    Uint32 interval = 500;

    struct RDArgs rda;
    SDL_memset(&rda, 0, sizeof(rda));
    rda.RDA_Source.CS_Buffer = (unsigned char*)SDL_LoadFile("ENV:sys/mouse.conf", (size_t *)&rda.RDA_Source.CS_Length);
    if (rda.RDA_Source.CS_Buffer != NULL) {
        LONG *array[4] = {0};
        if (ReadArgs("Pointer/K,RMBEmulationQualifier/K,DoubleClickS/N/K,DoubleClickM/N/K,/F", (LONG *)array, &rda)) {
			if (array[2] != 0L && array[3] != 0L)
            	interval = *array[2] * 1000 + *array[3] / 1000;

	    	FreeArgs(&rda);
        }
        SDL_free(rda.RDA_Source.CS_Buffer);
    }
	D(kprintf("[%s] interval = %d\n", __FUNCTION__, interval));
    return interval;
}

static SDL_Cursor*
MOS_CreateCursorInternal()
{
    SDL_Cursor *cursor = SDL_calloc(1, sizeof(SDL_Cursor));

    if (cursor) {
        SDL_CursorData *data = SDL_calloc(1, sizeof(SDL_CursorData));

        if (data) {
            cursor->internal = data;
        } else {
			D(kprintf("[%s] error SDL_CursorData\n", __FUNCTION__));
		}
    } else {
		D(kprintf("[%s] error cursor\n", __FUNCTION__));
	}

    return cursor;
}

static SDL_Cursor *
MOS_CreateCursor(SDL_Surface * surface, int hot_x, int hot_y)
{
	D(kprintf("[%s]\n", __FUNCTION__));
	SDL_Cursor *cursor = MOS_CreateCursorInternal();

	if (cursor) {
		SDL_CursorData *data = cursor->internal;
		
		struct BitMap *bmp;

		data->type = NULL;
		data->offx = hot_x;
		data->offy = hot_y;

		bmp = AllocBitMap(surface->w, surface->h, 32, BMF_MINPLANES | BMF_CLEAR | BMF_SPECIALFMT | SHIFT_PIXFMT(PIXFMT_ARGB32), NULL);

		if (bmp != NULL) {
			struct RastPort rp;

			InitRastPort(&rp);
			rp.BitMap = bmp;

			if (SDL_MUSTLOCK(surface)) {
				SDL_LockSurface(surface);
			}
			
			WritePixelArray(surface->pixels, 0, 0, surface->pitch, &rp, 0, 0, surface->w, surface->h, RECTFMT_ARGB);
			Object *mouseptr = NewObject(NULL, POINTERCLASS,
							POINTERA_BitMap, bmp,
							POINTERA_XOffset, -hot_x,
							POINTERA_YOffset, -hot_y,
							POINTERA_WordWidth, 1,
							POINTERA_XResolution, POINTERXRESN_SCREENRES,
                            POINTERA_YResolution, POINTERYRESN_SCREENRESASPECT,
							TAG_DONE);

			data->mouseptr = mouseptr;
						
		    if (SDL_MUSTLOCK(surface)) {
				SDL_UnlockSurface(surface);
			}

			FreeBitMap(bmp);
		} else {
			D(kprintf("[%s] AllocBitMap failed! \n", __FUNCTION__));
		}
	} else {
		D(kprintf("[%s] cursor NULL \n", __FUNCTION__));
	}

	return cursor;
}

static SDL_Cursor *
MOS_CreateSystemCursor(SDL_SystemCursor id)
{
	SDL_Cursor *cursor = MOS_CreateCursorInternal();

	if (cursor && cursor->internal) {
		size_t type = POINTERTYPE_NORMAL;
		switch (id) {	
			case SDL_SYSTEM_CURSOR_TEXT:
				type = POINTERTYPE_SELECTTEXT; 
				break;
			case SDL_SYSTEM_CURSOR_WAIT:
				type = POINTERTYPE_BUSY; 
				break;
			case SDL_SYSTEM_CURSOR_CROSSHAIR:
				type = POINTERTYPE_AIMING;
				break;
			case SDL_SYSTEM_CURSOR_PROGRESS:
				type = POINTERTYPE_WORKING;
				break;
			case SDL_SYSTEM_CURSOR_NW_RESIZE:
			case SDL_SYSTEM_CURSOR_SE_RESIZE:
			case SDL_SYSTEM_CURSOR_NWSE_RESIZE:
				type = POINTERTYPE_DIAGONALRESIZE2;
				break;
			case SDL_SYSTEM_CURSOR_NE_RESIZE:
			case SDL_SYSTEM_CURSOR_SW_RESIZE:
			case SDL_SYSTEM_CURSOR_NESW_RESIZE:
				type = POINTERTYPE_DIAGONALRESIZE1; 
				break;
			case SDL_SYSTEM_CURSOR_E_RESIZE:
			case SDL_SYSTEM_CURSOR_W_RESIZE:
			case SDL_SYSTEM_CURSOR_EW_RESIZE:
				type = POINTERTYPE_HORIZONTALRESIZE;
				break;
			case SDL_SYSTEM_CURSOR_S_RESIZE:
			case SDL_SYSTEM_CURSOR_N_RESIZE:
			case SDL_SYSTEM_CURSOR_NS_RESIZE:
				type = POINTERTYPE_VERTICALRESIZE;
				break;
			case SDL_SYSTEM_CURSOR_MOVE:
				type = POINTERTYPE_MOVE;
				break;
			case SDL_SYSTEM_CURSOR_NOT_ALLOWED:
				type = POINTERTYPE_NOTAVAILABLE;
				break;
			case SDL_SYSTEM_CURSOR_POINTER:
				type = POINTERTYPE_SELECTLINK;
				break;
			case SDL_SYSTEM_CURSOR_DEFAULT:
			default:
			     type = POINTERTYPE_NORMAL; 
				 break;
		}
		SDL_CursorData *data = cursor->internal;
		data->type = (APTR)type;
		data->mouseptr = NULL;
	}

	return cursor;
}

static bool
MOS_ShowCursor(SDL_Cursor * cursor)
{
	SDL_VideoDevice *video = SDL_GetVideoDevice();
	SDL_VideoData *vdata = (SDL_VideoData *)video->internal;
	if (cursor) {
		SDL_CursorData *data = cursor->internal;
		if (data->mouseptr != NULL) {
			// Custom cursor
			SDL_WindowData *wd = NULL;
			ForeachNode(&vdata->windowlist, wd) {
				if (wd->win)
					SetWindowPointer(wd->win,WA_Pointer,(size_t)data->mouseptr, TAG_DONE);
			}
		} else if (data->type != NULL) {		
			// System Cursor
			if (vdata->CurrentPointer != cursor) {
				SDL_WindowData *wd = NULL;
				size_t pointertags[] = { WA_PointerType, (size_t)data->type, TAG_DONE };

				ForeachNode(&vdata->windowlist, wd)
				{
					if (wd->win) 
						SetAttrsA(wd->win, (struct TagItem *)&pointertags);

				}
			}
		}
	} else {
		SDL_WindowData *wd = NULL;
		size_t pointertags[] = { WA_PointerType, POINTERTYPE_INVISIBLE, TAG_DONE };
		ForeachNode(&vdata->windowlist, wd)
		{
			if (wd->win) 
				SetAttrsA(wd->win, (struct TagItem *)&pointertags);
		}
	}
		
	vdata->CurrentPointer = cursor;
	return true;
}

static void
MOS_FreeCursor(SDL_Cursor *cursor)
{
	SDL_CursorData *data = cursor->internal;

	if (data) {
        if (data->mouseptr) {
			DisposeObject(data->mouseptr);
			data->mouseptr = NULL;
		}
		
		SDL_free(data);
		cursor->internal = NULL;
	}
}

static bool
MOS_WarpMouseInternal(struct Screen *screen, float x, float y)
{
	SDL_VideoDevice *device = SDL_GetVideoDevice();
    SDL_VideoData *vd = device->internal;
    bool result = false;

	if (vd->inputReq != NULL) {
		//D(kprintf("[%s] %f, %f\n", __FUNCTION__, x, y));
		struct InputEvent ie = { 0 };
		struct IEPointerPixel newpos = { 0 };

		newpos.iepp_Screen = screen ? screen : vd->CustomScreen;
		newpos.iepp_Position.X = x;
		newpos.iepp_Position.Y = y;

		ie.ie_EventAddress = &newpos;
		ie.ie_NextEvent = NULL;
		ie.ie_Class = IECLASS_NEWPOINTERPOS;
		ie.ie_SubClass = IESUBCLASS_PIXEL;
		ie.ie_Code = IECODE_NOBUTTON;
		ie.ie_Qualifier = 0;

		vd->inputReq->io_Data = &ie;
		vd->inputReq->io_Length = sizeof(ie);
		vd->inputReq->io_Command = IND_WRITEEVENT;

		DoIO((struct IORequest *)vd->inputReq);
		
		result = true;
	}

	return result;		
}

static bool
MOS_WarpMouseGlobal(float x, float y)
{
    D(kprintf("[%s] Warping mouse to %f, %f\n", __FUNCTION__, x, y));

    return MOS_WarpMouseInternal(NULL, x, y);
}

static bool
MOS_WarpMouse(SDL_Window * window, float x, float y)
{
	SDL_WindowData *data = (SDL_WindowData *)window->internal;
	struct Window *syswin = data->win;
	
	D(kprintf("[%s] Warping mouse to %f, %f\n", __FUNCTION__, x, y));
	
	bool relativeMouseMode = SDL_GetRelativeMouseMode();
	
	BOOL warpHostPointer = !relativeMouseMode && (window == SDL_GetMouseFocus());

	if (warpHostPointer) {
		MOS_WarpMouseInternal(syswin->WScreen,
			x + syswin->BorderLeft + syswin->LeftEdge,
			y + syswin->BorderTop + syswin->TopEdge);	
	} else {
	//	D(kprintf("[%s] !warpHostPointer\n", __FUNCTION__));
		SDL_SendMouseMotion(0, window, 0, relativeMouseMode, x, y);
	}
	
	return true;
}

static bool
MOS_SetRelativeMouseMode(bool enabled)
{
	D(kprintf("[%s] enabled=%d\n", __FUNCTION__, enabled));
	
	SDL_VideoDevice *video = SDL_GetVideoDevice();
	SDL_VideoData *data = (SDL_VideoData *)video->internal;
	SDL_WindowData *wd;
	size_t or_mask, and_mask;

	if (enabled) {
		or_mask = IDCMP_DELTAMOVE;
		and_mask = ~0;
	} else {
		or_mask = 0;
		and_mask = ~IDCMP_DELTAMOVE;
	}

	ForeachNode(&data->windowlist, wd)
		if (wd->win)
			ModifyIDCMP(wd->win, (wd->win->IDCMPFlags | or_mask) & and_mask);

	return true;
}

static Uint32
MOS_GetGlobalMouseState(float * x, float * y)
{
    Uint32 buttons = 0;

    if (x) {
        *x = globalMouseState.x;
    }

    if (y) {
        *y = globalMouseState.y;
    }

    if (globalMouseState.buttonPressed[SDL_BUTTON_LEFT]) {
        buttons |= SDL_BUTTON_LMASK;
    }

    if (globalMouseState.buttonPressed[SDL_BUTTON_MIDDLE]) {
        buttons |= SDL_BUTTON_MMASK;
    }

    if (globalMouseState.buttonPressed[SDL_BUTTON_RIGHT]) {
        buttons |= SDL_BUTTON_RMASK;
    }
    
    if (globalMouseState.buttonPressed[SDL_BUTTON_X1]) {
        buttons |= SDL_BUTTON_X1MASK;
    }

    return buttons;
}

bool
MOS_InitMouse(SDL_VideoDevice *_this)
{
	D(kprintf("[%s]\n", __FUNCTION__));
	SDL_Mouse *mouse = SDL_GetMouse();
	char buffer[16];

	mouse->CreateCursor = MOS_CreateCursor;
	mouse->CreateSystemCursor = MOS_CreateSystemCursor;
	mouse->ShowCursor = MOS_ShowCursor;
	mouse->FreeCursor = MOS_FreeCursor;
	mouse->WarpMouse = MOS_WarpMouse;
	mouse->WarpMouseGlobal = MOS_WarpMouseGlobal;
	mouse->SetRelativeMouseMode = MOS_SetRelativeMouseMode;
	mouse->GetGlobalMouseState = MOS_GetGlobalMouseState;

	SDL_SetDefaultCursor(MOS_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT));
	
	SDL_SetHint(SDL_HINT_MOUSE_DOUBLE_CLICK_TIME, SDL_uitoa(MOS_GetDoubleClickTimeInMillis(_this), buffer, 10));
	
	return true;
}

void
MOS_QuitMouse(SDL_VideoDevice *_this)
{
	D(kprintf("[%s]\n", __FUNCTION__));
	SDL_Mouse *mouse = SDL_GetMouse();
	
	if ( mouse->def_cursor ) {
		MOS_FreeCursor(mouse->def_cursor);
		mouse->def_cursor = NULL;	
	}
	
	mouse->cur_cursor = NULL;
}
