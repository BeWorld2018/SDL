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

#ifdef SDL_VIDEO_RENDER_MOS_OVERLAY

/* AltiVec conversion to the overlay format, RGB565 little endian, 8 pixels
   per 16-byte store. Compiled on its own with -maltivec, see Makefile.mos:
   nothing here runs without SDL_HasAltiVec(). */

#include <altivec.h>

#include "SDL_render_overlay_altivec.h"

typedef vector unsigned char OVL_VU8;
typedef vector unsigned int OVL_VU32;

bool OVL_IsAltiVecFormat(SDL_PixelFormat format)
{
    switch (format) {
    case SDL_PIXELFORMAT_ARGB8888:
    case SDL_PIXELFORMAT_XRGB8888:
    case SDL_PIXELFORMAT_ABGR8888:
    case SDL_PIXELFORMAT_XBGR8888:
    case SDL_PIXELFORMAT_RGBA8888:
    case SDL_PIXELFORMAT_RGBX8888:
    case SDL_PIXELFORMAT_BGRA8888:
    case SDL_PIXELFORMAT_BGRX8888:
    case SDL_PIXELFORMAT_RGB565:
        return true;
    default:
        return false;
    }
}

/* Byte order putting the four pixels of a vector in A R G B order */
static OVL_VU8 OVL_OrderARGB(SDL_PixelFormat format)
{
    switch (format) {
    case SDL_PIXELFORMAT_ABGR8888:
    case SDL_PIXELFORMAT_XBGR8888:
        return (OVL_VU8){ 0, 3, 2, 1, 4, 7, 6, 5, 8, 11, 10, 9, 12, 15, 14, 13 };
    case SDL_PIXELFORMAT_RGBA8888:
    case SDL_PIXELFORMAT_RGBX8888:
        return (OVL_VU8){ 3, 0, 1, 2, 7, 4, 5, 6, 11, 8, 9, 10, 15, 12, 13, 14 };
    case SDL_PIXELFORMAT_BGRA8888:
    case SDL_PIXELFORMAT_BGRX8888:
        return (OVL_VU8){ 3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12 };
    default:
        return (OVL_VU8){ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    }
}

/* Same for one pixel */
static SDL_INLINE Uint32 OVL_ToARGB(SDL_PixelFormat format, Uint32 p)
{
    switch (format) {
    case SDL_PIXELFORMAT_ABGR8888:
    case SDL_PIXELFORMAT_XBGR8888:
        return (p & 0xFF00FF00) | ((p >> 16) & 0xFF) | ((p & 0xFF) << 16);
    case SDL_PIXELFORMAT_RGBA8888:
    case SDL_PIXELFORMAT_RGBX8888:
        return (p >> 8) | (p << 24);
    case SDL_PIXELFORMAT_BGRA8888:
    case SDL_PIXELFORMAT_BGRX8888:
        return SDL_Swap32(p);
    default:
        return p;
    }
}

/* RGB565 little endian of an A R G B pixel, as OVL_ConvertRow() */
static SDL_INLINE Uint16 OVL_PackARGB(Uint32 p)
{
    return SDL_Swap16LE((Uint16)(((p >> 8) & 0xF800) | ((p >> 5) & 0x07E0) | ((p >> 3) & 0x001F)));
}

/* Unaligned loads: vec_ld(15, src) and vec_ld(31, src) only reach the
   16-byte blocks holding the last bytes used, never past the row. vec_lvsl()
   stays the same along the row, src moving by whole vectors. */

static void OVL_ConvertRow565(const Uint8 *src, Uint8 *dst, int w)
{
    const OVL_VU8 swap = { 1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14 };
    OVL_VU8 ctl;

    for (; w > 0 && ((uintptr_t)dst & 15); w--, src += 2, dst += 2) {
        *(Uint16 *)dst = SDL_Swap16LE(*(const Uint16 *)src);
    }
    ctl = vec_perm(vec_lvsl(0, src), vec_lvsl(0, src), swap);
    for (; w >= 8; w -= 8, src += 16, dst += 16) {
        vec_st(vec_perm(vec_ld(0, src), vec_ld(15, src), ctl), 0, dst);
    }
    for (; w > 0; w--, src += 2, dst += 2) {
        *(Uint16 *)dst = SDL_Swap16LE(*(const Uint16 *)src);
    }
}

static void OVL_ConvertRow32(SDL_PixelFormat format, const Uint8 *src, Uint8 *dst, int w)
{
    /* Low halves of the eight 32-bit results, bytes swapped */
    const OVL_VU8 pack = { 3, 2, 7, 6, 11, 10, 15, 14, 19, 18, 23, 22, 27, 26, 31, 30 };
    const OVL_VU8 order = OVL_OrderARGB(format);
    const OVL_VU32 s8 = vec_splats(8u), s5 = vec_splats(5u), s3 = vec_splats(3u);
    const OVL_VU32 mr = vec_splats(0xF800u), mg = vec_splats(0x07E0u), mb = vec_splats(0x001Fu);
    OVL_VU8 ctl;

    for (; w > 0 && ((uintptr_t)dst & 15); w--, src += 4, dst += 2) {
        *(Uint16 *)dst = OVL_PackARGB(OVL_ToARGB(format, *(const Uint32 *)src));
    }
    ctl = vec_perm(vec_lvsl(0, src), vec_lvsl(0, src), order);
    for (; w >= 8; w -= 8, src += 32, dst += 16) {
        const OVL_VU32 p0 = (OVL_VU32)vec_perm(vec_ld(0, src), vec_ld(15, src), ctl);
        const OVL_VU32 p1 = (OVL_VU32)vec_perm(vec_ld(16, src), vec_ld(31, src), ctl);
        const OVL_VU32 c0 = vec_or(vec_or(vec_and(vec_sr(p0, s8), mr), vec_and(vec_sr(p0, s5), mg)),
                                   vec_and(vec_sr(p0, s3), mb));
        const OVL_VU32 c1 = vec_or(vec_or(vec_and(vec_sr(p1, s8), mr), vec_and(vec_sr(p1, s5), mg)),
                                   vec_and(vec_sr(p1, s3), mb));
        vec_st(vec_perm((OVL_VU8)c0, (OVL_VU8)c1, pack), 0, dst);
    }
    for (; w > 0; w--, src += 4, dst += 2) {
        *(Uint16 *)dst = OVL_PackARGB(OVL_ToARGB(format, *(const Uint32 *)src));
    }
}

void OVL_ConvertRowAltiVec(SDL_PixelFormat format, const Uint8 *src, Uint8 *dst, int w)
{
    if (format == SDL_PIXELFORMAT_RGB565) {
        OVL_ConvertRow565(src, dst, w);
    } else {
        OVL_ConvertRow32(format, src, dst, w);
    }
}

#endif /* SDL_VIDEO_RENDER_MOS_OVERLAY */

/* vi: set ts=4 sw=4 expandtab: */
