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

#ifndef SDL_render_overlay_altivec_h_
#define SDL_render_overlay_altivec_h_

#include "SDL_internal.h"

/* AltiVec part of the overlay renderer, compiled on its own with -maltivec:
   only called when SDL_HasAltiVec(). */

/* Formats OVL_ConvertRowAltiVec() takes */
extern bool OVL_IsAltiVecFormat(SDL_PixelFormat format);

/* Converts w pixels of src to RGB565 little endian at dst, any alignment:
   16-byte stores, so dst can be the overlay itself */
extern void OVL_ConvertRowAltiVec(SDL_PixelFormat format, const Uint8 *src, Uint8 *dst, int w);

#endif /* SDL_render_overlay_altivec_h_ */

/* vi: set ts=4 sw=4 expandtab: */
