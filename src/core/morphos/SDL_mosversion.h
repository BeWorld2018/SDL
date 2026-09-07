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

#ifndef SDL_CORE_MORPHOS_MOSVERSION_H
#define SDL_CORE_MORPHOS_MOSVERSION_H

/* single source of truth for the version numbers */
#include <SDL3/SDL_version.h>

#define MOS_STR(s)  #s
#define MOS_XSTR(s) MOS_STR(s)

#define VERSION   SDL_MAJOR_VERSION
#define REVISION  SDL_MINOR_VERSION

#define VERSTAG  "\0$VER: sdl3.library " \
                 MOS_XSTR(SDL_MAJOR_VERSION) "." MOS_XSTR(SDL_MINOR_VERSION) "." MOS_XSTR(SDL_MICRO_VERSION) \
                 " (" __AMIGADATE__ ") " \
                 "\xa9" "2026 BeWorld"

#endif /* SDL_CORE_MORPHOS_MOSVERSION_H */
