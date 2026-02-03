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
#include "../../SDL_internal.h"
#include "SDL_mosvideo.h"
#include "SDL_mosmodes.h"

#include <cybergraphx/cybergraphics.h>
#include <intuition/intuition.h>
#include <proto/cybergraphics.h>
#include <graphics/rpattr.h>
#include <proto/graphics.h>

#ifndef MIN
#   define MIN(x,y) ((x)<(y)?(x):(y))
#endif

static inline void 
MOS_FreeBitmap(SDL_WindowData *data)
{
    if (data && data->bitmap) {
        FreeBitMap(data->bitmap);
        data->bitmap = NULL;
    }
}

static inline struct BitMap *
MOS_GetFriendBitMap(_THIS, SDL_WindowData *data)
{

    if (data && data->win && data->win->RPort) {
        return data->win->RPort->BitMap;
    }
	
	// No window -> fallback, use WBScreen friend bitmap
	MOS_GetScreen(_this, 0, SDL_TRUE);
	
    if (data && data->videodata && data->videodata->WScreen) {
        return data->videodata->WScreen->RastPort.BitMap;
    }
	
    return NULL;
}

void 
MOS_DestroyWindowFramebuffer(_THIS, SDL_Window *window)
{
    SDL_WindowData *data = (SDL_WindowData *) window->driverdata;
    if (!data) {
        return;
    }
    MOS_FreeBitmap(data);
}

int 
MOS_CreateWindowFramebuffer(_THIS, SDL_Window *window, Uint32 *format, void **pixels, int *pitch)
{
    SDL_WindowData *data = (SDL_WindowData *) window->driverdata;
    if (!data) {
        return SDL_SetError("No window driverdata");
    }

    MOS_FreeBitmap(data);

    struct BitMap *friend_bitmap = MOS_GetFriendBitMap(_this, data);
    if (!friend_bitmap) {
        return SDL_SetError("No friend bitmap (no window and no WScreen)");
    }

    APTR lock;
    APTR base_address = NULL;
    Uint32 bytes_per_row = 0;

    const Uint32 depth = GetBitMapAttr(friend_bitmap, BMA_DEPTH);

    *format = SDL_PIXELFORMAT_BGRA8888;

    data->bitmap = AllocBitMap(window->w, window->h, depth,
                               BMF_MINPLANES | BMF_CLEAR,
                               friend_bitmap);
    if (!data->bitmap) {
        return SDL_SetError("AllocBitMap failed");
    }

    lock = LockBitMapTags(data->bitmap,
                          LBMI_BASEADDRESS, &base_address,
                          LBMI_BYTESPERROW, &bytes_per_row,
                          TAG_DONE);
    if (!lock) {
        MOS_FreeBitmap(data);
        return SDL_SetError("LockBitMapTags failed");
    }

    *pixels = base_address;
    *pitch  = (int)bytes_per_row;

    UnLockBitMap(lock);

    return 0;
}

int 
MOS_UpdateWindowFramebuffer(_THIS, SDL_Window *window, const SDL_Rect *rects, int numrects)
{
    SDL_WindowData *data = (SDL_WindowData *) window->driverdata;
    if (!data || !data->win || !data->bitmap) {
        return 0;
    }

    struct Window *win = data->win;
    struct RastPort *rp = win->RPort;

    const int left   = win->BorderLeft;
    const int top    = win->BorderTop;
    const int width  = win->Width  - win->BorderLeft - win->BorderRight;
    const int height = win->Height - win->BorderTop  - win->BorderBottom;

    for (int i = 0; i < numrects; ++i) {
        const SDL_Rect *r = &rects[i];
        const int w = MIN(r->w, width);
        const int h = MIN(r->h, height);
        BltBitMapRastPort(data->bitmap, r->x, r->y, rp,
                          r->x + left, r->y + top, w, h, 0xc0);
    }

    return 0;
}
