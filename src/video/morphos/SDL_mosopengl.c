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

#if SDL_VIDEO_DRIVER_MORPHOS

#include "SDL_error.h"
#include "SDL_syswm.h"
#include "../SDL_sysvideo.h"
#include "SDL_mosvideo.h"
#include "SDL_mosmodes.h"
#include "SDL_moswindow.h"
#include "../../core/morphos/SDL_library.h"

#include <proto/exec.h>
#include <proto/tinygl.h>
#include <proto/intuition.h>
#include <proto/graphics.h>

#include <tgl/gl.h>
#include <tgl/gla.h>

GLContext *__tglContext;

extern struct SDL_Library *SDL2Base;

int
MOS_GL_LoadLibrary(_THIS, const char *path)
{

	if (!TinyGLBase)
		TinyGLBase = OpenLibrary("tinygl.library", 53); 

	if (TinyGLBase) {
			if (!LIB_MINVER(TinyGLBase, 53, 8))		
			{
				CloseLibrary(TinyGLBase);
				TinyGLBase = NULL;
				SDL_SetError("Failed to open tinygl.library 53.8+");
				return -1;
			}
			if (SDL2Base->MyTinyGLBase)				
				*SDL2Base->MyTinyGLBase = TinyGLBase;	
			
			return 0;
	} else 
		SDL_SetError("Failed to open tinygl.library 53+");

	return -1;
}

void *
MOS_GL_GetProcAddress(_THIS, const char *proc)
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
MOS_GL_UnloadLibrary(_THIS)
{
	D("[%s]\n", __FUNCTION__);
	if (TinyGLBase) {
		CloseLibrary(TinyGLBase);
		TinyGLBase = NULL;
	}
	if (SDL2Base->MyTinyGLBase) {
		*SDL2Base->MyTinyGLBase = NULL;
	}
}

static void
MOS_GL_FreeBitMap(_THIS, SDL_Window *window)
{
    D("[%s]\n", __FUNCTION__);
    SDL_WindowData *data = (SDL_WindowData *)window->driverdata;
    if (data->bitmap != NULL) {
        FreeBitMap(data->bitmap);
        data->bitmap = NULL;
    }
}

static SDL_bool
MOS_GL_AllocBitmap(_THIS, SDL_Window * window)
{
	SDL_WindowData *data = (SDL_WindowData *) window->driverdata;
	struct Screen *pubscr = NULL;
	struct BitMap *fb;
	int w = window->w, h = window->h;

	MOS_GL_FreeBitMap(_this, window);

	if (data->win) {
		fb = data->win->RPort->BitMap;
		w = getv(data->win, WA_InnerWidth);
		h = getv(data->win, WA_InnerHeight);
	} else {
		// Hidden window: draw offscreen, in the format of the screen it will open on
		struct Screen *scr = data->videodata->WScreen;

		if (scr == NULL)
			scr = pubscr = LockPubScreen(NULL);
		if (scr == NULL)
			return SDL_FALSE;
		fb = scr->RastPort.BitMap;
	}

    ULONG depth = GetBitMapAttr(fb, BMA_DEPTH);

	D("[%s] AllocBitMap w=%d h=%d depth=%d\n", __FUNCTION__, w, h, (int)depth);
    data->bitmap = AllocBitMap(SDL_max(w, 1), SDL_max(h, 1), depth,
                               BMF_MINPLANES | BMF_DISPLAYABLE | BMF_3DTARGET,
                               fb);

	if (pubscr)
		UnlockPubScreen(NULL, pubscr);

    return (data->bitmap != NULL);
}

// Attaches the context to a new bitmap for the window.
// data->bitmap != NULL means data->__tglContext is attached to it.
static SDL_bool
MOS_GL_InitContext(_THIS, SDL_Window * window, GLContext *context)
{
	D("[%s] context 0x%08lx\n", __FUNCTION__, context);
	SDL_WindowData *data = (SDL_WindowData *) window->driverdata;

	if (data->bitmap != NULL) {
		GLADestroyContext(data->__tglContext);
        MOS_GL_FreeBitMap(_this, window);
	}

	// Kept even if the attach fails, so that MOS_GL_DeleteContext() still finds it
	data->__tglContext = context;

	struct TagItem tgltags[] =
	{
		{TGL_CONTEXT_BITMAP, 0},
		{TGL_CONTEXT_STENCIL, TRUE},
		{TAG_DONE}
	};

	if (!MOS_GL_AllocBitmap(_this, window)) {
		D("[%s] Failed to AllocBitmap !\n", __FUNCTION__);
		return SDL_FALSE;
	}

	tgltags[0].ti_Data = (IPTR)data->bitmap;

	if (!GLAInitializeContext(context, tgltags)) {
		MOS_GL_FreeBitMap(_this, window);
		return SDL_FALSE;
	}

	return SDL_TRUE;
}

SDL_GLContext
MOS_GL_CreateContext(_THIS, SDL_Window * window)
{
    D("[%s]\n", __FUNCTION__);
	SDL_WindowData *data = window->driverdata;

	GLContext *glcont = GLInit();
	if (glcont) {
#ifdef TGL_CONTEXT_VERSION_53_9
		if (SDL2Base->MyGetMaximumContextVersion)
		{
			unsigned int contextversion;
			contextversion = SDL2Base->MyGetMaximumContextVersion(TinyGLBase);

			if (contextversion == TGL_CONTEXT_VERSION_53_1)
			{
				TGLEnableNewExtensions(glcont, 0);
			}
			else if (contextversion >= TGL_CONTEXT_VERSION_53_9)
			{
				TGLSetContextVersion(glcont, contextversion);
			}
		}
#endif
		if (MOS_GL_InitContext(_this, window, glcont)) {
			D("[%s] MOS_GL_InitContext SUCCES 0x%08lx, data->__tglContext=0x%08lx\n", __FUNCTION__, glcont, data->__tglContext);

			*SDL2Base->MyGLContext = __tglContext = glcont;

			GLClearColor(glcont, 0.0f, 0.0f, 0.0f, 1.0f);
			GLClear(glcont, GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

			return glcont;
		} else {
			D("[%s] MOS_GL_InitContext FAILED 0x%08lx, data->__tglContext=0x%08lx\n", __FUNCTION__, glcont, data->__tglContext);

			// The current context (maybe another window's) is left alone
			data->__tglContext = NULL;
			GLClose(glcont);

			SDL_SetError("Couldn't initialize TinyGL context");
		}
	} else {
		SDL_SetError("Couldn't create TinyGL context");
	}

	return NULL;
}

int
MOS_GL_MakeCurrent(_THIS, SDL_Window * window, SDL_GLContext context)
{
	if (context)
	{
		*SDL2Base->MyGLContext = __tglContext = context; 
	}
	else
	{
		*SDL2Base->MyGLContext = NULL;
		__tglContext = NULL;
	}
	return 0;
}

void
MOS_GL_GetDrawableSize(_THIS, SDL_Window *window, int *width, int *height)
{
    
	SDL_WindowData *data = window->driverdata;
    int w = 0;
    int h = 0;
    if (data->win) {
			w = data->win->Width - data->win->BorderLeft - data->win->BorderRight;
			h = data->win->Height - data->win->BorderTop - data->win->BorderBottom;
	}
   // D("[%s] System window size (%d * %d), SDL window size (%d * %d)\n", __FUNCTION__, w, h, window->w, window->h);
    if (width)
        *width = w;
    if (height)
        *height = h;

}

int
MOS_GL_SetSwapInterval(_THIS, int interval)
{
	SDL_VideoData *data = _this->driverdata;
	
	switch (interval) {
		case 0:
		case 1:
			// always VSYNC in fullscreen
			data->vsyncEnabled = /*data->CustomScreen != NULL ? TRUE : */(interval ? TRUE : FALSE);
			return 0;
		default:
			return -1;
	}	
}

int
MOS_GL_GetSwapInterval(_THIS)
{
	SDL_VideoData *data = _this->driverdata;
	return data->vsyncEnabled ? 1 : 0;
}

int
MOS_GL_SwapWindow(_THIS, SDL_Window * window)
{
	SDL_WindowData *data = (SDL_WindowData *) window->driverdata;

	// No bitmap: the context could not be attached (failed resize)
	if (!data->win || !data->__tglContext || !data->bitmap) {
		SDL_SetError("SwapWindow called with no valid GL context");
		return -1;
	}

	SDL_VideoData *video = _this->driverdata;
	
	GLASwapBuffers(data->__tglContext);

	// WaitTOF() sleeps until the vertical blank, WaitBOVP() polls the beam position
	if (video->vsyncEnabled) {
		WaitTOF();
	}

	if (data->bitmap != NULL) {
		
		BltBitMapRastPort(data->bitmap, 0, 0, data->win->RPort, data->win->BorderLeft, data->win->BorderTop, 
				window->w, window->h, 0xc0);
	}
	
	return 0;
}

void
MOS_GL_DeleteContext(_THIS, SDL_GLContext context)
{
    if (!TinyGLBase || !context)
        return;

    SDL_Window *sdlwin;
    SDL_bool found = SDL_FALSE;

    for (sdlwin = _this->windows; sdlwin; sdlwin = sdlwin->next) {
        SDL_WindowData *data = sdlwin->driverdata;
        if (data == NULL)
            continue;
	     D("[%s] data->__tglContext=0x%08lx\n", __FUNCTION__, data->__tglContext);
        if (data->__tglContext == context) {

            // Already detached if the last MOS_GL_InitContext() failed
            if (data->bitmap != NULL) {
				D("[%s] GLADestroyContext data->__tglContext=0x%08lx\n", __FUNCTION__, data->__tglContext);
                GLADestroyContext(context);
				MOS_GL_FreeBitMap(_this, sdlwin);
			}
			data->__tglContext = NULL;
			found = SDL_TRUE;
        }
    }

    if (found) {
		GLClose(context);
    }

    // Another window's context may be the current one
    if (__tglContext == context) {
        *SDL2Base->MyGLContext = __tglContext = NULL;
    }
}

int
MOS_GL_ResizeContext(_THIS, SDL_Window *window)
{
	
	SDL_WindowData *data = (SDL_WindowData *) window->driverdata;
	D("[%s] Context=0x%08lx data->__tglContext=0x%08lx\n", __FUNCTION__, __tglContext, data->__tglContext);
	if (data->__tglContext == NULL) {
		return -1;
	}

	return (MOS_GL_InitContext(_this, window, data->__tglContext) ? 0 : -1);
}

#endif /* SDL_VIDEO_DRIVER_MORPHOS */
