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

#ifndef MIN
#   define MIN(x,y) ((x)<(y)?(x):(y))
#endif
#ifndef MAX
#   define MAX(x,y) ((x)>(y)?(x):(y))
#endif

/*
 * The window surface stays in memory as ARGB8888 (RECTFMT_ARGB):
 * - WritePixelArray() converts it to whatever the screen uses (15/16/24/32
 *   bits, any byte order), the surface format doesn't depend on the screen;
 * - SDL draws and blends in fast memory instead of video memory, and never
 *   uses a bitmap pointer outside of LockBitMap()/UnLockBitMap().
 */

void
MOS_DestroyWindowFramebuffer(_THIS, SDL_Window *window)
{
    SDL_WindowData *data = (SDL_WindowData *) window->driverdata;

    if (data && data->fb) {
        SDL_free(data->fb);
        data->fb = NULL;
    }
}

int
MOS_CreateWindowFramebuffer(_THIS, SDL_Window *window, Uint32 *format, void **pixels, int *pitch)
{
    SDL_WindowData *data = (SDL_WindowData *) window->driverdata;
    SDL_Framebuffer *fb;
    const int w = MAX(window->w, 1);
    const int h = MAX(window->h, 1);
    const int bpr = (w * 4 + 15) & ~15;

    if (!data) {
        return SDL_SetError("No window driverdata");
    }

    MOS_DestroyWindowFramebuffer(_this, window);

    fb = (SDL_Framebuffer *) SDL_calloc(1, sizeof(*fb) + (size_t) bpr * h);
    if (!fb) {
        return SDL_OutOfMemory();
    }
    fb->w = w;
    fb->h = h;
    fb->pitch = bpr;
    fb->pixfmt = SDL_PIXELFORMAT_ARGB8888;
    data->fb = fb;

    D("[%s] %ldx%ld, %ld bytes per row\n", __FUNCTION__, (long) w, (long) h, (long) bpr);

    *format = fb->pixfmt;
    *pixels = fb->buffer;
    *pitch = bpr;
    return 0;
}

int
MOS_UpdateWindowFramebuffer(_THIS, SDL_Window *window, const SDL_Rect *rects, int numrects)
{
    SDL_WindowData *data = (SDL_WindowData *) window->driverdata;
    SDL_Framebuffer *fb;
    struct Window *win;
    int width, height, i;

    if (!data || !data->win || !data->fb) {
        return 0;
    }

    fb = data->fb;
    win = data->win;
    width = MIN(fb->w, win->Width - win->BorderLeft - win->BorderRight);
    height = MIN(fb->h, win->Height - win->BorderTop - win->BorderBottom);

    for (i = 0; i < numrects; ++i) {
        const SDL_Rect *r = &rects[i];
        const int x = MAX(r->x, 0);
        const int y = MAX(r->y, 0);
        const int w = MIN(r->x + r->w, width) - x;
        const int h = MIN(r->y + r->h, height) - y;

        if (w > 0 && h > 0) {
            WritePixelArray(fb->buffer, x, y, fb->pitch, win->RPort,
                            win->BorderLeft + x, win->BorderTop + y, w, h, RECTFMT_ARGB);
        }
    }

    return 0;
}
