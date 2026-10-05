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
#include "../../SDL_internal.h"

#if SDL_VIDEO_RENDER_MOS_OVERLAY

/* MorphOS renderer showing its output through a cgxvideo.library overlay
 * (VLayer), selected with SDL_HINT_RENDER_DRIVER = "overlay".
 */

#include "SDL_cpuinfo.h"
#include "SDL_hints.h"
#include "SDL_timer.h"
#include "../SDL_sysrender.h"
#include "../SDL_yuv_sw_c.h"
#include "../software/SDL_blendfillrect.h"
#include "../software/SDL_blendline.h"
#include "../software/SDL_blendpoint.h"
#include "../software/SDL_drawline.h"
#include "../software/SDL_drawpoint.h"
#include "../software/SDL_triangle.h"
#include "../../video/morphos/SDL_mosvideo.h"
#include "SDL_render_overlay_altivec.h"

#include <cybergraphx/cgxvideo.h>
#include <cybergraphx/cybergraphics.h>
#include <intuition/intuition.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/cybergraphics.h>
#include <proto/cgxvideo.h>

#define OVL_SWITCH_FRAMES 8
#define OVL_MAX_COMP_SIZE 4096
#define OVL_RETRY_MS 2000
#define OVL_UPLOAD_CHUNK 16384  /* bytes converted in RAM per copy to the overlay */
#define OVL_MAX_SCALE_TMP (1024 * 1024) /* pixels of the scaling surface kept between copies */
#define OVL_WRITE_TRIALS 16     /* uploads timed to choose how to write an RGB16 overlay */

struct Library *CGXVideoBase = NULL;
static int OVL_cgxvideo_users = 0;

typedef enum
{
    OVL_MODE_COMPOSE,
    OVL_MODE_DIRECT
} OVL_Mode;

/* Texture pixels: drawing always uses surface */
typedef struct
{
    SDL_Surface *surface;          /* YUV textures: RGB copy, converted when needed */
    SDL_SW_YUVTexture *yuv;        /* YUV textures: the pixels */
    Uint32 version;                /* bumped whenever the pixels may change */
    Uint32 surface_version;        /* YUV textures: version converted into surface */
} OVL_TextureData;

/* An opaque texture copy kept out of the composition surface */
typedef struct
{
    SDL_Texture *texture;
    SDL_Rect src;      /* texture pixels */
    SDL_Rect dst;      /* window pixels */
    SDL_Rect comp_dst; /* composition pixels, if it has to be composed after all */
} OVL_DirectCopy;

/* Last overlay creation failure */
typedef struct
{
    struct Window *win;
    ULONG srcfmt;
    int w, h;
    Uint32 ticks;
} OVL_Failure;

typedef struct
{
    SDL_Window *window;

    /* Overlay */
    struct VLayerHandle *vlayer;
    struct Window *vlayer_win;     /* Intuition window the overlay is attached to */
    OVL_Mode mode;
    ULONG srcfmt;                  /* SRCFMT_RGB16, or YCbCr for YUV textures in direct mode */
    int src_w, src_h;
    SDL_bool filter;
    SDL_bool double_buffer;
    LONG indents[4];               /* left, top, right, bottom */
    LONG win_left, win_top;        /* window position when the indents were set */
    SDL_Rect shown;                /* inner window rectangle showing the overlay */
    int switch_count;
    OVL_Failure failed[2];         /* per mode: direct and composed sizes differ */
    SDL_bool in_fallback;          /* last frame drawn into the window, without overlay */
    SDL_bool dims_logged;          /* real overlay size traced (debug build) */
    SDL_bool colorkey;             /* overlay only shown where the window has key_color */
    ULONG key_color;               /* ARGB */
    struct Screen *query_screen;   /* screen VSQ_SupportedFormats was asked for */
    ULONG query_formats;
    SDL_bool no_yuv420;            /* planar YUV overlay failed: packed one instead */
    SDL_bool no_yuv;               /* no YUV overlay at all: YUV textures converted to RGB16 */

    /* Direct mode, frame on screen: not uploaded again while unchanged */
    SDL_Texture *shown_texture;
    SDL_Rect shown_src;
    Uint32 shown_version;
    SDL_bool shown_clear;          /* a clear, of shown_clear_color */
    ULONG shown_clear_color;

    /* Frame of a YUV texture already written to the overlay when updated,
       only to be swapped, see OVL_WriteThroughYUV() */
    SDL_Texture *pending_texture;
    Uint32 pending_version;
    Uint64 pending_ticks[3];       /* debug build: lock, write, unlock */

    /* RGB16 uploads, see OVL_UploadRGB() */
    Uint8 *row_buf;                /* rows converted in RAM before the copy */
    size_t row_buf_size;
    SDL_bool altivec;              /* rows converted straight into the overlay */
    int write_trials;              /* uploads timed since the overlay was made */
    Uint64 write_best[2];          /* fastest: through RAM, in place */
    int write_method;              /* 1 to convert in place */

    /* Debug build: upload times, logged once a second */
    Uint64 upload_lock, upload_start; /* when the lock and the writes started */
    Uint64 upload_ticks[4];        /* lock, write, unlock, swap */
    int upload_frames;
    Uint64 upload_log;

    /* Drawing */
    SDL_Surface *comp;             /* composition surface of the default target, ARGB8888 */
    SDL_Surface *target;           /* surface of the target texture, NULL for the default target */
    OVL_TextureData *target_data;
    SDL_Surface *scale_tmp;        /* see OVL_BlitCopy() */
    SDL_Rect area;                 /* window rectangle the composition surface is shown in */
    float kx, ky;                  /* composition pixels per window pixel */
    SDL_bool comp_sized;           /* comp size fixed until the next present */
    SDL_bool compose_filter;

    /* Default target, frame being drawn */
    SDL_bool composed;             /* comp holds the frame */
    SDL_bool clear_pending;        /* last clear not applied to comp yet */
    Uint32 clear_color;            /* ARGB of the last clear */
    OVL_DirectCopy direct;

    /* Window area around the overlay */
    SDL_bool bars_dirty;
    Uint32 bars_color;
    SDL_bool bars_under;           /* painted for an overlay, not around a fallback frame */

    SDL_bool vsync;
} OVL_RenderData;

/* Maps the current viewport into the surface being drawn */
typedef struct
{
    float ox, oy;                  /* surface position of the viewport origin */
    float kx, ky;                  /* surface pixels per output pixel */
    SDL_Rect clip;
} OVL_Xform;

/* Vertex data, in output pixels relative to the viewport */
typedef struct
{
    SDL_Rect src;
    SDL_FRect dst;
} OVL_CopyData;

typedef struct
{
    SDL_Rect src;
    SDL_FRect dst;                 /* logical units, see scale_x/scale_y */
    double angle;
    SDL_FPoint center;
    SDL_RendererFlip flip;
    float scale_x;
    float scale_y;
} OVL_CopyExData;

typedef struct
{
    const SDL_PixelFormat *format;
    int bpp;
    SDL_bool packed;
    SDL_bool alpha;
    Uint32 rs, gs, bs, as;
    Uint32 rot;
} OVL_Layout;

typedef struct
{
    SDL_FPoint dst;
    SDL_Color color;
} OVL_GeometryFillQueued;

typedef struct
{
    SDL_Point dst;
    SDL_Color color;
} OVL_GeometryFillData;

typedef struct
{
    SDL_Point src;
    SDL_FPoint dst;
    SDL_Color color;
} OVL_GeometryCopyQueued;

typedef struct
{
    SDL_Point src;
    SDL_Point dst;
    SDL_Color color;
} OVL_GeometryCopyData;

SDL_COMPILE_TIME_ASSERT(ovl_point, sizeof(SDL_FPoint) == sizeof(SDL_Point));
SDL_COMPILE_TIME_ASSERT(ovl_rect, sizeof(SDL_FRect) == sizeof(SDL_Rect));
SDL_COMPILE_TIME_ASSERT(ovl_fill, sizeof(OVL_GeometryFillQueued) == sizeof(OVL_GeometryFillData));
SDL_COMPILE_TIME_ASSERT(ovl_copy, sizeof(OVL_GeometryCopyQueued) == sizeof(OVL_GeometryCopyData));

static SDL_INLINE int OVL_Floor(double v)
{
    return (int)(v + 4194304.0) - 4194304;
}

static SDL_INLINE int OVL_Round(float v)
{
    return OVL_Floor((double)v + 0.5);
}

static struct Window *OVL_GetIntuiWindow(const OVL_RenderData *data)
{
    const SDL_WindowData *wd = (const SDL_WindowData *)data->window->driverdata;
    return wd ? wd->win : NULL;
}

/* -------------------------------------------------------------------------
 * Overlay
 * ------------------------------------------------------------------------- */

static SDL_bool OVL_OpenCGXVideo(void)
{
    if (!CGXVideoBase) {
        CGXVideoBase = OpenLibrary("cgxvideo.library", 43);
        if (!CGXVideoBase) {
            return SDL_FALSE;
        }
    }
    OVL_cgxvideo_users++;
    return SDL_TRUE;
}

static void OVL_CloseCGXVideo(void)
{
    if (OVL_cgxvideo_users > 0 && --OVL_cgxvideo_users == 0) {
        CloseLibrary(CGXVideoBase);
        CGXVideoBase = NULL;
    }
}

static ULONG OVL_Query(struct Screen *screen, ULONG attr)
{
    if (!screen || CGXVideoBase->lib_Version < 50) {
        return 0;
    }
    return QueryVLayerAttr(screen, attr);
}

#ifdef __SDL_DEBUG
static const char *OVL_ModeName(OVL_Mode mode)
{
    return (mode == OVL_MODE_DIRECT) ? "direct" : "composed";
}

static const char *OVL_FormatName(ULONG srcfmt)
{
    return (srcfmt == SRCFMT_YCbCr420) ? "YCbCr420" : (srcfmt == SRCFMT_YCbCr16) ? "YCbCr16" : "RGB16";
}
#endif

static void OVL_DestroyOverlay(OVL_RenderData *data)
{
    if (data->vlayer) {
        D("[%s] %s %s overlay %ldx%ld\n", __FUNCTION__, OVL_ModeName(data->mode), OVL_FormatName(data->srcfmt),
          (long)data->src_w, (long)data->src_h);
        DetachVLayer(data->vlayer);
        DeleteVLayerHandle(data->vlayer);
        data->vlayer = NULL;
    }
    data->vlayer_win = NULL;
    data->colorkey = SDL_FALSE;
    data->bars_dirty = SDL_TRUE;
    data->shown_texture = NULL;
    data->shown_clear = SDL_FALSE;
    data->pending_texture = NULL;
}

/* VSQ_SupportedFormats bit of an overlay format */
static ULONG OVL_QueryFormat(ULONG srcfmt)
{
    switch (srcfmt) {
    case SRCFMT_YCbCr16:
        return VSQ_FMT_YUYV;
    case SRCFMT_YCbCr420:
        return VSQ_FMT_YUV420_PLANAR;
    default:
        return VSQ_FMT_R5G6B5_LE;
    }
}

static SDL_bool OVL_IsPlanarYUV(Uint32 format)
{
    switch (format) {
    case SDL_PIXELFORMAT_YV12:
    case SDL_PIXELFORMAT_IYUV:
    case SDL_PIXELFORMAT_NV12:
    case SDL_PIXELFORMAT_NV21:
        return SDL_TRUE;
    default:
        return SDL_FALSE;
    }
}

/* Overlay format showing a texture directly: YCbCr420, then YCbCr16, then
   RGB16 as each one fails, see OVL_SetupOverlay() */
static ULONG OVL_OverlayFormat(OVL_RenderData *data, struct Window *win, Uint32 format)
{
    if (!SDL_ISPIXELFORMAT_FOURCC(format) || data->no_yuv) {
        return SRCFMT_RGB16;
    }
    if (OVL_IsPlanarYUV(format) && !data->no_yuv420) {
        if (win->WScreen != data->query_screen) {
            data->query_screen = win->WScreen;
            data->query_formats = OVL_Query(win->WScreen, VSQ_SupportedFormats);
        }
        /* Tried anyway without QueryVLayerAttr() (before cgxvideo 50) */
        if (!data->query_formats || (data->query_formats & VSQ_FMT_YUV420_PLANAR)) {
            return SRCFMT_YCbCr420;
        }
    }
    return SRCFMT_YCbCr16;
}

static void OVL_WindowClosing(void *userdata)
{
    OVL_RenderData *data = (OVL_RenderData *)userdata;

    D("[%s] Intuition window closing\n", __FUNCTION__);
    OVL_DestroyOverlay(data);
    SDL_zero(data->failed);
}

static SDL_bool OVL_SetupOverlay(OVL_RenderData *data, struct Window *win, OVL_Mode mode, ULONG srcfmt,
                                 int w, int h, SDL_bool filter, SDL_bool wait_switch)
{
    struct Screen *screen = win->WScreen;
    OVL_Failure *fail = &data->failed[mode];
    struct VLayerHandle *vlayer;
    ULONG features = 0, formats = 0, max_width = 0, error = 0;
    SDL_bool double_buffer, colorkey;

    if (data->vlayer && data->vlayer_win == win && data->srcfmt == srcfmt &&
        data->src_w == w && data->src_h == h && data->filter == filter) {
        if (data->mode != mode) {
            D("[%s] %ldx%ld overlay now %s\n", __FUNCTION__, (long)w, (long)h, OVL_ModeName(mode));
        }
        data->mode = mode;
        /* A frame waiting for a direct overlay is shown by the composed one
           meanwhile: that must not restart the count, see OVL_RenderPresent() */
        if (mode == OVL_MODE_DIRECT) {
            data->switch_count = 0;
        }
        return SDL_TRUE;
    }

    if (wait_switch && data->vlayer && data->vlayer_win == win &&
        ++data->switch_count < OVL_SWITCH_FRAMES) {
        return SDL_FALSE;
    }
    data->switch_count = 0;

    if (!screen || w < 1 || h < 1) {
        return SDL_FALSE;
    }
    /* Known to fail: tried again only while no overlay is shown, destroying a
       working one for that would flicker */
    if (fail->win == win && fail->srcfmt == srcfmt && fail->w == w && fail->h == h &&
        ((data->vlayer && data->vlayer_win == win) || !SDL_TICKS_PASSED(SDL_GetTicks(), fail->ticks + OVL_RETRY_MS))) {
        return SDL_FALSE;
    }

    OVL_DestroyOverlay(data);

    features = OVL_Query(screen, VSQ_SupportedFeatures);
    formats = OVL_Query(screen, VSQ_SupportedFormats);
    max_width = OVL_Query(screen, VSQ_MaxWidth);
    double_buffer = (!features || (features & VSQ_FEAT_DOUBLEBUFFER)) ? SDL_TRUE : SDL_FALSE;
    colorkey = (!features || (features & VSQ_FEAT_COLORKEYING)) ? SDL_TRUE : SDL_FALSE;

    D("[%s] new %s %s overlay %ldx%ld, filter %ld, double buffer %ld, color key %ld, screen 0x%08lx %ldx%ld\n", __FUNCTION__,
      OVL_ModeName(mode), OVL_FormatName(srcfmt), (long)w, (long)h, (long)filter, (long)double_buffer, (long)colorkey,
      (unsigned long)screen, (long)screen->Width, (long)screen->Height);

    if ((features && !(features & VSQ_FEAT_OVERLAY)) ||
        (formats && !(formats & OVL_QueryFormat(srcfmt))) ||
        (max_width && (ULONG)w > max_width)) {
        goto failed;
    }

    vlayer = CreateVLayerHandleTags(screen,
                                    VOA_SrcType, srcfmt,
                                    VOA_SrcWidth, (ULONG)w,
                                    VOA_SrcHeight, (ULONG)h,
                                    VOA_DoubleBuffer, (ULONG)double_buffer,
                                    VOA_UseFilter, (ULONG)filter,
                                    VOA_UseColorKey, (ULONG)colorkey,
                                    VOA_Error, (ULONG)&error,
                                    TAG_DONE);
    if (vlayer && AttachVLayerTags(vlayer, win,
                                   VOA_LeftIndent, 0, VOA_RightIndent, 0,
                                   VOA_TopIndent, 0, VOA_BottomIndent, 0,
                                   TAG_DONE) != 0) {
        DeleteVLayerHandle(vlayer);
        vlayer = NULL;
    }
    if (!vlayer) {
        goto failed;
    }

    data->vlayer = vlayer;
    data->vlayer_win = win;
    data->mode = mode;
    data->srcfmt = srcfmt;
    data->src_w = w;
    data->src_h = h;
    data->filter = filter;
    data->double_buffer = double_buffer;
    data->colorkey = colorkey;
    data->key_color = colorkey ? (0xFF000000 | GetVLayerAttr(vlayer, VOA_ColorKey)) : 0;
    D("[%s] key color 0x%08lx\n", __FUNCTION__, (unsigned long)data->key_color);
    data->indents[0] = data->indents[1] = data->indents[2] = data->indents[3] = -1;
    data->bars_dirty = SDL_TRUE;
    data->dims_logged = SDL_FALSE;
    data->write_trials = 0;
    data->write_best[0] = data->write_best[1] = (Uint64)-1;
    return SDL_TRUE;

failed:
    D("[%s] no %ldx%ld overlay: error %ld, features 0x%08lx, formats 0x%08lx, max width %ld\n", __FUNCTION__,
      (long)w, (long)h, (long)error, (unsigned long)features, (unsigned long)formats, (long)max_width);
    /* YUV formats are not tried again, see OVL_OverlayFormat() */
    if (srcfmt == SRCFMT_YCbCr420) {
        data->no_yuv420 = SDL_TRUE;
    } else if (srcfmt == SRCFMT_YCbCr16) {
        data->no_yuv = SDL_TRUE;
    }
    fail->win = win;
    fail->srcfmt = srcfmt;
    fail->w = w;
    fail->h = h;
    fail->ticks = SDL_GetTicks();
    return SDL_FALSE;
}

/* Shows the overlay on dst, in pixels of the window inner area */
static void OVL_SetGeometry(OVL_RenderData *data, struct Window *win, const SDL_Rect *dst)
{
    const LONG inner_w = win->Width - win->BorderLeft - win->BorderRight;
    const LONG inner_h = win->Height - win->BorderTop - win->BorderBottom;
    const LONG left = win->LeftEdge + (win->WScreen ? win->WScreen->LeftEdge : 0);
    const LONG top = win->TopEdge + (win->WScreen ? win->WScreen->TopEdge : 0);
    LONG indents[4];

    data->shown = *dst;

    indents[0] = SDL_max(0, dst->x);
    indents[1] = SDL_max(0, dst->y);
    indents[2] = SDL_max(0, inner_w - (dst->x + dst->w));
    indents[3] = SDL_max(0, inner_h - (dst->y + dst->h));

    if (indents[2] > 0 || win->BorderRight > 0) {
        indents[2]--;
    }
    if (indents[3] > 0 || win->BorderBottom > 0) {
        indents[3]--;
    }

    if (SDL_memcmp(indents, data->indents, sizeof(indents)) != 0) {
        D("[%s] window %ldx%ld, overlay at %ld,%ld %ldx%ld\n", __FUNCTION__, (long)inner_w, (long)inner_h,
          (long)dst->x, (long)dst->y, (long)dst->w, (long)dst->h);
        SDL_memcpy(data->indents, indents, sizeof(indents));
        data->bars_dirty = SDL_TRUE;
    } else if (left == data->win_left && top == data->win_top) {
        return;
    }

    data->win_left = left;
    data->win_top = top;
    SetVLayerAttrTags(data->vlayer,
                      VOA_LeftIndent, (ULONG)indents[0],
                      VOA_TopIndent, (ULONG)indents[1],
                      VOA_RightIndent, (ULONG)indents[2],
                      VOA_BottomIndent, (ULONG)indents[3],
                      TAG_DONE);
}

static void OVL_FillWindowRect(struct Window *win, LONG x, LONG y, LONG w, LONG h, ULONG argb)
{
    if (w > 0 && h > 0) {
        FillPixelArray(win->RPort, (UWORD)(win->BorderLeft + x), (UWORD)(win->BorderTop + y),
                       (UWORD)w, (UWORD)h, argb);
    }
}

static void OVL_PaintBars(OVL_RenderData *data, struct Window *win, const SDL_Rect *dst, SDL_bool under_overlay)
{
    const LONG inner_w = win->Width - win->BorderLeft - win->BorderRight;
    const LONG inner_h = win->Height - win->BorderTop - win->BorderBottom;
    const ULONG argb = 0xFF000000 | (data->clear_color & 0x00FFFFFF);
    const SDL_bool keyed = (under_overlay && data->colorkey) ? SDL_TRUE : SDL_FALSE;
    LONG x0, y0, x1, y1;

    if (!data->bars_dirty && argb == data->bars_color && under_overlay == data->bars_under) {
        return;
    }
    data->bars_dirty = SDL_FALSE;
    data->bars_color = argb;
    data->bars_under = under_overlay;

    if (!win->RPort) {
        return;
    }

    if (under_overlay && !keyed) {
        OVL_FillWindowRect(win, 0, 0, inner_w, inner_h, argb);
        return;
    }

    x0 = SDL_max(0, dst->x);
    y0 = SDL_max(0, dst->y);
    x1 = SDL_min(inner_w, dst->x + dst->w);
    y1 = SDL_min(inner_h, dst->y + dst->h);
    if (keyed && win->BorderRight == 0) {
        x1 = SDL_min(x1, inner_w - 1);
    }
    if (keyed && win->BorderBottom == 0) {
        y1 = SDL_min(y1, inner_h - 1);
    }

    if (x1 <= x0 || y1 <= y0) {
        OVL_FillWindowRect(win, 0, 0, inner_w, inner_h, argb);
        return;
    }
    OVL_FillWindowRect(win, 0, 0, inner_w, y0, argb);
    OVL_FillWindowRect(win, 0, y1, inner_w, inner_h - y1, argb);
    OVL_FillWindowRect(win, 0, y0, x0, y1 - y0, argb);
    OVL_FillWindowRect(win, x1, y0, inner_w - x1, y1 - y0, argb);
    if (keyed) {
        OVL_FillWindowRect(win, x0, y0, x1 - x0, y1 - y0, data->key_color);
    }
}

/* -------------------------------------------------------------------------
 * Pixel conversion to the overlay format, RGB565 little endian
 * ------------------------------------------------------------------------- */

static SDL_INLINE Uint16 OVL_PackRGB(Uint32 r, Uint32 g, Uint32 b)
{
    return SDL_SwapLE16((Uint16)(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)));
}

static SDL_bool OVL_IsDirectFormat(Uint32 format)
{
    switch (format) {
    case SDL_PIXELFORMAT_ARGB8888:
    case SDL_PIXELFORMAT_RGB888:
    case SDL_PIXELFORMAT_ABGR8888:
    case SDL_PIXELFORMAT_BGR888:
    case SDL_PIXELFORMAT_RGBA8888:
    case SDL_PIXELFORMAT_BGRA8888:
    case SDL_PIXELFORMAT_RGB565:
    case SDL_PIXELFORMAT_RGB555:
    /* YUV overlay */
    case SDL_PIXELFORMAT_YV12:
    case SDL_PIXELFORMAT_IYUV:
    case SDL_PIXELFORMAT_NV12:
    case SDL_PIXELFORMAT_NV21:
    case SDL_PIXELFORMAT_YUY2:
    case SDL_PIXELFORMAT_UYVY:
    case SDL_PIXELFORMAT_YVYU:
        return SDL_TRUE;
    default:
        return SDL_FALSE;
    }
}

/* Native RGB565 of a pixel, (c & 0xF8) << 8 | (c & 0xFC) << 3 | c >> 3 on its channels */
#define OVL_565_ARGB(p) ((((p) >> 8) & 0xF800) | (((p) >> 5) & 0x07E0) | (((p) >> 3) & 0x001F))
#define OVL_565_ABGR(p) ((((p) << 8) & 0xF800) | (((p) >> 5) & 0x07E0) | (((p) >> 19) & 0x001F))
#define OVL_565_RGBA(p) ((((p) >> 16) & 0xF800) | (((p) >> 13) & 0x07E0) | (((p) >> 11) & 0x001F))
#define OVL_565_BGRA(p) (((p) & 0xF800) | (((p) >> 13) & 0x07E0) | ((p) >> 27))
#define OVL_565_555(p)  ((((p) & 0x7FE0) << 1) | (((p) >> 4) & 0x0020) | ((p) & 0x001F))
#define OVL_565_565(p)  (p)

/* CPUs without hardware prefetching (e5500) wait for the memory on each cache
   miss: lines are asked for ahead, OVL_PREFETCH pixels when converting rows,
   OVL_PREFETCH_ROWS rows when drawing, 32 bytes apart for 32 or 64-byte
   lines. dcbt and dcbtst are only hints, harmless on uncached memory. */
#define OVL_PREFETCH 128
#define OVL_PREFETCH_ROWS 2

static SDL_INLINE void OVL_PrefetchWrite(Uint8 *p, int bytes)
{
    int i;
    for (i = 0; i < bytes; i += 32) {
        __builtin_prefetch(p + i, 1);
    }
}

/* Two pixels per 32-bit store, in little endian order */
#define OVL_CONVERT_PAIRS(T, PIX)                                     \
    {                                                                 \
        const T *s = (const T *)src;                                  \
        Uint32 *d = (Uint32 *)dst;                                    \
        for (x = 0; x + 1 < w; x += 2) {                              \
            const Uint32 p0 = s[x], p1 = s[x + 1];                    \
            if (!(x & 7)) {                                           \
                __builtin_prefetch(s + x + OVL_PREFETCH, 0);          \
                __builtin_prefetch(d + OVL_PREFETCH / 2, 1);          \
            }                                                         \
            *d++ = SDL_SwapLE32(PIX(p0) | (PIX(p1) << 16));           \
        }                                                             \
        if (x < w) {                                                  \
            const Uint32 p0 = s[x];                                   \
            *(Uint16 *)d = SDL_SwapLE16((Uint16)PIX(p0));             \
        }                                                             \
    }

/* dst must be 4-byte aligned */
static void OVL_ConvertRow(Uint32 format, const void *src, Uint16 *dst, int w)
{
    int x;

    switch (format) {
    case SDL_PIXELFORMAT_ARGB8888:
    case SDL_PIXELFORMAT_RGB888:
        OVL_CONVERT_PAIRS(Uint32, OVL_565_ARGB)
        break;
    case SDL_PIXELFORMAT_ABGR8888:
    case SDL_PIXELFORMAT_BGR888:
        OVL_CONVERT_PAIRS(Uint32, OVL_565_ABGR)
        break;
    case SDL_PIXELFORMAT_RGBA8888:
        OVL_CONVERT_PAIRS(Uint32, OVL_565_RGBA)
        break;
    case SDL_PIXELFORMAT_BGRA8888:
        OVL_CONVERT_PAIRS(Uint32, OVL_565_BGRA)
        break;
    case SDL_PIXELFORMAT_RGB565:
        OVL_CONVERT_PAIRS(Uint16, OVL_565_565)
        break;
    case SDL_PIXELFORMAT_RGB555:
        OVL_CONVERT_PAIRS(Uint16, OVL_565_555)
        break;
    default:
        break;
    }
}

#ifdef __SDL_DEBUG
/* exec knows the address: system RAM, not video memory */
static SDL_bool OVL_IsSystemRAM(const void *address)
{
    return TypeOfMem((APTR)address) ? SDL_TRUE : SDL_FALSE;
}
#endif

/* Rows converted in RAM, then copied by CopyMem(): in video memory each store
   is a bus transfer */
static SDL_bool OVL_GrowRowBuffer(OVL_RenderData *data, size_t size)
{
    if (size > data->row_buf_size) {
        Uint8 *buf = (Uint8 *)SDL_SIMDAlloc(size);
        if (!buf) {
            return SDL_FALSE;
        }
        SDL_SIMDFree(data->row_buf);
        data->row_buf = buf;
        data->row_buf_size = size;
    }
    return SDL_TRUE;
}

/* Locks the overlay buffer being drawn: its address and bytes per row,
   min_modulo if the driver doesn't tell */
static UBYTE *OVL_LockOverlay(OVL_RenderData *data, ULONG *modulo, ULONG min_modulo)
{
    struct VLayerHandle *vlayer = data->vlayer;
    UBYTE *base;

    /* Whatever is written now replaces a frame left there, see OVL_WriteThroughYUV() */
    data->pending_texture = NULL;
#ifdef __SDL_DEBUG
    data->upload_lock = SDL_GetPerformanceCounter();
#endif
    if (!LockVLayer(vlayer)) {
        D("[%s] LockVLayer() failed\n", __FUNCTION__);
        return NULL;
    }
#ifdef __SDL_DEBUG
    data->upload_start = SDL_GetPerformanceCounter();
#endif
    base = (UBYTE *)GetVLayerAttr(vlayer, VOA_BaseAddress);
    *modulo = GetVLayerAttr(vlayer, VOA_Modulo);
    if (!*modulo) {
        *modulo = min_modulo;
    }
    if (!base) {
        D("[%s] no VOA_BaseAddress\n", __FUNCTION__);
        UnlockVLayer(vlayer);
        return NULL;
    }
    if (!data->dims_logged) {
        D("[%s] %ldx%ld %s overlay: VOA_Width %ld, VOA_Height %ld, VOA_Modulo %ld, base 0x%08lx in %s\n", __FUNCTION__,
          (long)data->src_w, (long)data->src_h, OVL_FormatName(data->srcfmt), (long)GetVLayerAttr(vlayer, VOA_Width),
          (long)GetVLayerAttr(vlayer, VOA_Height), (long)*modulo, (unsigned long)base,
          OVL_IsSystemRAM(base) ? "system RAM" : "video memory");
        data->dims_logged = SDL_TRUE;
    }
    return base;
}

/* Shows the buffer drawn into, once unlocked. lock, write, unlock: their
   durations, for the debug log */
static void OVL_SwapOverlay(OVL_RenderData *data, Uint64 lock, Uint64 write, Uint64 unlock)
{
#ifdef __SDL_DEBUG
    const Uint64 start = SDL_GetPerformanceCounter();
#endif

    data->pending_texture = NULL;
    if (data->vsync) {
        WaitTOF();
    }
    if (data->double_buffer) {
        SwapVLayerBuffer(data->vlayer);
    }

#ifdef __SDL_DEBUG
    {
        const Uint64 shown = SDL_GetPerformanceCounter();
        data->upload_ticks[0] += lock;
        data->upload_ticks[1] += write;
        data->upload_ticks[2] += unlock;
        data->upload_ticks[3] += shown - start;
        /* Once a second: the log itself costs */
        ++data->upload_frames;
        if (shown - data->upload_log >= SDL_GetPerformanceFrequency()) {
            const Uint64 div = SDL_GetPerformanceFrequency() * data->upload_frames;
            if (data->upload_log) {
                D("[%s] %ldx%ld %s, us per upload: lock %ld, write %ld, unlock %ld, %s %ld (%ld uploads)\n", __FUNCTION__,
                  (long)data->src_w, (long)data->src_h, OVL_FormatName(data->srcfmt),
                  (long)(data->upload_ticks[0] * 1000000 / div), (long)(data->upload_ticks[1] * 1000000 / div),
                  (long)(data->upload_ticks[2] * 1000000 / div), data->vsync ? "WaitTOF+swap" : "swap",
                  (long)(data->upload_ticks[3] * 1000000 / div), (long)data->upload_frames);
            }
            SDL_zeroa(data->upload_ticks);
            data->upload_frames = 0;
            data->upload_log = shown;
        }
    }
#endif
    if (data->in_fallback) {
        D("[%s] back to the overlay\n", __FUNCTION__);
        data->in_fallback = SDL_FALSE;
    }
}

/* Unlocks the overlay and shows what was drawn into it */
static void OVL_ShowOverlay(OVL_RenderData *data)
{
#ifdef __SDL_DEBUG
    const Uint64 written = SDL_GetPerformanceCounter();
#endif

    UnlockVLayer(data->vlayer);
#ifdef __SDL_DEBUG
    OVL_SwapOverlay(data, data->upload_start - data->upload_lock, written - data->upload_start,
                    SDL_GetPerformanceCounter() - written);
#else
    OVL_SwapOverlay(data, 0, 0, 0);
#endif
}

/* Converts rect of src into an RGB16 overlay and shows it. The whole buffer
   is rewritten: with double buffering, the one drawn doesn't hold the
   previous frame.

   The buffer may be video memory, uncached RAM, or cached RAM that some
   drivers copy to video memory at the swap: converting in place is slow on
   the first ones, copying needless on the last one. The first uploads of an
   overlay try both, the fastest time of each decides. AltiVec stores of 16
   bytes go in place anyway. */
static SDL_bool OVL_UploadRGB(OVL_RenderData *data, SDL_Surface *src, const SDL_Rect *rect)
{
    const Uint32 format = src->format->format;
    const ULONG row_bytes = (ULONG)rect->w * 2;
    const ULONG stride = (row_bytes + 3) & ~3; /* rows 4-byte aligned in RAM, see OVL_ConvertRow() */
    const int chunk = SDL_max(1, OVL_UPLOAD_CHUNK / (int)stride);
    const SDL_bool timed = (data->write_trials < OVL_WRITE_TRIALS) ? SDL_TRUE : SDL_FALSE;
    const Uint8 *row = (const Uint8 *)src->pixels + rect->y * src->pitch + rect->x * src->format->BytesPerPixel;
    UBYTE *base;
    ULONG modulo;
    Uint64 start = 0;
    int y, n, i, method;

    base = OVL_LockOverlay(data, &modulo, row_bytes);
    if (!base) {
        return SDL_FALSE;
    }

    if (data->altivec && OVL_IsAltiVecFormat(format)) {
        for (y = 0; y < rect->h; y++, row += src->pitch, base += modulo) {
            OVL_ConvertRowAltiVec(format, row, base, rect->w);
        }
        OVL_ShowOverlay(data);
        return SDL_TRUE;
    }

    method = timed ? (data->write_trials & 1) : data->write_method;
    if (method == 1 && (((uintptr_t)base | modulo) & 3)) {
        method = 0; /* OVL_ConvertRow() stores 32 bits */
    }
    if (timed) {
        start = SDL_GetPerformanceCounter();
    }

    if (method == 1) {
        for (y = 0; y < rect->h; y++, row += src->pitch, base += modulo) {
            OVL_ConvertRow(format, row, (Uint16 *)base, rect->w);
        }
    } else {
        if (!OVL_GrowRowBuffer(data, (size_t)stride * SDL_min(chunk, rect->h))) {
            UnlockVLayer(data->vlayer);
            return SDL_FALSE;
        }
        /* Rows are converted a few at a time, copied at once when the
           overlay rows follow each other too. */
        for (y = 0; y < rect->h; y += n) {
            n = SDL_min(chunk, rect->h - y);
            for (i = 0; i < n; i++, row += src->pitch) {
                OVL_ConvertRow(format, row, (Uint16 *)(data->row_buf + i * stride), rect->w);
            }
            if (modulo == stride) {
                CopyMem(data->row_buf, base, stride * n);
                base += stride * n;
            } else {
                for (i = 0; i < n; i++, base += modulo) {
                    CopyMem(data->row_buf + i * stride, base, row_bytes);
                }
            }
        }
    }

    if (timed) {
        const Uint64 ticks = SDL_GetPerformanceCounter() - start;
        data->write_best[method] = SDL_min(data->write_best[method], ticks);
        if (++data->write_trials == OVL_WRITE_TRIALS) {
            data->write_method = (data->write_best[1] < data->write_best[0]) ? 1 : 0;
            D("[%s] %ldx%ld written %s\n", __FUNCTION__, (long)rect->w, (long)rect->h,
              data->write_method ? "in place" : "through RAM");
        }
    }
    OVL_ShowOverlay(data);
    return SDL_TRUE;
}

/* Copies rows of one plane, at once if they follow each other on both sides */
static void OVL_CopyPlane(const Uint8 *src, int src_pitch, UBYTE *dst, ULONG dst_pitch, int w, int h)
{
    int y;

    if (src_pitch == w && dst_pitch == (ULONG)w) {
        CopyMem((APTR)src, dst, (ULONG)w * h);
        return;
    }
    for (y = 0; y < h; y++, src += src_pitch, dst += dst_pitch) {
        CopyMem((APTR)src, dst, (ULONG)w);
    }
}

/* Copies rect of a YUV texture into a YCbCr overlay and shows it. x and w
   are even, and y and h too for 4:2:0 textures, see OVL_TryDirectCopy().

   SRCFMT_YCbCr16 is packed Y0 U Y1 V (YUY2). SRCFMT_YCbCr420 is planar, Y
   then U then V, with VOA_Modulo counting two bytes per pixel, as mplayer
   uses it. */
static SDL_bool OVL_UploadYUV(OVL_RenderData *data, SDL_Texture *texture, const SDL_Rect *rect)
{
    const SDL_SW_YUVTexture *yuv = ((const OVL_TextureData *)texture->driverdata)->yuv;
    const Uint32 format = texture->format;
    const int w = rect->w, h = rect->h;
    const int pitch = yuv->pitches[0];
    Uint32 *row;
    UBYTE *base;
    ULONG modulo;
    int x, y;

    if (!OVL_GrowRowBuffer(data, (size_t)w * 2)) {
        return SDL_FALSE;
    }
    row = (Uint32 *)data->row_buf;
    base = OVL_LockOverlay(data, &modulo, (ULONG)w * 2);
    if (!base) {
        return SDL_FALSE;
    }

    if (!OVL_IsPlanarYUV(format)) {
        /* Packed 4:2:2, 32-bit aligned: pitch is a multiple of 4 and x even */
        const Uint8 *src = yuv->planes[0] + rect->y * pitch + rect->x * 2;

        if (format == SDL_PIXELFORMAT_YUY2) {
            OVL_CopyPlane(src, pitch, base, modulo, w * 2, h);
        } else {
            for (y = 0; y < h; y++, src += pitch, base += modulo) {
                const Uint32 *s = (const Uint32 *)src;
                if (format == SDL_PIXELFORMAT_UYVY) {
                    for (x = 0; x < w / 2; x++) {
                        const Uint32 p = s[x];
                        row[x] = ((p & 0x00FF00FF) << 8) | ((p >> 8) & 0x00FF00FF);
                    }
                } else { /* YVYU */
                    for (x = 0; x < w / 2; x++) {
                        const Uint32 p = s[x];
                        row[x] = (p & 0xFF00FF00) | ((p >> 16) & 0xFF) | ((p & 0xFF) << 16);
                    }
                }
                CopyMem(row, base, (ULONG)w * 2);
            }
        }
    } else {
        /* 4:2:0: planes U then V for IYUV, V then U for YV12, interleaved
           UV for NV12 and VU for NV21 */
        const SDL_bool nv = (format == SDL_PIXELFORMAT_NV12 || format == SDL_PIXELFORMAT_NV21) ? SDL_TRUE : SDL_FALSE;
        const int step = nv ? 2 : 1;
        const int cpitch = yuv->pitches[1];
        const int cw = w / 2;
        const Uint8 *ysrc = yuv->planes[0] + rect->y * pitch + rect->x;
        const Uint8 *c1 = yuv->planes[1] + (rect->y / 2) * cpitch + (rect->x / 2) * step;
        const Uint8 *c2 = nv ? c1 + 1 : yuv->planes[2] + (rect->y / 2) * cpitch + rect->x / 2;
        const SDL_bool u_first = (format == SDL_PIXELFORMAT_IYUV || format == SDL_PIXELFORMAT_NV12) ? SDL_TRUE : SDL_FALSE;
        const Uint8 *u = u_first ? c1 : c2;
        const Uint8 *v = u_first ? c2 : c1;

        if (data->srcfmt == SRCFMT_YCbCr420) {
            const ULONG ypitch = modulo / 2, upitch = ypitch / 2;
            UBYTE *ubase = base + ypitch * data->src_h;
            UBYTE *vbase = ubase + upitch * (data->src_h / 2);

            OVL_CopyPlane(ysrc, pitch, base, ypitch, w, h);
            if (!nv) {
                OVL_CopyPlane(u, cpitch, ubase, upitch, cw, h / 2);
                OVL_CopyPlane(v, cpitch, vbase, upitch, cw, h / 2);
            } else {
                Uint8 *ubuf = data->row_buf, *vbuf = data->row_buf + cw;
                for (y = 0; y < h / 2; y++, u += cpitch, v += cpitch, ubase += upitch, vbase += upitch) {
                    for (x = 0; x < cw; x++) {
                        ubuf[x] = u[2 * x];
                        vbuf[x] = v[2 * x];
                    }
                    CopyMem(ubuf, ubase, (ULONG)cw);
                    CopyMem(vbuf, vbase, (ULONG)cw);
                }
            }
        } else {
            /* Packed: each chroma row serves two rows */
            for (y = 0; y < h; y++, ysrc += pitch, base += modulo) {
                const Uint8 *us = u + (y / 2) * cpitch;
                const Uint8 *vs = v + (y / 2) * cpitch;
                for (x = 0; x < cw; x++) {
                    row[x] = ((Uint32)ysrc[2 * x] << 24) | ((Uint32)us[x * step] << 16) |
                             ((Uint32)ysrc[2 * x + 1] << 8) | vs[x * step];
                }
                CopyMem(row, base, (ULONG)w * 2);
            }
        }
    }
    OVL_ShowOverlay(data);
    return SDL_TRUE;
}

/* Surface to draw the texture from, YUV converted to RGB if it changed */
static SDL_Surface *OVL_GetSurface(SDL_Texture *texture)
{
    OVL_TextureData *td = (OVL_TextureData *)texture->driverdata;

    if (td->yuv && td->surface_version != td->version) {
        SDL_ConvertPixels(texture->w, texture->h, texture->format, td->yuv->planes[0], td->yuv->pitches[0],
                          td->surface->format->format, td->surface->pixels, td->surface->pitch);
        td->surface_version = td->version;
    }
    return td->surface;
}

/* YUV textures go through their RGB copy without a YUV overlay */
static SDL_bool OVL_UploadTexture(OVL_RenderData *data, SDL_Texture *texture, const SDL_Rect *rect)
{
    const OVL_TextureData *td = (const OVL_TextureData *)texture->driverdata;

    if (td->yuv && data->srcfmt != SRCFMT_RGB16) {
        return OVL_UploadYUV(data, texture, rect);
    }
    return OVL_UploadRGB(data, OVL_GetSurface(texture), rect);
}

/* Fills the whole overlay with one color, whatever its format, and shows it */
static SDL_bool OVL_FillOverlay(OVL_RenderData *data, ULONG argb)
{
    const int w = data->src_w, h = data->src_h;
    const int r = (argb >> 16) & 0xFF, g = (argb >> 8) & 0xFF, b = argb & 0xFF;
    /* BT.601, studio range */
    const Uint32 Y = (Uint32)(16 + ((66 * r + 129 * g + 25 * b + 128) >> 8));
    const Uint32 U = (Uint32)(128 + ((-38 * r - 74 * g + 112 * b + 128) >> 8));
    const Uint32 V = (Uint32)(128 + ((112 * r - 94 * g - 18 * b + 128) >> 8));
    Uint32 *row;
    UBYTE *base;
    ULONG modulo;
    int x, y;

    if (!OVL_GrowRowBuffer(data, (size_t)w * 2 + 4)) {
        return SDL_FALSE;
    }
    row = (Uint32 *)data->row_buf;
    base = OVL_LockOverlay(data, &modulo, (ULONG)w * 2);
    if (!base) {
        return SDL_FALSE;
    }

    /* One row in RAM, copied to every row of the overlay, see OVL_GrowRowBuffer() */
    if (data->srcfmt == SRCFMT_YCbCr420) {
        const ULONG ypitch = modulo / 2, upitch = ypitch / 2;
        UBYTE *chroma = base + ypitch * h;

        SDL_memset(data->row_buf, (int)Y, w);
        for (y = 0; y < h; y++, base += ypitch) {
            CopyMem(data->row_buf, base, (ULONG)w);
        }
        SDL_memset(data->row_buf, (int)U, w / 2);
        for (y = 0; y < h / 2; y++, chroma += upitch) {
            CopyMem(data->row_buf, chroma, (ULONG)(w / 2));
        }
        SDL_memset(data->row_buf, (int)V, w / 2);
        for (y = 0; y < h / 2; y++, chroma += upitch) {
            CopyMem(data->row_buf, chroma, (ULONG)(w / 2));
        }
    } else {
        const Uint32 pair = (data->srcfmt == SRCFMT_YCbCr16) ? ((Y << 24) | (U << 16) | (Y << 8) | V) :
                                                               ((Uint32)OVL_PackRGB(r, g, b) * 0x10001);
        for (x = 0; x < (w + 1) / 2; x++) {
            row[x] = pair;
        }
        for (y = 0; y < h; y++, base += modulo) {
            CopyMem(row, base, (ULONG)w * 2);
        }
    }
    OVL_ShowOverlay(data);
    return SDL_TRUE;
}

/* Frame with nothing but a clear: keeps a direct overlay instead of making a
   composed one, which would have to be recreated for the next real frame. */
static SDL_bool OVL_PresentClear(OVL_RenderData *data, struct Window *win)
{
    const ULONG argb = 0xFF000000 | (data->clear_color & 0x00FFFFFF);

    if (!data->vlayer) {
        if (win->RPort) {
            OVL_FillWindowRect(win, 0, 0, win->Width - win->BorderLeft - win->BorderRight,
                               win->Height - win->BorderTop - win->BorderBottom, argb);
        }
        data->bars_dirty = SDL_TRUE;
        if (data->vsync) {
            WaitTOF();
        }
        return SDL_TRUE;
    }
    if (data->mode != OVL_MODE_DIRECT || data->vlayer_win != win) {
        return SDL_FALSE;
    }

    OVL_PaintBars(data, win, &data->shown, SDL_TRUE);
    if (data->shown_clear && data->shown_clear_color == argb && !data->in_fallback) {
        /* Already on screen */
        if (data->vsync) {
            WaitTOF();
        }
        return SDL_TRUE;
    }
    if (!OVL_FillOverlay(data, argb)) {
        return SDL_FALSE;
    }
    data->shown_texture = NULL;
    data->shown_clear = SDL_TRUE;
    data->shown_clear_color = argb;
    return SDL_TRUE;
}

/* -------------------------------------------------------------------------
 * Composition surface
 * ------------------------------------------------------------------------- */

/* Viewport and scale for a logical size, as UpdateLogicalSize() sets them */
static void OVL_GetLogicalViewport(int w, int h, int lw, int lh, SDL_bool integer_scale,
                                   SDL_Rect *viewport, float *scale)
{
    const char *hint = SDL_GetHint(SDL_HINT_RENDER_LOGICAL_SIZE_MODE);
    const SDL_bool overscan = (hint && (*hint == '1' || SDL_strcasecmp(hint, "overscan") == 0)) ? SDL_TRUE : SDL_FALSE;
    const float want_aspect = (float)lw / lh;
    const float real_aspect = (float)w / h;

    if (integer_scale) {
        *scale = (want_aspect > real_aspect) ? (float)(w / lw) : (float)(h / lh);
        if (*scale < 1.0f) {
            *scale = 1.0f;
        }
        viewport->w = (int)SDL_floor(lw * *scale);
        viewport->x = (w - viewport->w) / 2;
        viewport->h = (int)SDL_floor(lh * *scale);
        viewport->y = (h - viewport->h) / 2;
    } else if (SDL_fabs(want_aspect - real_aspect) < 0.0001) {
        *scale = (float)w / lw;
        viewport->x = viewport->y = 0;
        viewport->w = w;
        viewport->h = h;
    } else if ((want_aspect > real_aspect) != overscan) {
        *scale = (float)w / lw;
        viewport->x = 0;
        viewport->w = w;
        viewport->h = (int)SDL_floor(lh * *scale);
        viewport->y = (h - viewport->h) / 2;
    } else {
        *scale = (float)h / lh;
        viewport->y = 0;
        viewport->h = h;
        viewport->w = (int)SDL_floor(lw * *scale);
        viewport->x = (w - viewport->w) / 2;
    }
}

/* Sizes the composition surface for the default target: logical size, or
   window size divided by the render scale. */
static SDL_Surface *OVL_UpdateComposition(SDL_Renderer *renderer, OVL_RenderData *data)
{
    const SDL_bool backup = renderer->target ? SDL_TRUE : SDL_FALSE;
    const int lw = backup ? renderer->logical_w_backup : renderer->logical_w;
    const int lh = backup ? renderer->logical_h_backup : renderer->logical_h;
    const SDL_FPoint scale = backup ? renderer->scale_backup : renderer->scale;
    SDL_Rect output, area;
    int comp_w, comp_h;

    /* Sized once per frame: SDL flushes the queue in the middle of a frame
       (texture update, destroy...) maybe at another scale, and a new surface
       would lose what was drawn. The commands are in window pixels, mapped
       through kx/ky whatever the size. */
    if (data->comp && data->comp_sized) {
        return data->comp;
    }

    output.x = output.y = 0;
    SDL_GetWindowSizeInPixels(data->window, &output.w, &output.h);
    output.w = SDL_max(output.w, 1);
    output.h = SDL_max(output.h, 1);

    if (lw > 0 && lh > 0) {
        SDL_Rect viewport;
        float s;

        OVL_GetLogicalViewport(output.w, output.h, lw, lh, renderer->integer_scale, &viewport, &s);
        if (!SDL_IntersectRect(&viewport, &output, &area)) {
            area = output;
        }
        comp_w = (int)(area.w / s + 0.5f);
        comp_h = (int)(area.h / s + 0.5f);
    } else {
        area = output;
        comp_w = (scale.x > 1.0f) ? (int)SDL_ceilf(output.w / scale.x) : output.w;
        comp_h = (scale.y > 1.0f) ? (int)SDL_ceilf(output.h / scale.y) : output.h;
    }
    comp_w = SDL_clamp(comp_w, 1, OVL_MAX_COMP_SIZE);
    comp_h = SDL_clamp(comp_h, 1, OVL_MAX_COMP_SIZE);

    data->area = area;
    data->kx = (float)comp_w / area.w;
    data->ky = (float)comp_h / area.h;

    if (!data->comp || data->comp->w != comp_w || data->comp->h != comp_h) {
        SDL_Surface *comp = SDL_CreateRGBSurfaceWithFormat(0, comp_w, comp_h, 32, SDL_PIXELFORMAT_ARGB8888);
        D("[%s] composition %ldx%ld for window area %ld,%ld %ldx%ld (logical %ldx%ld)\n", __FUNCTION__,
          (long)comp_w, (long)comp_h, (long)area.x, (long)area.y, (long)area.w, (long)area.h, (long)lw, (long)lh);
        if (!comp) {
            return data->comp;
        }
        SDL_FillRect(comp, NULL, 0xFF000000 | data->clear_color);
        SDL_FreeSurface(data->comp);
        data->comp = comp;
    }
    data->comp_sized = SDL_TRUE;
    return data->comp;
}

/* -------------------------------------------------------------------------
 * Drawing, adapted from the software renderer
 * ------------------------------------------------------------------------- */

static void OVL_MapFRect(const OVL_Xform *xf, const SDL_FRect *r, SDL_Rect *out)
{
    const int x0 = OVL_Round(xf->ox + r->x * xf->kx);
    const int y0 = OVL_Round(xf->oy + r->y * xf->ky);
    const int x1 = OVL_Round(xf->ox + (r->x + r->w) * xf->kx);
    const int y1 = OVL_Round(xf->oy + (r->y + r->h) * xf->ky);

    out->x = x0;
    out->y = y0;
    out->w = x1 - x0;
    out->h = y1 - y0;
}

/* Output rectangle (absolute) to surface pixels */
static void OVL_MapOutputRect(const OVL_RenderData *data, SDL_bool to_comp, const SDL_Rect *r, SDL_Rect *out)
{
    if (to_comp) {
        const int x0 = OVL_Round((r->x - data->area.x) * data->kx);
        const int y0 = OVL_Round((r->y - data->area.y) * data->ky);
        const int x1 = OVL_Round((r->x + r->w - data->area.x) * data->kx);
        const int y1 = OVL_Round((r->y + r->h - data->area.y) * data->ky);
        out->x = x0;
        out->y = y0;
        out->w = x1 - x0;
        out->h = y1 - y0;
    } else {
        *out = *r;
    }
}

static void OVL_SetupXform(const OVL_RenderData *data, SDL_Surface *surface, SDL_bool to_comp,
                           const SDL_Rect *viewport, const SDL_Rect *cliprect, OVL_Xform *xf)
{
    SDL_Rect vp, clip, bounds;

    if (viewport) {
        vp = *viewport;
    } else if (to_comp) {
        vp = data->area;
    } else {
        vp.x = vp.y = 0;
        vp.w = surface->w;
        vp.h = surface->h;
    }

    if (to_comp) {
        xf->kx = data->kx;
        xf->ky = data->ky;
        xf->ox = (vp.x - data->area.x) * data->kx;
        xf->oy = (vp.y - data->area.y) * data->ky;
    } else {
        xf->kx = 1.0f;
        xf->ky = 1.0f;
        xf->ox = (float)vp.x;
        xf->oy = (float)vp.y;
    }

    clip = vp;
    if (cliprect) {
        SDL_Rect c;
        c.x = vp.x + cliprect->x;
        c.y = vp.y + cliprect->y;
        c.w = cliprect->w;
        c.h = cliprect->h;
        if (!SDL_IntersectRect(&vp, &c, &clip)) {
            clip.w = clip.h = 0;
        }
    }
    OVL_MapOutputRect(data, to_comp, &clip, &clip);

    bounds.x = bounds.y = 0;
    bounds.w = surface->w;
    bounds.h = surface->h;
    if (!SDL_IntersectRect(&clip, &bounds, &xf->clip)) {
        xf->clip.x = xf->clip.y = xf->clip.w = xf->clip.h = 0;
    }
}

static SDL_Surface *OVL_PrepTextureForCopy(const SDL_RenderCommand *cmd)
{
    const Uint8 r = cmd->data.draw.r;
    const Uint8 g = cmd->data.draw.g;
    const Uint8 b = cmd->data.draw.b;
    const Uint8 a = cmd->data.draw.a;
    SDL_Surface *surface = OVL_GetSurface(cmd->data.draw.texture);

    SDL_SetSurfaceColorMod(surface, r, g, b);
    SDL_SetSurfaceAlphaMod(surface, a);
    SDL_SetSurfaceBlendMode(surface, cmd->data.draw.blend);
    return surface;
}

/* tmp_cache keeps the scaling surface of a clipped copy for the next one,
   the same sprite usually */
static void OVL_BlitCopy(SDL_Surface *src, const SDL_Rect *srcrect, SDL_Surface *surface,
                         const SDL_Rect *dstrect, SDL_ScaleMode scaleMode, SDL_Surface **tmp_cache)
{
    SDL_Rect s = *srcrect;
    SDL_Rect d = *dstrect;

    if (d.w <= 0 || d.h <= 0) {
        return;
    }
    if (s.w == d.w && s.h == d.h) {
        SDL_BlitSurface(src, &s, surface, &d);
        return;
    }

    /* Don't scale and clip in one go, it may lose proportion */
    if (d.x < 0 || d.y < 0 || d.x + d.w > surface->w || d.y + d.h > surface->h) {
        const SDL_bool keep = ((size_t)d.w * d.h <= OVL_MAX_SCALE_TMP) ? SDL_TRUE : SDL_FALSE;
        SDL_Surface *tmp = keep ? *tmp_cache : NULL;

        if (!tmp || tmp->w != d.w || tmp->h != d.h || tmp->format->format != src->format->format) {
            if (keep) {
                SDL_FreeSurface(*tmp_cache);
                *tmp_cache = NULL;
            }
            tmp = SDL_CreateRGBSurfaceWithFormat(0, d.w, d.h, 0, src->format->format);
        }
        if (tmp) {
            SDL_Rect r;
            SDL_BlendMode blendmode;
            Uint8 alphaMod, rMod, gMod, bMod;

            SDL_GetSurfaceBlendMode(src, &blendmode);
            SDL_GetSurfaceAlphaMod(src, &alphaMod);
            SDL_GetSurfaceColorMod(src, &rMod, &gMod, &bMod);

            r.x = r.y = 0;
            r.w = d.w;
            r.h = d.h;
            SDL_SetSurfaceBlendMode(src, SDL_BLENDMODE_NONE);
            SDL_SetSurfaceColorMod(src, 255, 255, 255);
            SDL_SetSurfaceAlphaMod(src, 255);
            SDL_PrivateUpperBlitScaled(src, &s, tmp, &r, scaleMode);

            SDL_SetSurfaceColorMod(tmp, rMod, gMod, bMod);
            SDL_SetSurfaceAlphaMod(tmp, alphaMod);
            SDL_SetSurfaceBlendMode(tmp, blendmode);
            SDL_BlitSurface(tmp, NULL, surface, &d);
            if (keep) {
                *tmp_cache = tmp;
            } else {
                SDL_FreeSurface(tmp);
            }
        }
    } else {
        SDL_PrivateUpperBlitScaled(src, &s, surface, &d, scaleMode);
    }
}

static OVL_Layout OVL_GetLayout(const SDL_Surface *surface)
{
    const SDL_PixelFormat *format = surface->format;
    OVL_Layout l;

    l.format = format;
    l.bpp = format->BytesPerPixel;
    l.packed = (l.bpp == 4 && SDL_PIXELLAYOUT(format->format) == SDL_PACKEDLAYOUT_8888) ? SDL_TRUE : SDL_FALSE;
    l.alpha = format->Amask ? SDL_TRUE : SDL_FALSE;
    l.rs = format->Rshift;
    l.gs = format->Gshift;
    l.bs = format->Bshift;
    l.as = format->Ashift;
    l.rot = 0;
    if (l.packed) {
        Uint32 top = format->Ashift;
        if (!l.alpha) {
            const Uint32 used = format->Rmask | format->Gmask | format->Bmask;
            for (top = 24; top > 0 && ((used >> top) & 0xFF); top -= 8) {
            }
        }
        l.rot = (24 - top) & 31;
    }
    return l;
}

#define OVL_ROTL(v, n) (((v) << (n)) | ((v) >> ((32 - (n)) & 31)))

SDL_FORCE_INLINE void OVL_ReadRGBA(const OVL_Layout *l, const Uint8 *p, Uint32 *r, Uint32 *g, Uint32 *b, Uint32 *a)
{
    if (l->packed) {
        const Uint32 v = *(const Uint32 *)p;
        *r = (v >> l->rs) & 0xFF;
        *g = (v >> l->gs) & 0xFF;
        *b = (v >> l->bs) & 0xFF;
        *a = l->alpha ? ((v >> l->as) & 0xFF) : 0xFF;
    } else {
        Uint8 r8, g8, b8, a8;
        SDL_GetRGBA(*(const Uint16 *)p, l->format, &r8, &g8, &b8, &a8);
        *r = r8;
        *g = g8;
        *b = b8;
        *a = a8;
    }
}

SDL_FORCE_INLINE void OVL_WriteRGBA(const OVL_Layout *l, Uint8 *p, Uint32 r, Uint32 g, Uint32 b, Uint32 a)
{
    if (l->packed) {
        *(Uint32 *)p = (r << l->rs) | (g << l->gs) | (b << l->bs) | (l->alpha ? (a << l->as) : 0);
    } else {
        *(Uint16 *)p = (Uint16)SDL_MapRGBA(l->format, (Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a);
    }
}

/* Bilinear sample, u and v in fixed point texels from the source rectangle */
SDL_FORCE_INLINE void OVL_SampleLinear(const OVL_Layout *l, const Uint8 *base, int pitch, int sw, int sh,
                                       Sint32 u, Sint32 v, int shift,
                                       Uint32 *r, Uint32 *g, Uint32 *b, Uint32 *a)
{
    const Sint32 uu = u - (1 << (shift - 1));
    const Sint32 vv = v - (1 << (shift - 1));
    const Uint32 wx = (Uint32)(uu >> (shift - 8)) & 0xFF;
    const Uint32 wy = (Uint32)(vv >> (shift - 8)) & 0xFF;
    const int x0 = SDL_clamp(uu >> shift, 0, sw - 1);
    const int x1 = SDL_clamp((uu >> shift) + 1, 0, sw - 1);
    const Uint8 *row0 = base + SDL_clamp(vv >> shift, 0, sh - 1) * pitch;
    const Uint8 *row1 = base + SDL_clamp((vv >> shift) + 1, 0, sh - 1) * pitch;
    Uint32 r00, g00, b00, a00, r10, g10, b10, a10, r01, g01, b01, a01, r11, g11, b11, a11;

    OVL_ReadRGBA(l, row0 + x0 * l->bpp, &r00, &g00, &b00, &a00);
    OVL_ReadRGBA(l, row0 + x1 * l->bpp, &r10, &g10, &b10, &a10);
    OVL_ReadRGBA(l, row1 + x0 * l->bpp, &r01, &g01, &b01, &a01);
    OVL_ReadRGBA(l, row1 + x1 * l->bpp, &r11, &g11, &b11, &a11);

#define OVL_LERP(c00, c10, c01, c11) \
    ((((c00) * (256 - wx) + (c10) * wx) * (256 - wy) + ((c01) * (256 - wx) + (c11) * wx) * wy + 0x8000) >> 16)
    *r = OVL_LERP(r00, r10, r01, r11);
    *g = OVL_LERP(g00, g10, g01, g11);
    *b = OVL_LERP(b00, b10, b01, b11);
    *a = OVL_LERP(a00, a10, a01, a11);
#undef OVL_LERP
}

/* The four bytes of p0 and p1 interpolated, two at a time */
SDL_FORCE_INLINE Uint32 OVL_Lerp32(Uint32 p0, Uint32 p1, Uint32 w)
{
    const Uint32 lo = (((p0 & 0x00FF00FF) * (256 - w) + (p1 & 0x00FF00FF) * w) >> 8) & 0x00FF00FF;
    const Uint32 hi = (((p0 >> 8) & 0x00FF00FF) * (256 - w) + ((p1 >> 8) & 0x00FF00FF) * w) & 0xFF00FF00;
    return lo | hi;
}

/* Bilinear sample of a 32-bit source, whatever the order of the channels */
SDL_FORCE_INLINE Uint32 OVL_SampleLinear32(const Uint8 *base, int pitch, int sw, int sh, Sint32 u, Sint32 v, int shift)
{
    const Sint32 uu = u - (1 << (shift - 1));
    const Sint32 vv = v - (1 << (shift - 1));
    const Uint32 wx = (Uint32)(uu >> (shift - 8)) & 0xFF;
    const Uint32 wy = (Uint32)(vv >> (shift - 8)) & 0xFF;
    const int x0 = SDL_clamp(uu >> shift, 0, sw - 1) * 4;
    const int x1 = SDL_clamp((uu >> shift) + 1, 0, sw - 1) * 4;
    const Uint8 *row0 = base + SDL_clamp(vv >> shift, 0, sh - 1) * pitch;
    const Uint8 *row1 = base + SDL_clamp((vv >> shift) + 1, 0, sh - 1) * pitch;

    return OVL_Lerp32(OVL_Lerp32(*(const Uint32 *)(row0 + x0), *(const Uint32 *)(row0 + x1), wx),
                      OVL_Lerp32(*(const Uint32 *)(row1 + x0), *(const Uint32 *)(row1 + x1), wx), wy);
}

/* Narrows [*lo, *hi) to its intersection with [a, b) */
static void OVL_Narrow(double a, double b, int *lo, int *hi)
{
    if (a > *lo) {
        *lo = (a < *hi) ? (int)a : *hi;
    }
    if (b < *hi) {
        *hi = (b > *lo) ? (int)b : *lo;
    }
}

/* One pixel of a copy between 32-bit formats, the alpha byte (or the unused
   one) rotated to the top: blends like BlitRGBtoRGBPixelAlpha() */
SDL_FORCE_INLINE void OVL_PutPixel32(Uint8 *d, Uint32 p, Uint32 srot, Uint32 drot, SDL_bool swap,
                                     Uint32 afill, Uint32 amod, SDL_bool blend)
{
    Uint32 a;

    p = OVL_ROTL(p, srot);
    if (swap) {
        p = (p & 0xFF00FF00) | ((p >> 16) & 0xFF) | ((p & 0xFF) << 16);
    }
    p |= afill;
    a = p >> 24;
    if (amod != 0xFF) {
        a = a * amod / 255;
    }
    if (!blend) {
        p = (p & 0x00FFFFFF) | (a << 24);
    } else if (a == 0) {
        return;
    } else if (a != 0xFF) {
        const Uint32 q = OVL_ROTL(*(const Uint32 *)d, drot);
        const Uint32 rb = ((q & 0xFF00FF) + (((p & 0xFF00FF) - (q & 0xFF00FF)) * a >> 8)) & 0xFF00FF;
        const Uint32 g = ((q & 0xFF00) + (((p & 0xFF00) - (q & 0xFF00)) * a >> 8)) & 0xFF00;
        p = rb | g | ((a + ((q >> 24) * (a ^ 0xFF) >> 8)) << 24);
    }
    *(Uint32 *)d = OVL_ROTL(p, (32 - drot) & 31);
}

/* One pixel of any other copy, channel by channel like SDL_Blit_Slow() */
SDL_FORCE_INLINE void OVL_PutPixel(const OVL_Layout *dl, Uint8 *d, Uint32 sr, Uint32 sg, Uint32 sb, Uint32 sa,
                                   SDL_BlendMode blend, SDL_bool color_mod,
                                   Uint32 rmod, Uint32 gmod, Uint32 bmod, Uint32 amod)
{
    Uint32 dr, dg, db, da;

    if (color_mod) {
        sr = sr * rmod / 255;
        sg = sg * gmod / 255;
        sb = sb * bmod / 255;
    }
    if (amod != 0xFF) {
        sa = sa * amod / 255;
    }

    switch (blend) {
    case SDL_BLENDMODE_BLEND:
        if (sa == 0) {
            return;
        }
        if (sa == 0xFF) {
            OVL_WriteRGBA(dl, d, sr, sg, sb, 0xFF);
            return;
        }
        OVL_ReadRGBA(dl, d, &dr, &dg, &db, &da);
        dr = sr * sa / 255 + (255 - sa) * dr / 255;
        dg = sg * sa / 255 + (255 - sa) * dg / 255;
        db = sb * sa / 255 + (255 - sa) * db / 255;
        da = sa + (255 - sa) * da / 255;
        break;
    case SDL_BLENDMODE_ADD:
        if (sa == 0) {
            return;
        }
        OVL_ReadRGBA(dl, d, &dr, &dg, &db, &da);
        if (sa < 0xFF) {
            sr = sr * sa / 255;
            sg = sg * sa / 255;
            sb = sb * sa / 255;
        }
        dr = SDL_min(sr + dr, 255);
        dg = SDL_min(sg + dg, 255);
        db = SDL_min(sb + db, 255);
        break;
    case SDL_BLENDMODE_MOD:
        OVL_ReadRGBA(dl, d, &dr, &dg, &db, &da);
        dr = sr * dr / 255;
        dg = sg * dg / 255;
        db = sb * db / 255;
        break;
    case SDL_BLENDMODE_MUL:
        OVL_ReadRGBA(dl, d, &dr, &dg, &db, &da);
        dr = SDL_min((sr * dr + dr * (255 - sa)) / 255, 255);
        dg = SDL_min((sg * dg + dg * (255 - sa)) / 255, 255);
        db = SDL_min((sb * db + db * (255 - sa)) / 255, 255);
        break;
    default:
        dr = sr;
        dg = sg;
        db = sb;
        da = sa;
        break;
    }
    OVL_WriteRGBA(dl, d, dr, dg, db, da);
}

/* Largest |p0 + i * di + j * dj| for 0 <= i <= ni and 0 <= j <= nj */
static double OVL_MaxAbs(double p0, double di, double ni, double dj, double nj)
{
    const double a = p0 + di * ni;
    const double b = p0 + dj * nj;

    return SDL_max(SDL_max(SDL_fabs(p0), SDL_fabs(a)), SDL_max(SDL_fabs(b), SDL_fabs(a + dj * nj)));
}

/* Rotated texture copy drawn straight into the surface, where the software
   renderer goes through three temporary surfaces. Each pixel center inside
   the rotated rectangle is mapped back into the source rectangle, stepping
   in fixed point with additions only. dst and center are in surface pixels,
   fractional positions are kept.

   Between 32-bit formats with the NONE or BLEND mode and no color
   modulation, whole pixels are handled with the alpha byte (or the unused
   one) rotated to the top, blending like BlitRGBtoRGBPixelAlpha(). Anything
   else goes channel by channel like SDL_Blit_Slow(). */
static void OVL_DrawRotated(SDL_Surface *surface, SDL_Surface *src, const SDL_Rect *srcrect,
                            const SDL_FRect *dst, const SDL_FPoint *center, double angle,
                            SDL_RendererFlip flip, SDL_bool linear, SDL_BlendMode blend,
                            Uint32 rmod, Uint32 gmod, Uint32 bmod, Uint32 amod)
{
    const OVL_Layout sl = OVL_GetLayout(src);
    const OVL_Layout dl = OVL_GetLayout(surface);
    const SDL_bool color_mod = ((rmod & gmod & bmod) != 0xFF) ? SDL_TRUE : SDL_FALSE;
    const SDL_bool blending = (blend == SDL_BLENDMODE_BLEND) ? SDL_TRUE : SDL_FALSE;
    const int sw = srcrect->w, sh = srcrect->h;
    const int spitch = src->pitch, dpitch = surface->pitch;
    const Uint32 srot = sl.rot, drot = dl.rot;
    const Uint32 afill = sl.alpha ? 0 : 0xFF000000;
    const Uint32 s_r = (sl.rs + srot) & 31, s_g = (sl.gs + srot) & 31, s_b = (sl.bs + srot) & 31;
    const Uint32 d_r = (dl.rs + drot) & 31, d_g = (dl.gs + drot) & 31, d_b = (dl.bs + drot) & 31;
    const SDL_bool swap = (s_r != d_r) ? SDL_TRUE : SDL_FALSE; /* red and blue, bytes 0 and 2 */
    const SDL_bool whole = (sl.packed && dl.packed && !color_mod &&
                            (blend == SDL_BLENDMODE_NONE || blending) && s_g == d_g &&
                            (swap ? (s_r == d_b && s_b == d_r) : (s_b == d_b))) ? SDL_TRUE : SDL_FALSE;
    const Uint8 *sbase;
    Uint8 *drow;
    double c, s, turns, ax, ay, bx, by, px, py, fu, fv, u0, v0, dudx, dudy, dvdx, dvdy, us, vs, lim, one;
    Sint32 ur, vr, du, dv, dur, dvr;
    Uint32 su, sv;
    int shift, x0, x1, y0, y1, y;

    if (sw < 1 || sh < 1 || !(dst->w > 0.0f) || !(dst->h > 0.0f) ||
        !(sl.packed || sl.bpp == 2) || !(dl.packed || dl.bpp == 2)) {
        return;
    }

    /* Quarter turns exactly, sin and cos only come close */
    turns = angle / 90.0;
    if (turns == SDL_floor(turns)) {
        switch (((int)SDL_fmod(turns, 4.0) + 4) % 4) {
        case 0:
            c = 1.0;
            s = 0.0;
            break;
        case 1:
            c = 0.0;
            s = 1.0;
            break;
        case 2:
            c = -1.0;
            s = 0.0;
            break;
        default:
            c = 0.0;
            s = -1.0;
            break;
        }
    } else {
        c = SDL_cos(angle * (M_PI / 180.0));
        s = SDL_sin(angle * (M_PI / 180.0));
    }

    /* Bounding box of the rotated rectangle, (x, y) -> (x c - y s, x s + y c)
       around the rotation center, clipped */
    px = dst->x + center->x;
    py = dst->y + center->y;
    ax = -center->x;
    ay = -center->y;
    bx = dst->w - center->x;
    by = dst->h - center->y;
    x0 = surface->clip_rect.x;
    x1 = x0 + surface->clip_rect.w;
    y0 = surface->clip_rect.y;
    y1 = y0 + surface->clip_rect.h;
    OVL_Narrow(SDL_ceil(px + SDL_min(ax * c, bx * c) + SDL_min(-ay * s, -by * s) - 0.5),
               SDL_floor(px + SDL_max(ax * c, bx * c) + SDL_max(-ay * s, -by * s) - 0.5) + 1.0, &x0, &x1);
    OVL_Narrow(SDL_ceil(py + SDL_min(ax * s, bx * s) + SDL_min(ay * c, by * c) - 0.5),
               SDL_floor(py + SDL_max(ax * s, bx * s) + SDL_max(ay * c, by * c) - 0.5) + 1.0, &y0, &y1);
    if (x0 >= x1 || y0 >= y1) {
        return;
    }

    /* Inverse mapping to source texels: u = u0 + qx dudx + qy dudy, with
       (qx, qy) the pixel center relative to the rotation center */
    fu = sw / (double)dst->w;
    fv = sh / (double)dst->h;
    if (flip & SDL_FLIP_HORIZONTAL) {
        u0 = (dst->w - center->x) * fu;
        dudx = -c * fu;
        dudy = -s * fu;
    } else {
        u0 = center->x * fu;
        dudx = c * fu;
        dudy = s * fu;
    }
    if (flip & SDL_FLIP_VERTICAL) {
        v0 = (dst->h - center->y) * fv;
        dvdx = s * fv;
        dvdy = -c * fv;
    } else {
        v0 = center->y * fv;
        dvdx = -s * fv;
        dvdy = c * fv;
    }
    us = u0 + (x0 + 0.5 - px) * dudx + (y0 + 0.5 - py) * dudy;
    vs = v0 + (x0 + 0.5 - px) * dvdx + (y0 + 0.5 - py) * dvdy;

    /* Fixed point: every u and v met in the box, one step past it included,
       has to fit in 31 bits */
    lim = SDL_max(OVL_MaxAbs(us, dudx, x1 - x0, dudy, y1 - y0), OVL_MaxAbs(vs, dvdx, x1 - x0, dvdy, y1 - y0));
    lim = SDL_max(lim, (double)SDL_max(sw, sh)) + 2.0;
    shift = 16;
    while (shift > 8 && lim * (1 << shift) >= 2147483648.0) {
        shift--;
    }
    if (lim * (1 << shift) >= 2147483648.0) {
        return;
    }
    one = (double)(1 << shift);
    ur = (Sint32)SDL_floor(us * one + 0.5);
    vr = (Sint32)SDL_floor(vs * one + 0.5);
    du = (Sint32)SDL_floor(dudx * one + 0.5);
    dv = (Sint32)SDL_floor(dvdx * one + 0.5);
    dur = (Sint32)SDL_floor(dudy * one + 0.5);
    dvr = (Sint32)SDL_floor(dvdy * one + 0.5);
    su = (Uint32)sw << shift;
    sv = (Uint32)sh << shift;

    sbase = (const Uint8 *)src->pixels + srcrect->y * spitch + srcrect->x * sl.bpp;
    drow = (Uint8 *)surface->pixels + y0 * dpitch + x0 * dl.bpp;
    /* Rows of the box ahead, see OVL_PREFETCH_ROWS */
    for (y = y0; y < y0 + OVL_PREFETCH_ROWS && y < y1; y++) {
        OVL_PrefetchWrite(drow + (y - y0) * dpitch, (x1 - x0) * dl.bpp);
    }
    for (y = y0; y < y1; y++, ur += dur, vr += dvr, drow += dpitch) {
        Sint32 u = ur, v = vr;
        Uint8 *d = drow;
        int n = x1 - x0;

        if (y + OVL_PREFETCH_ROWS < y1) {
            OVL_PrefetchWrite(drow + OVL_PREFETCH_ROWS * dpitch, (x1 - x0) * dl.bpp);
        }

        /* Up to the rectangle, then along it: inside a row, its pixels are
           contiguous, and their u and v always within the source */
        while (n > 0 && ((Uint32)u >= su || (Uint32)v >= sv)) {
            u += du;
            v += dv;
            d += dl.bpp;
            n--;
        }
        if (whole && !linear) {
            for (; n > 0 && (Uint32)u < su && (Uint32)v < sv; n--, u += du, v += dv, d += 4) {
                OVL_PutPixel32(d, *(const Uint32 *)(sbase + (v >> shift) * spitch + (u >> shift) * 4),
                               srot, drot, swap, afill, amod, blending);
            }
        } else if (whole) {
            for (; n > 0 && (Uint32)u < su && (Uint32)v < sv; n--, u += du, v += dv, d += 4) {
                OVL_PutPixel32(d, OVL_SampleLinear32(sbase, spitch, sw, sh, u, v, shift),
                               srot, drot, swap, afill, amod, blending);
            }
        } else {
            for (; n > 0 && (Uint32)u < su && (Uint32)v < sv; n--, u += du, v += dv, d += dl.bpp) {
                Uint32 sr, sg, sb, sa;

                if (linear) {
                    OVL_SampleLinear(&sl, sbase, spitch, sw, sh, u, v, shift, &sr, &sg, &sb, &sa);
                } else {
                    OVL_ReadRGBA(&sl, sbase + (v >> shift) * spitch + (u >> shift) * sl.bpp, &sr, &sg, &sb, &sa);
                }
                OVL_PutPixel(&dl, d, sr, sg, sb, sa, blend, color_mod, rmod, gmod, bmod, amod);
            }
        }
    }
}

/* DRAW_MUL(inva, channel) + premultiplied color on the four channels of an
   ARGB8888 pixel, as SDL_BlendFillRect_ARGB8888() and SDL_BlendPoint_ARGB8888()
   do for the BLEND mode, but two channels per multiplication: each 16-bit
   half holds one product, and (x + 1 + (x >> 8)) >> 8 is exactly x / 255
   for x < 65535 */
SDL_FORCE_INLINE Uint32 OVL_BlendARGB(Uint32 d, Uint32 inva, Uint32 add_rb, Uint32 add_ag)
{
    Uint32 rb = (d & 0x00FF00FF) * inva;
    Uint32 ag = ((d >> 8) & 0x00FF00FF) * inva;

    rb = ((rb + 0x00010001 + ((rb >> 8) & 0x00FF00FF)) >> 8) & 0x00FF00FF;
    ag = ((ag + 0x00010001 + ((ag >> 8) & 0x00FF00FF)) >> 8) & 0x00FF00FF;
    return (rb + add_rb) | ((ag + add_ag) << 8);
}

/* SDL_BlendFillRects() with the BLEND mode on ARGB8888, same pixels */
static void OVL_BlendFillRectsARGB(SDL_Surface *surface, const SDL_Rect *rects, int count,
                                   Uint32 r, Uint32 g, Uint32 b, Uint32 a)
{
    const Uint32 inva = 255 - a;
    const Uint32 add_rb = ((r * a / 255) << 16) | (b * a / 255);
    const Uint32 add_ag = (a << 16) | (g * a / 255);
    int i;

    if (a == 0) {
        return;
    }
    if (a == 0xFF) {
        SDL_FillRects(surface, rects, count, 0xFF000000 | (r << 16) | (g << 8) | b);
        return;
    }
    for (i = 0; i < count; i++) {
        SDL_Rect rect;
        Uint8 *row;
        int x, y;

        if (!SDL_IntersectRect(&rects[i], &surface->clip_rect, &rect)) {
            continue;
        }
        row = (Uint8 *)surface->pixels + rect.y * surface->pitch + rect.x * 4;
        for (y = 0; y < OVL_PREFETCH_ROWS && y < rect.h; y++) {
            OVL_PrefetchWrite(row + y * surface->pitch, rect.w * 4);
        }
        for (y = 0; y < rect.h; y++, row += surface->pitch) {
            Uint32 *p = (Uint32 *)row;
            if (y + OVL_PREFETCH_ROWS < rect.h) {
                OVL_PrefetchWrite(row + OVL_PREFETCH_ROWS * surface->pitch, rect.w * 4);
            }
            for (x = 0; x < rect.w; x++) {
                p[x] = OVL_BlendARGB(p[x], inva, add_rb, add_ag);
            }
        }
    }
}

/* SDL_BlendPoints() with the BLEND mode on ARGB8888, same pixels */
static void OVL_BlendPointsARGB(SDL_Surface *surface, const SDL_Point *points, int count,
                                Uint32 r, Uint32 g, Uint32 b, Uint32 a)
{
    const Uint32 inva = 255 - a;
    const Uint32 add_rb = ((r * a / 255) << 16) | (b * a / 255);
    const Uint32 add_ag = (a << 16) | (g * a / 255);
    const SDL_Rect clip = surface->clip_rect;
    int i;

    if (a == 0) {
        return;
    }
    if (a == 0xFF) {
        SDL_DrawPoints(surface, points, count, 0xFF000000 | (r << 16) | (g << 8) | b);
        return;
    }
    for (i = 0; i < count; i++) {
        const int x = points[i].x, y = points[i].y;
        Uint32 *p;

        if (x < clip.x || x >= clip.x + clip.w || y < clip.y || y >= clip.y + clip.h) {
            continue;
        }
        p = (Uint32 *)((Uint8 *)surface->pixels + y * surface->pitch) + x;
        *p = OVL_BlendARGB(*p, inva, add_rb, add_ag);
    }
}

static void OVL_Draw(OVL_RenderData *data, SDL_Surface *surface, const OVL_Xform *xf,
                     const SDL_RenderCommand *cmd, void *vertices)
{
    Uint8 *verts = (Uint8 *)vertices + cmd->data.draw.first;
    const int count = (int)cmd->data.draw.count;
    const Uint8 r = cmd->data.draw.r;
    const Uint8 g = cmd->data.draw.g;
    const Uint8 b = cmd->data.draw.b;
    const Uint8 a = cmd->data.draw.a;
    const SDL_BlendMode blend = cmd->data.draw.blend;
    const SDL_bool blend_argb = (blend == SDL_BLENDMODE_BLEND && surface->format->format == SDL_PIXELFORMAT_ARGB8888) ? SDL_TRUE : SDL_FALSE;
    int i;

    switch (cmd->command) {
    case SDL_RENDERCMD_DRAW_POINTS:
    case SDL_RENDERCMD_DRAW_LINES: {
        SDL_FPoint *fpoints = (SDL_FPoint *)verts;
        SDL_Point *points = (SDL_Point *)verts; /* converted in place */

        for (i = 0; i < count; i++) {
            const float x = fpoints[i].x;
            const float y = fpoints[i].y;
            points[i].x = OVL_Floor(xf->ox + x * xf->kx);
            points[i].y = OVL_Floor(xf->oy + y * xf->ky);
        }
        if (cmd->command == SDL_RENDERCMD_DRAW_POINTS) {
            if (blend == SDL_BLENDMODE_NONE) {
                SDL_DrawPoints(surface, points, count, SDL_MapRGBA(surface->format, r, g, b, a));
            } else if (blend_argb) {
                OVL_BlendPointsARGB(surface, points, count, r, g, b, a);
            } else {
                SDL_BlendPoints(surface, points, count, blend, r, g, b, a);
            }
        } else {
            if (blend == SDL_BLENDMODE_NONE) {
                SDL_DrawLines(surface, points, count, SDL_MapRGBA(surface->format, r, g, b, a));
            } else {
                SDL_BlendLines(surface, points, count, blend, r, g, b, a);
            }
        }
        break;
    }

    case SDL_RENDERCMD_FILL_RECTS: {
        SDL_FRect *frects = (SDL_FRect *)verts;
        SDL_Rect *rects = (SDL_Rect *)verts; /* converted in place */
        SDL_bool pixels = SDL_TRUE;

        for (i = 0; i < count; i++) {
            const SDL_FRect fr = frects[i];
            OVL_MapFRect(xf, &fr, &rects[i]);
            rects[i].w = SDL_max(rects[i].w, 1);
            rects[i].h = SDL_max(rects[i].h, 1);
            if (rects[i].w != 1 || rects[i].h != 1) {
                pixels = SDL_FALSE;
            }
        }
        if (pixels) {
            /* With a render scale, SDL draws lines and points as one rectangle
               per logical pixel: plot them as points */
            int *xy = (int *)verts; /* x, y, w, h -> x, y, converted in place */
            for (i = 0; i < count; i++) {
                xy[2 * i] = xy[4 * i];
                xy[2 * i + 1] = xy[4 * i + 1];
            }
            if (blend == SDL_BLENDMODE_NONE) {
                SDL_DrawPoints(surface, (SDL_Point *)verts, count, SDL_MapRGBA(surface->format, r, g, b, a));
            } else if (blend_argb) {
                OVL_BlendPointsARGB(surface, (SDL_Point *)verts, count, r, g, b, a);
            } else {
                SDL_BlendPoints(surface, (SDL_Point *)verts, count, blend, r, g, b, a);
            }
        } else if (blend == SDL_BLENDMODE_NONE) {
            SDL_FillRects(surface, rects, count, SDL_MapRGBA(surface->format, r, g, b, a));
        } else if (blend_argb) {
            OVL_BlendFillRectsARGB(surface, rects, count, r, g, b, a);
        } else {
            SDL_BlendFillRects(surface, rects, count, blend, r, g, b, a);
        }
        break;
    }

    case SDL_RENDERCMD_COPY: {
        const OVL_CopyData *copy = (const OVL_CopyData *)verts;
        SDL_Texture *texture = cmd->data.draw.texture;
        SDL_Rect dst;

        OVL_MapFRect(xf, &copy->dst, &dst);
        OVL_BlitCopy(OVL_PrepTextureForCopy(cmd), &copy->src, surface, &dst, texture->scaleMode, &data->scale_tmp);
        break;
    }

    case SDL_RENDERCMD_COPY_EX: {
        const OVL_CopyExData *copy = (const OVL_CopyExData *)verts;
        SDL_Texture *texture = cmd->data.draw.texture;
        SDL_FRect dst;
        SDL_FPoint center;

        /* Not rounded: rotated sprites move smoothly */
        dst.x = xf->ox + copy->dst.x * copy->scale_x * xf->kx;
        dst.y = xf->oy + copy->dst.y * copy->scale_y * xf->ky;
        dst.w = copy->dst.w * copy->scale_x * xf->kx;
        dst.h = copy->dst.h * copy->scale_y * xf->ky;
        center.x = copy->center.x * copy->scale_x * xf->kx;
        center.y = copy->center.y * copy->scale_y * xf->ky;
        OVL_DrawRotated(surface, OVL_GetSurface(texture), &copy->src, &dst, &center,
                        copy->angle, copy->flip, (texture->scaleMode != SDL_ScaleModeNearest) ? SDL_TRUE : SDL_FALSE,
                        blend, r, g, b, a);
        break;
    }

    case SDL_RENDERCMD_GEOMETRY: {
        SDL_Texture *texture = cmd->data.draw.texture;

        if (texture) {
            OVL_GeometryCopyQueued *queued = (OVL_GeometryCopyQueued *)verts;
            OVL_GeometryCopyData *ptr = (OVL_GeometryCopyData *)verts; /* converted in place */
            SDL_Surface *src;

            for (i = 0; i < count; i++) {
                const float x = queued[i].dst.x;
                const float y = queued[i].dst.y;
                ptr[i].dst.x = (int)(xf->ox + x * xf->kx);
                ptr[i].dst.y = (int)(xf->oy + y * xf->ky);
                trianglepoint_2_fixedpoint(&ptr[i].dst);
            }
            src = OVL_PrepTextureForCopy(cmd);
            for (i = 0; i + 2 < count; i += 3, ptr += 3) {
                SDL_SW_BlitTriangle(src,
                                    &(ptr[0].src), &(ptr[1].src), &(ptr[2].src),
                                    surface,
                                    &(ptr[0].dst), &(ptr[1].dst), &(ptr[2].dst),
                                    ptr[0].color, ptr[1].color, ptr[2].color);
            }
        } else {
            OVL_GeometryFillQueued *queued = (OVL_GeometryFillQueued *)verts;
            OVL_GeometryFillData *ptr = (OVL_GeometryFillData *)verts; /* converted in place */

            for (i = 0; i < count; i++) {
                const float x = queued[i].dst.x;
                const float y = queued[i].dst.y;
                ptr[i].dst.x = (int)(xf->ox + x * xf->kx);
                ptr[i].dst.y = (int)(xf->oy + y * xf->ky);
                trianglepoint_2_fixedpoint(&ptr[i].dst);
            }
            for (i = 0; i + 2 < count; i += 3, ptr += 3) {
                SDL_SW_FillTriangle(surface, &(ptr[0].dst), &(ptr[1].dst), &(ptr[2].dst),
                                    blend, ptr[0].color, ptr[1].color, ptr[2].color);
            }
        }
        break;
    }

    default:
        break;
    }
}

/* Draws what was kept out of the composition surface: the frame can't go
   straight to the overlay anymore. */
static void OVL_Materialize(OVL_RenderData *data)
{
    SDL_Surface *comp = data->comp;

    if (data->composed || !comp) {
        return;
    }
    if (data->clear_pending) {
        const Uint32 c = data->clear_color;
        SDL_SetClipRect(comp, NULL);
        SDL_FillRect(comp, NULL, SDL_MapRGBA(comp->format, (c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF, c >> 24));
        data->clear_pending = SDL_FALSE;
    }
    if (data->direct.texture) {
        SDL_Texture *texture = data->direct.texture;
        SDL_Surface *src = OVL_GetSurface(texture);

        data->direct.texture = NULL;
        SDL_SetSurfaceColorMod(src, 255, 255, 255);
        SDL_SetSurfaceAlphaMod(src, 255);
        SDL_SetSurfaceBlendMode(src, SDL_BLENDMODE_NONE);
        SDL_SetClipRect(comp, NULL);
        OVL_BlitCopy(src, &data->direct.src, comp, &data->direct.comp_dst, texture->scaleMode, &data->scale_tmp);
    }
    data->composed = SDL_TRUE;
}

/* Keeps an opaque copy that may be the only thing drawn in the frame out of
   the composition surface. */
static SDL_bool OVL_TryDirectCopy(OVL_RenderData *data, const SDL_RenderCommand *cmd, const OVL_CopyData *copy,
                                  const SDL_Rect *viewport, SDL_bool clipping, const OVL_Xform *xf)
{
    SDL_Texture *texture = cmd->data.draw.texture;
    const OVL_TextureData *td = (const OVL_TextureData *)texture->driverdata;
    const SDL_BlendMode blend = cmd->data.draw.blend;
    SDL_Rect dst;
    int out_w, out_h, x1, y1;

    if (data->composed || data->direct.texture || clipping || !viewport) {
        return SDL_FALSE;
    }
    if ((cmd->data.draw.r & cmd->data.draw.g & cmd->data.draw.b & cmd->data.draw.a) != 0xFF) {
        return SDL_FALSE;
    }
    if (!(blend == SDL_BLENDMODE_NONE || (blend == SDL_BLENDMODE_BLEND && !SDL_ISPIXELFORMAT_ALPHA(texture->format)))) {
        return SDL_FALSE;
    }
    if (!OVL_IsDirectFormat(texture->format) || SDL_MUSTLOCK(td->surface) ||
        copy->src.w < 1 || copy->src.h < 1) {
        return SDL_FALSE;
    }
    /* YUV overlays take chroma pairs, and 4:2:0 row pairs */
    if (td->yuv && (((copy->src.x | copy->src.w) & 1) ||
                    (OVL_IsPlanarYUV(texture->format) && ((copy->src.y | copy->src.h) & 1)))) {
        return SDL_FALSE;
    }

    dst.x = OVL_Round(viewport->x + copy->dst.x);
    dst.y = OVL_Round(viewport->y + copy->dst.y);
    x1 = OVL_Round(viewport->x + copy->dst.x + copy->dst.w);
    y1 = OVL_Round(viewport->y + copy->dst.y + copy->dst.h);
    dst.w = x1 - dst.x;
    dst.h = y1 - dst.y;

    /* The overlay can't be cropped by the viewport or the window */
    SDL_GetWindowSizeInPixels(data->window, &out_w, &out_h);
    if (dst.w < 1 || dst.h < 1 ||
        dst.x < SDL_max(viewport->x, 0) || dst.y < SDL_max(viewport->y, 0) ||
        x1 > SDL_min(viewport->x + viewport->w, out_w) || y1 > SDL_min(viewport->y + viewport->h, out_h)) {
        return SDL_FALSE;
    }

    data->direct.texture = texture;
    data->direct.src = copy->src;
    data->direct.dst = dst;
    OVL_MapFRect(xf, &copy->dst, &data->direct.comp_dst);
    return SDL_TRUE;
}

/* -------------------------------------------------------------------------
 * Renderer interface
 * ------------------------------------------------------------------------- */

/* Shows the frame on screen again at the window position: some drivers only
   move the overlay with a new frame. */
static void OVL_ShowAgain(OVL_RenderData *data, struct Window *win)
{
    OVL_SetGeometry(data, win, &data->shown);

    if (data->mode == OVL_MODE_DIRECT) {
        /* shown_* always match the overlay, see OVL_DestroyOverlay() */
        if (data->shown_texture) {
            if (OVL_UploadTexture(data, data->shown_texture, &data->shown_src)) {
                data->shown_version = ((const OVL_TextureData *)data->shown_texture->driverdata)->version;
            }
        } else if (data->shown_clear) {
            OVL_FillOverlay(data, data->shown_clear_color);
        }
    } else if (data->comp && !data->composed &&
               data->comp->w == data->src_w && data->comp->h == data->src_h) {
        /* Nothing drawn since the last present: comp still holds the frame */
        SDL_Rect all;
        all.x = all.y = 0;
        all.w = data->comp->w;
        all.h = data->comp->h;
        OVL_UploadRGB(data, data->comp, &all);
    }
}

static void OVL_WindowEvent(SDL_Renderer *renderer, const SDL_WindowEvent *event)
{
    OVL_RenderData *data = (OVL_RenderData *)renderer->driverdata;
    struct Window *win = OVL_GetIntuiWindow(data);

    /* The overlay keeps the last frame: it is shown again right away, an app
       drawing only on input events may not present before the next one */
    const SDL_bool shown = (win && data->vlayer && data->vlayer_win == win && !data->in_fallback) ? SDL_TRUE : SDL_FALSE;

    switch (event->event) {
    case SDL_WINDOWEVENT_SIZE_CHANGED:
    case SDL_WINDOWEVENT_RESIZED:
        data->bars_dirty = SDL_TRUE;
        break;
    case SDL_WINDOWEVENT_EXPOSED:
        data->bars_dirty = SDL_TRUE;
        if (shown) {
            OVL_PaintBars(data, win, &data->shown, SDL_TRUE);
        }
        break;
    case SDL_WINDOWEVENT_MOVED:
        D("[%s] window moved to %ld,%ld, overlay %s\n", __FUNCTION__, (long)event->data1, (long)event->data2,
          shown ? "shown again" : "not shown");
        data->win_left = data->win_top = -32768;
        if (shown) {
            OVL_ShowAgain(data, win);
        }
        break;
    default:
        break;
    }
}

static int OVL_GetOutputSize(SDL_Renderer *renderer, int *w, int *h)
{
    SDL_GetWindowSizeInPixels(renderer->window, w, h);
    return 0;
}

static int OVL_CreateTexture(SDL_Renderer *renderer, SDL_Texture *texture)
{
    OVL_TextureData *td = (OVL_TextureData *)SDL_calloc(1, sizeof(*td));
    Uint32 format = texture->format;

    if (!td) {
        return SDL_OutOfMemory();
    }
    if (SDL_ISPIXELFORMAT_FOURCC(format)) {
        td->yuv = SDL_SW_CreateYUVTexture(format, texture->w, texture->h);
        if (!td->yuv) {
            SDL_free(td);
            return -1;
        }
        /* Drawn from an RGB copy, see OVL_GetSurface(): RGB565 like the
           overlay, converted to it directly by SDL_ConvertPixels() */
        format = SDL_PIXELFORMAT_RGB565;
    }
    td->surface = SDL_CreateRGBSurfaceWithFormat(0, texture->w, texture->h, 0, format);
    if (!td->surface) {
        SDL_SW_DestroyYUVTexture(td->yuv);
        SDL_free(td);
        return -1;
    }
    td->version = 1;
    SDL_SetSurfaceColorMod(td->surface, texture->color.r, texture->color.g, texture->color.b);
    SDL_SetSurfaceAlphaMod(td->surface, texture->color.a);
    SDL_SetSurfaceBlendMode(td->surface, texture->blendMode);
    texture->driverdata = td;
    return 0;
}

/* The texture pixels are about to change */
static OVL_TextureData *OVL_TouchTexture(SDL_Renderer *renderer, SDL_Texture *texture)
{
    OVL_RenderData *data = (OVL_RenderData *)renderer->driverdata;
    OVL_TextureData *td = (OVL_TextureData *)texture->driverdata;

    /* The copy kept out of comp needs the pixels as they were */
    if (texture == data->direct.texture) {
        OVL_Materialize(data);
    }
    td->version++;
    return td;
}

/* Full update of a YUV 4:2:0 texture shown directly by a YCbCr420 overlay:
   each row is read once, written to the texture and to the overlay buffer,
   the next present only swaps. At video sizes, one pass less over memory.
   The texture stays complete for anything else. SDL_FALSE if not that case. */
static SDL_bool OVL_WriteThroughYUV(OVL_RenderData *data, SDL_Texture *texture,
                                    const Uint8 *Yplane, int Ypitch,
                                    const Uint8 *Uplane, int Upitch,
                                    const Uint8 *Vplane, int Vpitch)
{
    OVL_TextureData *td = (OVL_TextureData *)texture->driverdata;
    SDL_SW_YUVTexture *yuv = td->yuv;
    const int w = texture->w, h = texture->h;
    const SDL_bool iyuv = (texture->format == SDL_PIXELFORMAT_IYUV) ? SDL_TRUE : SDL_FALSE;
    Uint8 *ty, *tu, *tv;
    UBYTE *base, *ubase, *vbase;
    ULONG modulo, ypitch, cpitch;
    int y;
#ifdef __SDL_DEBUG
    Uint64 written;
#endif

    /* shown_src is the whole texture, the overlay its size, see OVL_PresentDirect() */
    if (!data->vlayer || data->mode != OVL_MODE_DIRECT || data->srcfmt != SRCFMT_YCbCr420 ||
        data->in_fallback || data->composed || texture != data->shown_texture ||
        data->shown_src.x || data->shown_src.y || data->shown_src.w != w || data->shown_src.h != h ||
        (!iyuv && texture->format != SDL_PIXELFORMAT_YV12)) {
        return SDL_FALSE;
    }
    base = OVL_LockOverlay(data, &modulo, (ULONG)w * 2);
    if (!base) {
        return SDL_FALSE;
    }
    ypitch = modulo / 2; /* see OVL_UploadYUV() */
    cpitch = ypitch / 2;
    ubase = base + ypitch * h;
    vbase = ubase + cpitch * (h / 2);
    ty = yuv->planes[0];
    tu = iyuv ? yuv->planes[1] : yuv->planes[2];
    tv = iyuv ? yuv->planes[2] : yuv->planes[1];

    /* The row just copied to the texture is in the cache for the overlay */
    for (y = 0; y < h; y++, Yplane += Ypitch, ty += yuv->pitches[0], base += ypitch) {
        SDL_memcpy(ty, Yplane, w);
        CopyMem((APTR)Yplane, base, (ULONG)w);
    }
    for (y = 0; y < h / 2; y++) {
        SDL_memcpy(tu, Uplane, w / 2);
        CopyMem((APTR)Uplane, ubase, (ULONG)(w / 2));
        SDL_memcpy(tv, Vplane, w / 2);
        CopyMem((APTR)Vplane, vbase, (ULONG)(w / 2));
        Uplane += Upitch;
        Vplane += Vpitch;
        tu += yuv->pitches[1];
        tv += yuv->pitches[1];
        ubase += cpitch;
        vbase += cpitch;
    }

#ifdef __SDL_DEBUG
    written = SDL_GetPerformanceCounter();
#endif
    UnlockVLayer(data->vlayer);
#ifdef __SDL_DEBUG
    data->pending_ticks[0] = data->upload_start - data->upload_lock;
    data->pending_ticks[1] = written - data->upload_start;
    data->pending_ticks[2] = SDL_GetPerformanceCounter() - written;
#endif
    data->pending_texture = texture;
    data->pending_version = td->version;
    return SDL_TRUE;
}

static SDL_bool OVL_IsWholeTexture(const SDL_Texture *texture, const SDL_Rect *rect)
{
    return (rect->x == 0 && rect->y == 0 && rect->w == texture->w && rect->h == texture->h) ? SDL_TRUE : SDL_FALSE;
}

static int OVL_UpdateTexture(SDL_Renderer *renderer, SDL_Texture *texture,
                             const SDL_Rect *rect, const void *pixels, int pitch)
{
    OVL_TextureData *td = OVL_TouchTexture(renderer, texture);
    SDL_Surface *surface = td->surface;
    const size_t length = (size_t)rect->w * surface->format->BytesPerPixel;
    const Uint8 *src = (const Uint8 *)pixels;
    Uint8 *dst;
    int row;

    if (td->yuv) {
        /* Planes one after the other, as SDL_SW_UpdateYUVTexture() takes them */
        if ((texture->format == SDL_PIXELFORMAT_IYUV || texture->format == SDL_PIXELFORMAT_YV12) &&
            OVL_IsWholeTexture(texture, rect)) {
            const int cpitch = (pitch + 1) / 2;
            const Uint8 *p1 = src + rect->h * pitch;
            const Uint8 *p2 = p1 + ((rect->h + 1) / 2) * cpitch;
            const SDL_bool iyuv = (texture->format == SDL_PIXELFORMAT_IYUV) ? SDL_TRUE : SDL_FALSE;
            if (OVL_WriteThroughYUV((OVL_RenderData *)renderer->driverdata, texture, src, pitch,
                                    iyuv ? p1 : p2, cpitch, iyuv ? p2 : p1, cpitch)) {
                return 0;
            }
        }
        return SDL_SW_UpdateYUVTexture(td->yuv, rect, pixels, pitch);
    }

    dst = (Uint8 *)surface->pixels + rect->y * surface->pitch + rect->x * surface->format->BytesPerPixel;
    if (rect->x == 0 && rect->w == surface->w && pitch == surface->pitch) {
        SDL_memcpy(dst, src, (size_t)pitch * (rect->h - 1) + length);
        return 0;
    }
    for (row = 0; row < rect->h; ++row) {
        SDL_memcpy(dst, src, length);
        src += pitch;
        dst += surface->pitch;
    }
    return 0;
}

static int OVL_UpdateTextureYUV(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_Rect *rect,
                                const Uint8 *Yplane, int Ypitch,
                                const Uint8 *Uplane, int Upitch,
                                const Uint8 *Vplane, int Vpitch)
{
    OVL_TextureData *td = OVL_TouchTexture(renderer, texture);

    if (OVL_IsWholeTexture(texture, rect) &&
        OVL_WriteThroughYUV((OVL_RenderData *)renderer->driverdata, texture, Yplane, Ypitch, Uplane, Upitch, Vplane, Vpitch)) {
        return 0;
    }
    return SDL_SW_UpdateYUVTexturePlanar(td->yuv, rect, Yplane, Ypitch, Uplane, Upitch, Vplane, Vpitch);
}

static int OVL_UpdateTextureNV(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_Rect *rect,
                               const Uint8 *Yplane, int Ypitch,
                               const Uint8 *UVplane, int UVpitch)
{
    OVL_TextureData *td = OVL_TouchTexture(renderer, texture);

    return SDL_SW_UpdateNVTexturePlanar(td->yuv, rect, Yplane, Ypitch, UVplane, UVpitch);
}

static int OVL_LockTexture(SDL_Renderer *renderer, SDL_Texture *texture,
                           const SDL_Rect *rect, void **pixels, int *pitch)
{
    OVL_TextureData *td = OVL_TouchTexture(renderer, texture);
    SDL_Surface *surface = td->surface;

    if (td->yuv) {
        return SDL_SW_LockYUVTexture(td->yuv, rect, pixels, pitch);
    }
    *pixels = (Uint8 *)surface->pixels + rect->y * surface->pitch + rect->x * surface->format->BytesPerPixel;
    *pitch = surface->pitch;
    return 0;
}

static void OVL_UnlockTexture(SDL_Renderer *renderer, SDL_Texture *texture)
{
}

static void OVL_SetTextureScaleMode(SDL_Renderer *renderer, SDL_Texture *texture, SDL_ScaleMode scaleMode)
{
}

static int OVL_SetRenderTarget(SDL_Renderer *renderer, SDL_Texture *texture)
{
    OVL_RenderData *data = (OVL_RenderData *)renderer->driverdata;

    if (texture && texture == data->direct.texture) {
        OVL_Materialize(data);
    }
    data->target_data = texture ? (OVL_TextureData *)texture->driverdata : NULL;
    data->target = texture ? data->target_data->surface : NULL;
    return 0;
}

static int OVL_QueueNoOp(SDL_Renderer *renderer, SDL_RenderCommand *cmd)
{
    return 0;
}

static int OVL_QueueDrawPoints(SDL_Renderer *renderer, SDL_RenderCommand *cmd, const SDL_FPoint *points, int count)
{
    SDL_FPoint *verts = (SDL_FPoint *)SDL_AllocateRenderVertices(renderer, count * sizeof(SDL_FPoint), 0, &cmd->data.draw.first);

    if (!verts) {
        return -1;
    }
    cmd->data.draw.count = count;
    SDL_memcpy(verts, points, count * sizeof(SDL_FPoint));
    return 0;
}

static int OVL_QueueFillRects(SDL_Renderer *renderer, SDL_RenderCommand *cmd, const SDL_FRect *rects, int count)
{
    SDL_FRect *verts = (SDL_FRect *)SDL_AllocateRenderVertices(renderer, count * sizeof(SDL_FRect), 0, &cmd->data.draw.first);

    if (!verts) {
        return -1;
    }
    cmd->data.draw.count = count;
    SDL_memcpy(verts, rects, count * sizeof(SDL_FRect));
    return 0;
}

static int OVL_QueueCopy(SDL_Renderer *renderer, SDL_RenderCommand *cmd, SDL_Texture *texture,
                         const SDL_Rect *srcrect, const SDL_FRect *dstrect)
{
    OVL_CopyData *verts = (OVL_CopyData *)SDL_AllocateRenderVertices(renderer, sizeof(OVL_CopyData), 0, &cmd->data.draw.first);

    if (!verts) {
        return -1;
    }
    cmd->data.draw.count = 1;
    verts->src = *srcrect;
    verts->dst = *dstrect;
    return 0;
}

static int OVL_QueueCopyEx(SDL_Renderer *renderer, SDL_RenderCommand *cmd, SDL_Texture *texture,
                           const SDL_Rect *srcrect, const SDL_FRect *dstrect,
                           const double angle, const SDL_FPoint *center, const SDL_RendererFlip flip, float scale_x, float scale_y)
{
    OVL_CopyExData *verts = (OVL_CopyExData *)SDL_AllocateRenderVertices(renderer, sizeof(OVL_CopyExData), 0, &cmd->data.draw.first);

    if (!verts) {
        return -1;
    }
    cmd->data.draw.count = 1;
    verts->src = *srcrect;
    verts->dst = *dstrect;
    verts->angle = angle;
    verts->center = *center;
    verts->flip = flip;
    verts->scale_x = scale_x;
    verts->scale_y = scale_y;
    return 0;
}

static int OVL_QueueGeometry(SDL_Renderer *renderer, SDL_RenderCommand *cmd, SDL_Texture *texture,
                             const float *xy, int xy_stride, const SDL_Color *color, int color_stride, const float *uv, int uv_stride,
                             int num_vertices, const void *indices, int num_indices, int size_indices,
                             float scale_x, float scale_y)
{
    const int count = indices ? num_indices : num_vertices;
    const size_t sz = texture ? sizeof(OVL_GeometryCopyQueued) : sizeof(OVL_GeometryFillQueued);
    void *verts = SDL_AllocateRenderVertices(renderer, count * sz, 0, &cmd->data.draw.first);
    int i;

    if (!verts) {
        return -1;
    }
    cmd->data.draw.count = count;
    size_indices = indices ? size_indices : 0;

    for (i = 0; i < count; i++) {
        const float *xy_;
        SDL_Color col_;
        int j;

        if (size_indices == 4) {
            j = ((const Uint32 *)indices)[i];
        } else if (size_indices == 2) {
            j = ((const Uint16 *)indices)[i];
        } else if (size_indices == 1) {
            j = ((const Uint8 *)indices)[i];
        } else {
            j = i;
        }

        xy_ = (const float *)((const char *)xy + j * xy_stride);
        col_ = *(const SDL_Color *)((const char *)color + j * color_stride);

        if (texture) {
            OVL_GeometryCopyQueued *ptr = (OVL_GeometryCopyQueued *)verts + i;
            const float *uv_ = (const float *)((const char *)uv + j * uv_stride);
            ptr->src.x = (int)(uv_[0] * texture->w);
            ptr->src.y = (int)(uv_[1] * texture->h);
            ptr->dst.x = xy_[0] * scale_x;
            ptr->dst.y = xy_[1] * scale_y;
            ptr->color = col_;
        } else {
            OVL_GeometryFillQueued *ptr = (OVL_GeometryFillQueued *)verts + i;
            ptr->dst.x = xy_[0] * scale_x;
            ptr->dst.y = xy_[1] * scale_y;
            ptr->color = col_;
        }
    }
    return 0;
}

static int OVL_RunCommandQueue(SDL_Renderer *renderer, SDL_RenderCommand *cmd, void *vertices, size_t vertsize)
{
    OVL_RenderData *data = (OVL_RenderData *)renderer->driverdata;
    const SDL_bool to_comp = data->target ? SDL_FALSE : SDL_TRUE;
    SDL_Surface *surface = to_comp ? OVL_UpdateComposition(renderer, data) : data->target;
    const SDL_Rect *viewport = NULL;
    const SDL_Rect *cliprect = NULL;
    SDL_bool xform_dirty = SDL_TRUE;
    OVL_Xform xf;

    if (!surface) {
        return -1;
    }
    /* Drawn into, the target texture changes */
    if (!to_comp) {
        data->target_data->version++;
    }

    for (; cmd; cmd = cmd->next) {
        switch (cmd->command) {
        case SDL_RENDERCMD_SETVIEWPORT:
            viewport = &cmd->data.viewport.rect;
            xform_dirty = SDL_TRUE;
            break;

        case SDL_RENDERCMD_SETCLIPRECT:
            cliprect = cmd->data.cliprect.enabled ? &cmd->data.cliprect.rect : NULL;
            xform_dirty = SDL_TRUE;
            break;

        case SDL_RENDERCMD_CLEAR: {
            const Uint8 r = cmd->data.color.r;
            const Uint8 g = cmd->data.color.g;
            const Uint8 b = cmd->data.color.b;
            const Uint8 a = cmd->data.color.a;

            if (to_comp) {
                /* Applied only if something else than one opaque copy gets drawn */
                data->clear_color = ((Uint32)a << 24) | ((Uint32)r << 16) | ((Uint32)g << 8) | b;
                data->clear_pending = SDL_TRUE;
                data->composed = SDL_FALSE;
                data->direct.texture = NULL;
            } else {
                /* By definition the clear ignores the clip rect */
                SDL_SetClipRect(surface, NULL);
                SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, r, g, b, a));
            }
            break;
        }

        case SDL_RENDERCMD_DRAW_POINTS:
        case SDL_RENDERCMD_DRAW_LINES:
        case SDL_RENDERCMD_FILL_RECTS:
        case SDL_RENDERCMD_COPY:
        case SDL_RENDERCMD_COPY_EX:
        case SDL_RENDERCMD_GEOMETRY:
            if (xform_dirty) {
                OVL_SetupXform(data, surface, to_comp, viewport, cliprect, &xf);
                xform_dirty = SDL_FALSE;
            }
            if (to_comp) {
                if (cmd->command == SDL_RENDERCMD_COPY &&
                    OVL_TryDirectCopy(data, cmd, (const OVL_CopyData *)((Uint8 *)vertices + cmd->data.draw.first),
                                      viewport, cliprect ? SDL_TRUE : SDL_FALSE, &xf)) {
                    break;
                }
                OVL_Materialize(data);
            }
            SDL_SetClipRect(surface, &xf.clip);
            OVL_Draw(data, surface, &xf, cmd, vertices);
            break;

        default:
            break;
        }
    }
    return 0;
}

static int OVL_RenderReadPixels(SDL_Renderer *renderer, const SDL_Rect *rect,
                                Uint32 format, void *pixels, int pitch)
{
    OVL_RenderData *data = (OVL_RenderData *)renderer->driverdata;
    SDL_Surface *comp;
    Uint32 *tmp;
    int x, y, retval;

    if (data->target) {
        SDL_Surface *surface = data->target;
        if (rect->x < 0 || rect->x + rect->w > surface->w ||
            rect->y < 0 || rect->y + rect->h > surface->h) {
            return SDL_SetError("Tried to read outside of surface bounds");
        }
        return SDL_ConvertPixels(rect->w, rect->h, surface->format->format,
                                 (Uint8 *)surface->pixels + rect->y * surface->pitch + rect->x * surface->format->BytesPerPixel,
                                 surface->pitch, format, pixels, pitch);
    }

    comp = data->comp ? data->comp : OVL_UpdateComposition(renderer, data);
    if (!comp) {
        return -1;
    }
    OVL_Materialize(data);

    /* The window resolution is read from the composition surface */
    tmp = (Uint32 *)SDL_malloc((size_t)rect->w * rect->h * sizeof(Uint32));
    if (!tmp) {
        return SDL_OutOfMemory();
    }
    for (y = 0; y < rect->h; y++) {
        const int cy = OVL_Floor((rect->y + y + 0.5f - data->area.y) * data->ky);
        for (x = 0; x < rect->w; x++) {
            const int cx = OVL_Floor((rect->x + x + 0.5f - data->area.x) * data->kx);
            Uint32 p = 0xFF000000 | data->clear_color;
            if (cx >= 0 && cy >= 0 && cx < comp->w && cy < comp->h &&
                rect->x + x >= data->area.x && rect->y + y >= data->area.y) {
                p = ((const Uint32 *)((const Uint8 *)comp->pixels + cy * comp->pitch))[cx];
            }
            tmp[y * rect->w + x] = p;
        }
    }
    retval = SDL_ConvertPixels(rect->w, rect->h, SDL_PIXELFORMAT_ARGB8888, tmp, rect->w * sizeof(Uint32),
                               format, pixels, pitch);
    SDL_free(tmp);
    return retval;
}

static SDL_bool OVL_PresentDirect(OVL_RenderData *data, struct Window *win)
{
    SDL_Texture *texture = data->direct.texture;
    const OVL_TextureData *td = (const OVL_TextureData *)texture->driverdata;
    const SDL_Rect *src = &data->direct.src;
    const SDL_bool filter = (texture->scaleMode != SDL_ScaleModeNearest) ? SDL_TRUE : SDL_FALSE;
    SDL_bool wait_switch = (data->vlayer && data->mode == OVL_MODE_COMPOSE) ? SDL_TRUE : SDL_FALSE;
    ULONG srcfmt = OVL_OverlayFormat(data, win, texture->format);

    while (!OVL_SetupOverlay(data, win, OVL_MODE_DIRECT, srcfmt, src->w, src->h, filter, wait_switch)) {
        /* That YUV format just failed: the next one right away */
        const ULONG next = OVL_OverlayFormat(data, win, texture->format);
        if (next == srcfmt) {
            return SDL_FALSE;
        }
        srcfmt = next;
        wait_switch = SDL_FALSE;
    }
    OVL_SetGeometry(data, win, &data->direct.dst);
    OVL_PaintBars(data, win, &data->direct.dst, SDL_TRUE);

    /* Same pixels as the frame on screen: the overlay keeps showing it */
    if (texture == data->shown_texture && td->version == data->shown_version &&
        SDL_memcmp(src, &data->shown_src, sizeof(*src)) == 0 && !data->in_fallback) {
        if (data->vsync) {
            WaitTOF();
        }
        return SDL_TRUE;
    }
    if (texture == data->pending_texture && td->version == data->pending_version &&
        SDL_memcmp(src, &data->shown_src, sizeof(*src)) == 0) {
        /* Already written when updated, see OVL_WriteThroughYUV() */
        OVL_SwapOverlay(data, data->pending_ticks[0], data->pending_ticks[1], data->pending_ticks[2]);
    } else if (!OVL_UploadTexture(data, texture, src)) {
        return SDL_FALSE;
    }
    data->shown_texture = texture;
    data->shown_src = *src;
    data->shown_version = td->version;
    data->shown_clear = SDL_FALSE;
    return SDL_TRUE;
}

static void OVL_PresentComposed(OVL_RenderData *data, struct Window *win)
{
    SDL_Surface *comp = data->comp;
    SDL_Rect all;

    all.x = all.y = 0;
    all.w = comp->w;
    all.h = comp->h;

    if (OVL_SetupOverlay(data, win, OVL_MODE_COMPOSE, SRCFMT_RGB16, comp->w, comp->h, data->compose_filter, SDL_FALSE)) {
        OVL_SetGeometry(data, win, &data->area);
        OVL_PaintBars(data, win, &data->area, SDL_TRUE);
        if (OVL_UploadRGB(data, comp, &all)) {
            data->shown_texture = NULL;
            data->shown_clear = SDL_FALSE;
            return;
        }
    }

    /* No overlay for this screen: draw into the window */
    if (!data->in_fallback) {
        D("[%s] no overlay, drawing %ldx%ld into the window\n", __FUNCTION__, (long)comp->w, (long)comp->h);
        data->in_fallback = SDL_TRUE;
        data->bars_dirty = SDL_TRUE;
    }
    OVL_PaintBars(data, win, &data->area, SDL_FALSE);
    if (win->RPort) {
        if (comp->w == data->area.w && comp->h == data->area.h) {
            WritePixelArray(comp->pixels, 0, 0, comp->pitch, win->RPort,
                            win->BorderLeft + data->area.x, win->BorderTop + data->area.y,
                            comp->w, comp->h, RECTFMT_ARGB);
        } else {
            ScalePixelArray(comp->pixels, comp->w, comp->h, comp->pitch, win->RPort,
                            win->BorderLeft + data->area.x, win->BorderTop + data->area.y,
                            data->area.w, data->area.h, RECTFMT_ARGB);
        }
    }
    if (data->vsync) {
        WaitTOF();
    }
}

static int OVL_RenderPresent(SDL_Renderer *renderer)
{
    OVL_RenderData *data = (OVL_RenderData *)renderer->driverdata;
    struct Window *win = OVL_GetIntuiWindow(data);
    int retval = 0;

    if (!win) {
        retval = -1;
    } else if (data->clear_pending && !data->direct.texture && !data->composed && OVL_PresentClear(data, win)) {
        /* nothing else drawn */
    } else if (!(data->direct.texture && !data->composed && OVL_PresentDirect(data, win))) {
        if (!data->direct.texture || data->composed) {
            data->switch_count = 0;
        }
        if (!data->comp) {
            OVL_UpdateComposition(renderer, data);
        }
        OVL_Materialize(data);
        if (data->comp) {
            OVL_PresentComposed(data, win);
        } else {
            retval = -1;
        }
    }

    data->composed = SDL_FALSE;
    data->clear_pending = SDL_FALSE;
    data->direct.texture = NULL;
    data->comp_sized = SDL_FALSE;
    return retval;
}

static void OVL_DestroyTexture(SDL_Renderer *renderer, SDL_Texture *texture)
{
    OVL_RenderData *data = (OVL_RenderData *)renderer->driverdata;
    OVL_TextureData *td = (OVL_TextureData *)texture->driverdata;

    if (texture == data->direct.texture) {
        OVL_Materialize(data);
    }
    if (texture == data->shown_texture) {
        data->shown_texture = NULL;
    }
    if (texture == data->pending_texture) {
        data->pending_texture = NULL;
    }
    if (td) {
        SDL_FreeSurface(td->surface);
        SDL_SW_DestroyYUVTexture(td->yuv);
        SDL_free(td);
    }
    texture->driverdata = NULL;
}

static void OVL_DestroyRenderer(SDL_Renderer *renderer)
{
    OVL_RenderData *data = (OVL_RenderData *)renderer->driverdata;
    SDL_WindowData *wd;

    if (!data) {
        return;
    }
    wd = (SDL_WindowData *)data->window->driverdata;
    if (wd && wd->overlay_userdata == data) {
        wd->overlay_closing = NULL;
        wd->overlay_userdata = NULL;
    }
    OVL_DestroyOverlay(data);
    SDL_FreeSurface(data->comp);
    SDL_FreeSurface(data->scale_tmp);
    SDL_SIMDFree(data->row_buf);
    SDL_free(data);
    renderer->driverdata = NULL;
    OVL_CloseCGXVideo();
}

static int OVL_SetVSync(SDL_Renderer *renderer, const int vsync)
{
    OVL_RenderData *data = (OVL_RenderData *)renderer->driverdata;

    data->vsync = vsync ? SDL_TRUE : SDL_FALSE;
    if (vsync) {
        renderer->info.flags |= SDL_RENDERER_PRESENTVSYNC;
    } else {
        renderer->info.flags &= ~SDL_RENDERER_PRESENTVSYNC;
    }
    return 0;
}

static int OVL_CreateRenderer(SDL_Renderer *renderer, SDL_Window *window, Uint32 flags)
{
    SDL_WindowData *wd = (SDL_WindowData *)window->driverdata;
    const char *driver = SDL_GetCurrentVideoDriver();
    const char *hint;
    OVL_RenderData *data;

    if (!driver || SDL_strcmp(driver, "mos") != 0 || !wd) {
        return SDL_SetError("The overlay renderer needs the MorphOS video driver");
    }
    if (wd->overlay_closing) {
        return SDL_SetError("The window already has an overlay renderer");
    }
    if (!OVL_OpenCGXVideo()) {
        D("[%s] no cgxvideo.library 43+\n", __FUNCTION__);
        return SDL_SetError("Couldn't open cgxvideo.library 43+");
    }

    /* Refuse screens without overlay now, SDL then falls back to another renderer */
    if (wd->win) {
        const ULONG features = OVL_Query(wd->win->WScreen, VSQ_SupportedFeatures);
        const ULONG formats = OVL_Query(wd->win->WScreen, VSQ_SupportedFormats);
        D("[%s] screen 0x%08lx: features 0x%08lx, formats 0x%08lx, max width %ld\n", __FUNCTION__,
          (unsigned long)wd->win->WScreen, (unsigned long)features, (unsigned long)formats,
          (long)OVL_Query(wd->win->WScreen, VSQ_MaxWidth));
        if ((features && !(features & VSQ_FEAT_OVERLAY)) || (formats && !(formats & VSQ_FMT_R5G6B5_LE))) {
            OVL_CloseCGXVideo();
            return SDL_SetError("No 16-bit RGB overlay on this screen");
        }
    }

    data = (OVL_RenderData *)SDL_calloc(1, sizeof(*data));
    if (!data) {
        OVL_CloseCGXVideo();
        return SDL_OutOfMemory();
    }
    data->window = window;
    data->clear_color = 0xFF000000;
    data->bars_dirty = SDL_TRUE;
    data->win_left = data->win_top = -32768;
    data->vsync = (flags & SDL_RENDERER_PRESENTVSYNC) ? SDL_TRUE : SDL_FALSE;
    data->altivec = SDL_HasAltiVec();
    /* Nearest by default, like textures */
    hint = SDL_GetHint(SDL_HINT_RENDER_SCALE_QUALITY);
    data->compose_filter = (hint && (*hint == '1' || *hint == '2' ||
                                     SDL_strcasecmp(hint, "linear") == 0 || SDL_strcasecmp(hint, "best") == 0)) ? SDL_TRUE : SDL_FALSE;

    renderer->WindowEvent = OVL_WindowEvent;
    renderer->GetOutputSize = OVL_GetOutputSize;
    renderer->CreateTexture = OVL_CreateTexture;
    renderer->UpdateTexture = OVL_UpdateTexture;
    renderer->UpdateTextureYUV = OVL_UpdateTextureYUV;
    renderer->UpdateTextureNV = OVL_UpdateTextureNV;
    renderer->LockTexture = OVL_LockTexture;
    renderer->UnlockTexture = OVL_UnlockTexture;
    renderer->SetTextureScaleMode = OVL_SetTextureScaleMode;
    renderer->SetRenderTarget = OVL_SetRenderTarget;
    renderer->QueueSetViewport = OVL_QueueNoOp;
    renderer->QueueSetDrawColor = OVL_QueueNoOp;
    renderer->QueueDrawPoints = OVL_QueueDrawPoints;
    renderer->QueueDrawLines = OVL_QueueDrawPoints;
    renderer->QueueFillRects = OVL_QueueFillRects;
    renderer->QueueCopy = OVL_QueueCopy;
    renderer->QueueCopyEx = OVL_QueueCopyEx;
    renderer->QueueGeometry = OVL_QueueGeometry;
    renderer->RunCommandQueue = OVL_RunCommandQueue;
    renderer->RenderReadPixels = OVL_RenderReadPixels;
    renderer->RenderPresent = OVL_RenderPresent;
    renderer->DestroyTexture = OVL_DestroyTexture;
    renderer->DestroyRenderer = OVL_DestroyRenderer;
    renderer->SetVSync = OVL_SetVSync;
    renderer->info = MOS_OVERLAY_RenderDriver.info;
    if (!data->vsync) {
        renderer->info.flags &= ~SDL_RENDERER_PRESENTVSYNC;
    }
    renderer->driverdata = data;

    renderer->always_batch = SDL_TRUE;

    wd->overlay_closing = OVL_WindowClosing;
    wd->overlay_userdata = data;

    D("[%s] cgxvideo.library %ld.%ld, window %ldx%ld, vsync %ld, composed filter %ld, altivec %ld\n", __FUNCTION__,
      (long)CGXVideoBase->lib_Version, (long)CGXVideoBase->lib_Revision,
      (long)window->w, (long)window->h, (long)data->vsync, (long)data->compose_filter, (long)data->altivec);
    return 0;
}

SDL_RenderDriver MOS_OVERLAY_RenderDriver = {
    OVL_CreateRenderer,
    {
     "overlay",
     SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE,
     15,
     {
      SDL_PIXELFORMAT_ARGB8888,
      SDL_PIXELFORMAT_ABGR8888,
      SDL_PIXELFORMAT_RGBA8888,
      SDL_PIXELFORMAT_BGRA8888,
      SDL_PIXELFORMAT_RGB888,
      SDL_PIXELFORMAT_BGR888,
      SDL_PIXELFORMAT_RGB565,
      SDL_PIXELFORMAT_RGB555,
      SDL_PIXELFORMAT_YV12,
      SDL_PIXELFORMAT_IYUV,
      SDL_PIXELFORMAT_NV12,
      SDL_PIXELFORMAT_NV21,
      SDL_PIXELFORMAT_YUY2,
      SDL_PIXELFORMAT_UYVY,
      SDL_PIXELFORMAT_YVYU
     },
     0,
     0}
};

#endif /* SDL_VIDEO_RENDER_MOS_OVERLAY */
