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

#ifndef _SDL_moswindow_h
#define _SDL_moswindow_h

#ifndef EXEC_NODES_H
#include <exec/nodes.h>
#endif

#include <tgl/gl.h>
#include <proto/gadtools.h>

enum
{
	MID_PROJECT = 0x0010,
	MID_ABOUT,
	MID_ABOUTSYS,
	MID_HIDE,
	MID_QUIT,
	MID_OPTIONS,
	MID_MUTE,
	MID_PRIORITY,
	MID_RRENDER, MID_RRAUTO, MID_RRGL, MID_RRSOFT,
	MID_RVSYNC, MID_RVAUTO, MID_RVENABLE, MID_RVDISABLE
};

typedef struct
{
	Uint32 kludge1, kludge2, bpr, pixfmt;
	Uint8 buffer[0];
} SDL_Framebuffer;

struct SDL_WindowData
{
	struct MinNode node;

	SDL_Framebuffer *fb;
	struct Region *region;
	
	SDL_Window 			*window;
	struct Window 		*win;
	struct AppWindow 	*appWin;
	struct AppIcon 		*appIcon;
	
	// Localized window title, use SDL_free() to deallocate
	char *window_title;

	struct SDL_VideoData *videodata;

	GLContext *__tglContext;
	
	// Currently known window position and dimensions
	int old_w, old_h, old_x, old_y;

	BOOL grabbed;
	BYTE first_deltamove;
	
	APTR 				menuvisualinfo;
	struct Menu 		*menu;
	
	struct BitMap *bitmap;
	
	BOOL wasMaximized; /* Remember state when going to fullscreen mode, or back */
};

/* SDL functions */
extern bool MOS_CreateWindow(SDL_VideoDevice *_this, SDL_Window * window, SDL_PropertiesID create_props);
extern void MOS_SetWindowTitle(SDL_VideoDevice *_this, SDL_Window * window);
extern bool MOS_SetWindowIcon(SDL_VideoDevice *_this, SDL_Window * window, SDL_Surface * icon);
extern bool MOS_SetWindowPosition(SDL_VideoDevice *_this, SDL_Window * window);
extern void MOS_SetWindowMinMaxSize(SDL_VideoDevice *_this, SDL_Window * window);
extern void MOS_SetWindowSize(SDL_VideoDevice *_this, SDL_Window * window);
extern void MOS_ShowWindow(SDL_VideoDevice *_this, SDL_Window * window);
extern void MOS_HideWindow(SDL_VideoDevice *_this, SDL_Window * window);
extern void MOS_RaiseWindow(SDL_VideoDevice *_this, SDL_Window * window);
extern void MOS_MaximizeWindow(SDL_VideoDevice *_this, SDL_Window * window);
extern void MOS_MinimizeWindow(SDL_VideoDevice *_this, SDL_Window * window);
extern void MOS_RestoreWindow(SDL_VideoDevice *_this, SDL_Window * window);
extern void MOS_SetWindowBordered(SDL_VideoDevice *_this, SDL_Window * window, bool bordered);
extern SDL_FullscreenResult MOS_SetWindowFullscreen(SDL_VideoDevice *_this, SDL_Window * window, SDL_VideoDisplay * display, SDL_FullscreenOp fullscreen);
extern bool MOS_SetWindowGrab(SDL_VideoDevice *_this, SDL_Window * window, bool grabbed);
extern void MOS_DestroyWindow(SDL_VideoDevice *_this, SDL_Window * window);
extern void MOS_SetWindowAlwaysOnTop(SDL_VideoDevice *_this, SDL_Window * window, bool on_top);
extern void MOS_SetWindowResizable (SDL_VideoDevice *_this, SDL_Window * window, bool resizable);
extern bool MOS_GetWindowBordersSize(SDL_VideoDevice *_this, SDL_Window * window, int * top, int * left, int * bottom, int * right);
extern bool MOS_FlashWindow(SDL_VideoDevice *_this, SDL_Window * window, SDL_FlashOperation);
extern bool MOS_SetWindowHitTest(SDL_Window *window, bool enabled);
extern bool MOS_SetWindowOpacity(SDL_VideoDevice *_this, SDL_Window * window, float opacity);

/* Non-SDL functions */
extern void MOS_CloseWindows(SDL_VideoDevice *_this);
extern bool MOS_CreateSystemWindow(SDL_VideoDevice *_this, SDL_Window * window);
extern bool MOS_SetWindowOpacityPrivate(SDL_VideoDevice *_this, SDL_Window * window, ULONG value);
extern void MOS_IconifyWindow(SDL_VideoDevice *_this, bool with_appicon, SDL_Window * window);
extern void MOS_UniconifyWindow(SDL_VideoDevice *_this, SDL_Window * window);
extern void MOS_SetWindowBox(SDL_VideoDevice *_this, SDL_Window * window, SDL_Rect * rect);
extern void MOS_GetWindowSize(struct Window * window, int * width, int * height);
#endif /* _SDL_moswindow_h */
