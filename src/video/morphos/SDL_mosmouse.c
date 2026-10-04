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
#include "../../SDL_internal.h"

#include "SDL_mosmouse.h"
#include "SDL_mosvideo.h"

#include "../../events/SDL_mouse_c.h"
#include "SDL_hints.h"

#include <cybergraphx/cybergraphics.h>
#include <devices/input.h>
#include <intuition/pointerclass.h>
#include <proto/cybergraphics.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/dos.h>
#include <dos/rdargs.h>

MOS_GlobalMouseState globalMouseState;

static SDL_Cursor *
MOS_CreateCursor(SDL_Surface * surface, int hot_x, int hot_y)
{
	SDL_MOSCursor *cursor = SDL_calloc(1, sizeof(*cursor));
	struct BitMap *bmp;

	D("[%s] %ldx%ld, hot spot %ld,%ld\n", __FUNCTION__, (long)surface->w, (long)surface->h, (long)hot_x, (long)hot_y);

	if (!cursor) {
		SDL_OutOfMemory();
		return NULL;
	}

	cursor->Cursor.next = NULL;
	cursor->Cursor.driverdata = &cursor->Pointer;
	cursor->Pointer.offx = hot_x;
	cursor->Pointer.offy = hot_y;

	// The surface is ARGB8888 (SDL_CreateColorCursor converts it)
	bmp = AllocBitMap(surface->w, surface->h, 32, BMF_MINPLANES | BMF_CLEAR | BMF_SPECIALFMT | SHIFT_PIXFMT(PIXFMT_ARGB32), NULL);

	if (bmp != NULL && SDL_LockSurface(surface) == 0) {
		struct RastPort rp;

		InitRastPort(&rp);
		rp.BitMap = bmp;
		WritePixelArray(surface->pixels, 0, 0, surface->pitch, &rp, 0, 0, surface->w, surface->h, RECTFMT_ARGB);
		SDL_UnlockSurface(surface);

		cursor->Pointer.mouseptr = NewObject(NULL, POINTERCLASS,
						POINTERA_BitMap, bmp,
						POINTERA_XOffset, -hot_x,
						POINTERA_YOffset, -hot_y,
						TAG_DONE);
	}

	if (cursor->Pointer.mouseptr == NULL) {
		if (bmp)
			FreeBitMap(bmp);
		SDL_free(cursor);
		SDL_SetError("Couldn't create the pointer");
		return NULL;
	}

	// Kept until the pointer object is disposed, in case it doesn't copy it
	cursor->Pointer.bitmap = bmp;

	return &cursor->Cursor;
}

static SDL_Cursor *
MOS_CreateSystemCursor(SDL_SystemCursor id)
{
	SDL_Cursor *cursor = SDL_malloc(sizeof(*cursor));
	//D("[%s]\n", __FUNCTION__);

	if (cursor) {
		size_t type = POINTERTYPE_NORMAL;
		cursor->next = NULL;
		switch (id) {
			default:
			case SDL_SYSTEM_CURSOR_ARROW:     type = POINTERTYPE_NORMAL; break;
			case SDL_SYSTEM_CURSOR_IBEAM:     type = POINTERTYPE_SELECTTEXT; break;
			case SDL_SYSTEM_CURSOR_WAIT:      type = POINTERTYPE_BUSY; break;
			case SDL_SYSTEM_CURSOR_CROSSHAIR: type = POINTERTYPE_AIMING; break;
			case SDL_SYSTEM_CURSOR_WAITARROW: type = POINTERTYPE_WORKING; break;
			case SDL_SYSTEM_CURSOR_SIZENWSE:  type = POINTERTYPE_DIAGONALRESIZE2; break;
			case SDL_SYSTEM_CURSOR_SIZENESW:  type = POINTERTYPE_DIAGONALRESIZE1; break;
			case SDL_SYSTEM_CURSOR_SIZEWE:    type = POINTERTYPE_HORIZONTALRESIZE; break;
			case SDL_SYSTEM_CURSOR_SIZENS:    type = POINTERTYPE_VERTICALRESIZE; break;
			case SDL_SYSTEM_CURSOR_SIZEALL:   type = POINTERTYPE_MOVE; break;
			case SDL_SYSTEM_CURSOR_NO:        type = POINTERTYPE_NOTAVAILABLE; break;
			case SDL_SYSTEM_CURSOR_HAND:      type = POINTERTYPE_SELECTLINK; break;
		}

		cursor->driverdata = (APTR)type;
	} else {
		SDL_OutOfMemory();
	}

	return cursor;
}

static void
MOS_FreeCursor(SDL_Cursor *cursor)
{
	D("[%s] 0x%08lx\n", __FUNCTION__, cursor);

	if (!IS_SYSTEM_CURSOR(cursor)) {
		SDL_MOSCursor *ac = (SDL_MOSCursor *)cursor;

		if (ac->Pointer.mouseptr)
			DisposeObject(ac->Pointer.mouseptr);
		if (ac->Pointer.bitmap)
			FreeBitMap(ac->Pointer.bitmap);
	}

	SDL_free(cursor);
}

// Sets the SDL cursor (NULL: hidden) as pointer of an Intuition window
void
MOS_ApplyPointer(SDL_VideoData *vd, struct Window *win)
{
	SDL_Cursor *cursor = vd->CurrentPointer;

	if (!win)
		return;

	if (IS_SYSTEM_CURSOR(cursor)) {
		size_t pointertags[] = { WA_PointerType, cursor ? (size_t)cursor->driverdata : POINTERTYPE_INVISIBLE, TAG_DONE };
		SetAttrsA(win, (struct TagItem *)&pointertags);
	} else {
		SDL_MOSCursor *ac = (SDL_MOSCursor *)cursor;
		if (ac->Pointer.mouseptr)
			SetWindowPointer(win, WA_Pointer, (size_t)ac->Pointer.mouseptr, TAG_DONE);
	}
}

static BOOL
MOS_IsPointerInWindow(struct Window *w)
{
	struct Screen *s = w->WScreen;

	if (!s)
		return TRUE;

	return s->MouseX >= w->LeftEdge + w->BorderLeft && s->MouseY >= w->TopEdge + w->BorderTop &&
	       s->MouseX < w->LeftEdge + w->Width - w->BorderRight && s->MouseY < w->TopEdge + w->Height - w->BorderBottom;
}

/*
 * Intuition shows the pointer of the active window on the whole screen, so the
 * SDL cursor (often hidden) is only set while the mouse is over the window and
 * the system pointer comes back outside of it. Except while the window owns the
 * events (WM_ObtainEvents) or the mouse is relative: the SDL cursor everywhere.
 * Only changed when the state changes, or when forced.
 */
void
MOS_UpdateWindowPointer(SDL_VideoData *vd, SDL_WindowData *wd, BOOL force)
{
	struct Window *w = wd->win;
	BOOL inside;

	if (!w)
		return;

	inside = wd->grab_owned || SDL_GetRelativeMouseMode() || MOS_IsPointerInWindow(w);
	if (!force && wd->pointer_inside == (inside ? 1 : 0))
		return;

	D("[%s] window 0x%08lx: %s pointer\n", __FUNCTION__, w, inside ? "SDL" : "system");
	wd->pointer_inside = inside ? 1 : 0;

	if (inside) {
		w->Flags |= WFLG_RMBTRAP;
		MOS_ApplyPointer(vd, w);
	} else {
		size_t pointertags[] = { WA_PointerType, POINTERTYPE_NORMAL, TAG_DONE };

		w->Flags &= ~WFLG_RMBTRAP;
		ClearPointer(w);
		SetAttrsA(w, (struct TagItem *)&pointertags);
	}
}

static int
MOS_ShowCursor(SDL_Cursor * cursor)
{
	SDL_VideoDevice *video = SDL_GetVideoDevice();
	SDL_VideoData *data = (SDL_VideoData *)video->driverdata;
	SDL_WindowData *wd;

	D("[%s] %s cursor 0x%08lx\n", __FUNCTION__,
	  cursor == NULL ? "hidden" : IS_SYSTEM_CURSOR(cursor) ? "system" : "custom", cursor);

	data->CurrentPointer = cursor;

	// Set again even if it didn't change: windows are reopened (fullscreen
	// switch...) and get the system pointer while the mouse is outside of them
	ForeachNode(&data->windowlist, wd) {
		if (wd->win && wd->pointer_inside != 0)
			MOS_ApplyPointer(data, wd->win);
	}

	return 0;
}

static void
MOS_WarpMouse(SDL_Window * window, int x, int y)
{
	SDL_WindowData *data = (SDL_WindowData *)window->driverdata;
	struct Window *win;

	BOOL warpHostPointer;
	warpHostPointer = !SDL_GetRelativeMouseMode() && (window == SDL_GetMouseFocus());

	if (warpHostPointer) {

		if ((win = data->win)) {
			struct MsgPort *port;
			struct IOStdReq *req;

			port = CreateMsgPort();
			if (port) {
				req = CreateIORequest(port, sizeof(*req));
				if (req) {
					if (OpenDevice("input.device", 0, (struct IORequest *)req, 0) == 0) {
						struct InputEvent ie = { 0 };
						struct IEPointerPixel newpos = { 0 };

						newpos.iepp_Screen = win->WScreen;
						newpos.iepp_Position.X = x + win->BorderLeft + win->LeftEdge;
						newpos.iepp_Position.Y = y + win->BorderTop + win->TopEdge;

						ie.ie_EventAddress = &newpos;
						ie.ie_NextEvent = NULL;
						ie.ie_Class = IECLASS_NEWPOINTERPOS;
						ie.ie_SubClass = IESUBCLASS_PIXEL;
						ie.ie_Code = IECODE_NOBUTTON;
						ie.ie_Qualifier = 0;

						req->io_Data = &ie;
						req->io_Length = sizeof(ie);
						req->io_Command = IND_WRITEEVENT;

						DoIO((struct IORequest *)req);
						CloseDevice((struct IORequest *)req);
					}
				}

				DeleteMsgPort(port);
			}
		}
	} else {
		SDL_SendMouseMotion(window,0, SDL_GetRelativeMouseMode(), x, y);
	}
}

static int
MOS_SetRelativeMouseMode(SDL_bool enabled)
{
	D("[%s] %s\n", __FUNCTION__, enabled ? "on" : "off");

	SDL_VideoDevice *video = SDL_GetVideoDevice();
	SDL_VideoData *data = (SDL_VideoData *)video->driverdata;
	SDL_WindowData *wd;
	size_t or_mask, and_mask;

	if (enabled) {
		or_mask = IDCMP_DELTAMOVE;
		and_mask = ~0;
	} else {
		or_mask = 0;
		and_mask = ~IDCMP_DELTAMOVE;
	}

	// Windows opened later get IDCMP_DELTAMOVE from MOS_ShowWindow_Internal()
	ForeachNode(&data->windowlist, wd) {
		if (wd->win) {
			ModifyIDCMP(wd->win, (wd->win->IDCMPFlags | or_mask) & and_mask);
			// The first delta after the switch is not one
			wd->first_deltamove = TRUE;
		}
	}

	return 0;
}

static Uint32
MOS_GetDoubleClickTimeInMillis(_THIS)
{
    Uint32 interval = 500;

    struct RDArgs rda;
    SDL_memset(&rda, 0, sizeof(rda));
    rda.RDA_Source.CS_Buffer = (STRPTR)SDL_LoadFile("ENV:sys/mouse.conf", (size_t *)&rda.RDA_Source.CS_Length);
    if (rda.RDA_Source.CS_Buffer) {
        LONG *array[4] = {0};
        if (ReadArgs("Pointer/K,RMBEmulationQualifier/K,DoubleClickS/N/K,DoubleClickM/N/K,/F", (LONG *)array, &rda)) {
			if (array[2] != 0L && array[3] != 0L)
            	interval = *array[2] * 1000 + *array[3] / 1000;

	    	FreeArgs(&rda);
        }
        SDL_free(rda.RDA_Source.CS_Buffer);
    }

    return interval;
}

static Uint32
MOS_GetGlobalMouseState(int *x, int *y)
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

void
MOS_InitMouse(_THIS)
{
	SDL_Mouse *mouse = SDL_GetMouse();
	char buffer[16];

	mouse->CreateCursor = MOS_CreateCursor;
	mouse->CreateSystemCursor = MOS_CreateSystemCursor;
	mouse->ShowCursor = MOS_ShowCursor;
	mouse->FreeCursor = MOS_FreeCursor;
	mouse->WarpMouse = MOS_WarpMouse;
	mouse->SetRelativeMouseMode = MOS_SetRelativeMouseMode;
    mouse->GetGlobalMouseState = MOS_GetGlobalMouseState;

	SDL_SetDefaultCursor(MOS_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
	SDL_SetHint(SDL_HINT_MOUSE_DOUBLE_CLICK_TIME,  SDL_uitoa(MOS_GetDoubleClickTimeInMillis(_this), buffer, 10));
}

void
MOS_QuitMouse(_THIS)
{
	SDL_Mouse *mouse = SDL_GetMouse();
	//D("[%s]\n", __FUNCTION__);

	if ( mouse->def_cursor ) {
		SDL_free(mouse->def_cursor);
		mouse->def_cursor = NULL;
		mouse->cur_cursor = NULL;
	}
}
