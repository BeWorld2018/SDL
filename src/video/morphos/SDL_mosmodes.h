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

#ifndef _SDL_mosmodes_h
#define _SDL_mosmodes_h

#include "../SDL_sysvideo.h"

#include <exec/types.h>

struct SDL_DisplayData
{
	struct Screen *	screen;
	APTR 			monitor;
};

struct SDL_DisplayModeData
{
    ULONG					modeid;
    LONG					x;
    LONG					y;
};

/* SDL functions */
extern bool MOS_GetDisplayModes(SDL_VideoDevice *device, SDL_VideoDisplay * sdl_display);
extern bool MOS_SetDisplayMode(SDL_VideoDevice *device, SDL_VideoDisplay *display, SDL_DisplayMode *mode);
extern bool MOS_GetDisplayBounds(SDL_VideoDevice *device, SDL_VideoDisplay * display, SDL_Rect * rect);

/* Non-SDL functions */
extern bool MOS_InitModes(SDL_VideoDevice *device);
extern void MOS_CloseDisplay(SDL_VideoDevice *_this);
extern void MOS_CloseScreen(SDL_VideoDevice *_this);

#endif /* _SDL_mosmodes_h */
