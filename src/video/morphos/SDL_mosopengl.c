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

#if SDL_VIDEO_DRIVER_MORPHOS

#include "../SDL_sysvideo.h"
#include "SDL_mosvideo.h"
#include "SDL_moswindow.h"
#include "SDL_mosopengl.h"

#include <proto/exec.h>
#include <proto/graphics.h>

#include <tgl/gl.h>
#include <tgl/gla.h>

GLContext *__tglContext;
struct Library *TinyGLBase;

#ifdef BUILD_SDL3_LIBRARY
#include "../../core/morphos/SDL_library.h"
#endif

/* sdl3.library: the program calls gl*() with its own TinyGLBase and
   __tglContext (libGL), mirror ours there. */
static void MOS_GL_SyncCaller(void)
{
#ifdef BUILD_SDL3_LIBRARY
	MOS_LIB_SET_TGLBASE(TinyGLBase);
	MOS_LIB_SET_TGLCONTEXT(__tglContext);
#endif
}

bool
MOS_GL_LoadLibrary(SDL_VideoDevice *_this, const char *path)
{
	D("");
	if (!TinyGLBase) {
		TinyGLBase = OpenLibrary("tinygl.library", 53);
		MOS_GL_SyncCaller();
	}

	if (TinyGLBase) {

			if (!LIB_MINVER(TinyGLBase, 53, 10))
			{
				CloseLibrary(TinyGLBase);
				TinyGLBase = NULL;
				MOS_GL_SyncCaller();
				SDL_SetError("Failed to open tinygl.library 53.10+");
				return false;
			}
			return true;
	} else
		SDL_SetError("Failed to open tinygl.library 53+");

	return false;
}

SDL_FunctionPointer
MOS_GL_GetProcAddress(SDL_VideoDevice *_this, const char *proc)
{
	void *func = NULL;
	func = tglGetProcAddress(proc);
	if (!func) {
    	SDL_SetError("Couldn't find OpenGL symbol");
		return NULL;
    }
	return func;
}

void
MOS_GL_UnloadLibrary(SDL_VideoDevice *_this)
{
	D("");
	if (TinyGLBase) {
		CloseLibrary(TinyGLBase);
		TinyGLBase = NULL;
		MOS_GL_SyncCaller();
	}
}

static void
MOS_GL_FreeBitMap(SDL_VideoDevice *_this, SDL_Window *window)
{
	D("");
    SDL_WindowData *data = (SDL_WindowData *)window->internal;
    if (data->bitmap != NULL) {
        FreeBitMap(data->bitmap);
        data->bitmap = NULL;
    }
}

/* Without a system window (screen closed under it), the bitmap gets the
   window size, in the format of the public screen. */
static bool
MOS_GL_AllocBitmap(SDL_VideoDevice *_this, SDL_Window * window)
{
	SDL_WindowData *data = (SDL_WindowData *) window->internal;

	MOS_GL_FreeBitMap(_this, window);

	struct BitMap * friend_bitmap = NULL;
	int w = window->w;
	int h = window->h;

	if (data->win) {
		friend_bitmap = data->win->RPort->BitMap;
		MOS_GetWindowSize(data->win, &w, &h);
	} else if (data->videodata->PublicScreen) {
		friend_bitmap = data->videodata->PublicScreen->RastPort.BitMap;
	}
	ULONG depth = friend_bitmap ? GetBitMapAttr(friend_bitmap, BMA_DEPTH) : 32;

	D("AllocBitMap w=%d h=%d depth=%d", w, h, (int)depth);
	return (data->bitmap = AllocBitMap(SDL_max(w, 1), SDL_max(h, 1), depth, BMF_MINPLANES|BMF_DISPLAYABLE|BMF_3DTARGET, friend_bitmap)) != NULL;
}

/* Exclusive fullscreen on our custom screen, opened with hardware 3D: TinyGL
   renders straight into it and flips its own buffers (triple buffering),
   there is no bitmap to copy. NULL when the window renders into a bitmap. */
static struct Screen *
MOS_GL_GetTargetScreen(SDL_VideoDevice *_this, SDL_Window *window)
{
	SDL_VideoData *video = _this->internal;
	SDL_WindowData *data = (SDL_WindowData *) window->internal;
	struct Window *win = data->win;
	struct Screen *screen = video->CustomScreen;

	if (!win || !screen || !video->CustomScreen3D || win->WScreen != screen || !(window->flags & SDL_WINDOW_FULLSCREEN) || data->gl_screen_failed) {
		return NULL;
	}
	// The whole screen is drawn: the window has to cover it
	if (win->Width != screen->Width || win->Height != screen->Height) {
		return NULL;
	}
	return screen;
}

/* Detaches the window's context from its bitmap or screen. The GLContext
   (textures, state) is kept for MOS_GL_InitContext(). */
static void
MOS_GL_DetachContext(SDL_VideoDevice *_this, SDL_Window *window)
{
	SDL_WindowData *data = (SDL_WindowData *) window->internal;

	if (data->bitmap != NULL || data->gl_screen != NULL) {
		GLADestroyContext(data->__tglContext);
		data->gl_screen = NULL;
		MOS_GL_FreeBitMap(_this, window);
	}
}

/* Attaches the context to the window's screen or to a new bitmap. */
static bool
MOS_GL_InitContext(SDL_VideoDevice *_this, SDL_Window * window, GLContext *context)
{
	D("context 0x%08lx", context);
	SDL_WindowData *data = (SDL_WindowData *) window->internal;
	SDL_VideoData *video = _this->internal;

	if (data->__tglContext != NULL) {
		MOS_GL_DetachContext(_this, window);
	}

	// Kept even if the attach fails, so that MOS_GL_DestroyContext() still finds it
	data->__tglContext = context;

	struct TagItem tgltags[] =
	{
		{TAG_IGNORE, 0},
		{TGL_CONTEXT_STENCIL, TRUE},
		{TAG_DONE}
	};

	struct Screen *screen = MOS_GL_GetTargetScreen(_this, window);
	if (screen) {
		tgltags[0].ti_Tag = TGL_CONTEXT_SCREEN;
		tgltags[0].ti_Data = (IPTR)screen;
		if (GLAInitializeContext(context, tgltags)) {
			D("GLAInitializeContext on screen %p Success", screen);
			data->gl_screen = screen;
			GLASetSync(context, video->vsyncEnabled ? 1 : 0);
			return true;
		}
		D("GLAInitializeContext on screen %p Failed, using a bitmap", screen);
		data->gl_screen_failed = true;
	}

	if (!MOS_GL_AllocBitmap(_this, window)) {
		D("Failed to AllocBitmap !");
		return false;
	}

	tgltags[0].ti_Tag = TGL_CONTEXT_BITMAP;
	tgltags[0].ti_Data = (IPTR)data->bitmap;
	if (!GLAInitializeContext(context, tgltags)) {
		D("GLAInitializeContext Failed");
		MOS_GL_FreeBitMap(_this, window);
		return false;
	}

	D("GLAInitializeContext Success");
	return true;
}

void
MOS_GL_DetachScreenContexts(SDL_VideoDevice *_this, struct Screen *screen)
{
	for (SDL_Window *sdlwin = _this->windows; sdlwin; sdlwin = sdlwin->next) {
		SDL_WindowData *data = sdlwin->internal;

		// Back to a bitmap, MOS_GL_SwapWindow() moves it to the next target
		if (data && data->__tglContext && data->gl_screen == screen) {
			D("Context 0x%08lx leaves screen %p", data->__tglContext, screen);
			MOS_GL_InitContext(_this, sdlwin, data->__tglContext);
		}
	}
}

SDL_GLContext
MOS_GL_CreateContext(SDL_VideoDevice *_this, SDL_Window * window)
{
	D("");
	SDL_WindowData *data = window->internal;

	GLContext *glcont = GLInit();
	if (glcont) {
		__tglContext = glcont;
		MOS_GL_SyncCaller();
#ifdef BUILD_SDL3_LIBRARY
		/* The program's gl*() calls follow the TinyGL SDK it was built
		   with: use its newest context version, as sdl2.library does. */
		if (SDL3Base && SDL3Base->MyGetMaximumContextVersion) {
			unsigned int contextversion = SDL3Base->MyGetMaximumContextVersion(TinyGLBase);

			if (contextversion == TGL_CONTEXT_VERSION_53_1) {
				TGLEnableNewExtensions(glcont, 0);
			} else if (contextversion >= TGL_CONTEXT_VERSION_53_9) {
				TGLSetContextVersion(glcont, contextversion);
			}
		} else
#endif
		TGLSetAutomaticContextVersion(TinyGLBase, glcont);
		bool success = MOS_GL_InitContext(_this, window, glcont);
		if (success) {
			if ((window->flags & SDL_WINDOW_FULLSCREEN) == 0) {
				GLClearColor(__tglContext, 0.0f, 0.0f, 0.0f, 1.0f);
				GLClear(__tglContext, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}
			D("SUCCES 0x%08lx, data->__tglContext=0x%08lx", glcont, data->__tglContext);
			return (SDL_GLContext)glcont;
		} else {
			D("FAILED 0x%08lx, data->__tglContext=0x%08lx", glcont, data->__tglContext);
			MOS_GL_FreeBitMap(_this, window);
			GLClose(glcont);
			data->__tglContext = __tglContext = NULL;
			MOS_GL_SyncCaller();
			SDL_SetError("Couldn't initialize TinyGL context");
		}
	} else {
		SDL_SetError("Couldn't create TinyGL context");
	}

	return NULL;
}

bool
MOS_GL_MakeCurrent(SDL_VideoDevice *_this, SDL_Window * window, SDL_GLContext context)
{
	if (context)
		__tglContext = (GLContext*)context;
	else
		__tglContext = NULL;

	MOS_GL_SyncCaller();
	return true;
}

bool
MOS_GL_SetSwapInterval(SDL_VideoDevice *_this, int interval)
{
	if (interval < 0) {
        return SDL_SetError("Late swap tearing currently unsupported");
    }
	SDL_VideoData *data = _this->internal;
	data->vsyncEnabled = (interval == 1 ? TRUE : FALSE);
	D("vsyncEnabled=%d", (data->vsyncEnabled ? 1 :0));

	// Screen contexts flip by themselves
	for (SDL_Window *sdlwin = _this->windows; sdlwin; sdlwin = sdlwin->next) {
		SDL_WindowData *wd = sdlwin->internal;
		if (wd && wd->gl_screen) {
			GLASetSync(wd->__tglContext, data->vsyncEnabled ? 1 : 0);
		}
	}
	return true;
}

bool
MOS_GL_GetSwapInterval(SDL_VideoDevice *_this, int *interval)
{
	SDL_VideoData *data = _this->internal;
	*interval = data->vsyncEnabled ? 1 : 0;
	return true;
}

bool
MOS_GL_SwapWindow(SDL_VideoDevice *_this, SDL_Window * window)
{
	SDL_WindowData *data = (SDL_WindowData *) window->internal;
	if (!data->win || !data->__tglContext || !__tglContext)
		return false;

	SDL_VideoData *video = _this->internal;

	if (data->gl_screen != NULL) {
		// TinyGL flips the screen buffers, GLASetSync() follows the swap interval
		GLASwapBuffers(data->__tglContext);
	} else if (data->bitmap != NULL) {
		// The card starts first, then the beam is waited for, as in sdl2.library
		GLASwapBuffers(data->__tglContext);

		if (video->vsyncEnabled) {
			WaitBOVP(&data->win->WScreen->ViewPort);
		}

		BltBitMapRastPort(data->bitmap, 0, 0, data->win->RPort, data->win->BorderLeft, data->win->BorderTop,
				window->w, window->h, 0xc0);
	}

	// Custom screen opened or closed under the context (fullscreen switch,
	// hide/show), or detached: the next frame goes to the right target
	if (MOS_GL_GetTargetScreen(_this, window) != data->gl_screen ||
	    (data->gl_screen == NULL && data->bitmap == NULL)) {
		return MOS_GL_InitContext(_this, window, data->__tglContext);
	}

	return true;
}

bool
MOS_GL_DestroyContext(SDL_VideoDevice *_this, SDL_GLContext context)
{
	D("context 0x%08lx", context);

	if (TinyGLBase != NULL && context) {
		SDL_Window *sdlwin;
		int deletions = 0;

		for (sdlwin = _this->windows; sdlwin; sdlwin = sdlwin->next) {

			SDL_WindowData *data = sdlwin->internal;

			if (data->__tglContext == (GLContext*)context) {
                D("Found TinyGL context 0x%08lx, clearing window binding", context);
                MOS_GL_DetachContext(_this, sdlwin);
				data->__tglContext = NULL;
				deletions++;
			}
		}
		if (deletions == 0) {
            D("GL context doesn't seem to have window binding\n");
			return false;
        }
		GLClose((GLContext*)context);
		__tglContext = NULL;
		MOS_GL_SyncCaller();
		return true;

	}

	return false;
}

bool
MOS_GL_ResizeContext(SDL_VideoDevice *_this, SDL_Window *window)
{
	SDL_WindowData *data = (SDL_WindowData *) window->internal;

	D("Context=0x%08lx data->__tglContext=0x%08lx data->win=0x%08lx", __tglContext, data->__tglContext, data->win);
	if (data->__tglContext == NULL || data->win == NULL) {
		return false;
	}

	if (!MOS_GL_InitContext(_this, window, data->__tglContext)) {
		return false;
	}

	SDL_Renderer *renderer = SDL_GetRenderer(window);
	if (renderer) {
		SDL_FlushRenderer(renderer);
	}

	return true;
}

#endif /* SDL_VIDEO_DRIVER_MORPHOS */
