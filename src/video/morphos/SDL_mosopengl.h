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

/* TinyGL implementation of SDL OpenGL support */

#include "SDL_internal.h"

#ifndef _SDL_mosopengl_h
#define _SDL_mosopengl_h

/* SDL functions */
extern bool MOS_GL_LoadLibrary(SDL_VideoDevice *_this, const char *path);
extern SDL_FunctionPointer MOS_GL_GetProcAddress(SDL_VideoDevice *_this, const char *proc);
extern void MOS_GL_UnloadLibrary(SDL_VideoDevice *_this);
extern SDL_GLContext MOS_GL_CreateContext(SDL_VideoDevice *_this, SDL_Window * window);
extern bool MOS_GL_MakeCurrent(SDL_VideoDevice *_this, SDL_Window * window, SDL_GLContext context);
extern bool MOS_GL_SetSwapInterval(SDL_VideoDevice *_this, int interval);
extern bool MOS_GL_GetSwapInterval(SDL_VideoDevice *_this, int *interval);
extern bool MOS_GL_SwapWindow(SDL_VideoDevice *_this, SDL_Window * window);
extern bool MOS_GL_DestroyContext(SDL_VideoDevice *_this, SDL_GLContext context);
/* Non-SDL functions */
extern bool MOS_GL_ResizeContext(SDL_VideoDevice *_this, SDL_Window *window);

#endif /* _SDL_mosopengl_h */
