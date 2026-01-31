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

#ifndef _SDL_mosevents_h
#define _SDL_mosevents_h

extern void MOS_PumpEvents(SDL_VideoDevice *_this);
extern void MOS_GlobalMenu(struct Menu *mymenu, UWORD menu, UWORD item, UWORD sub, UWORD check);
extern void MOS_UpdatePointerIfNeeded(SDL_VideoData *vd, SDL_WindowData *wdata);
extern void MOS_ClearPointerIfApplied(void);

extern void MOS_FocusAndWarpIfNeeded(SDL_VideoDevice *_this, SDL_WindowData *data);
extern void MOS_InvalidatePointerCacheForWindow(struct Window *win);
#endif /* _SDL_mosevents_h */
