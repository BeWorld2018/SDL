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

#if SDL_VIDEO_DRIVER_MORPHOS

#include "../SDL_sysvideo.h"
#include "SDL_mosvideo.h"
#include "SDL_moswindow.h"

#include <proto/exec.h>
#include <proto/graphics.h>

#include <tgl/gl.h>
#include <tgl/gla.h>

GLContext *__tglContext;
struct Library *TinyGLBase;

bool
MOS_GL_LoadLibrary(SDL_VideoDevice *_this, const char *path)
{
	D(kprintf("[%s]\n", __FUNCTION__));
	if (!TinyGLBase) {
		TinyGLBase = OpenLibrary("tinygl.library", 53); 
	}
	if (TinyGLBase) {
			
			if (!LIB_MINVER(TinyGLBase, 53, 8))		
			{
				SDL_SetError("Failed to open tinygl.library 53.8+");
				return -1;
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
	D(kprintf("[%s]\n", __FUNCTION__));
	if (TinyGLBase) {
		CloseLibrary(TinyGLBase);
		TinyGLBase = NULL;
	}
}

static void
MOS_GL_FreeBitMap(SDL_VideoDevice *_this, SDL_Window *window)
{
    D(kprintf("[%s]\n", __FUNCTION__));
    SDL_WindowData *data = (SDL_WindowData *)window->internal;
    if (data->bitmap != NULL) {
        FreeBitMap(data->bitmap);
        data->bitmap = NULL;
    }
}

bool MOS_GL_AllocBitmap(SDL_VideoDevice *_this, SDL_Window * window)
{
	D(kprintf("[%s]\n", __FUNCTION__));
	SDL_WindowData *data = (SDL_WindowData *) window->internal;
	SDL_VideoData *videodata= data->videodata;

	if (data->bitmap != NULL)
		MOS_GL_FreeBitMap(_this, window);
	
	struct BitMap * friend_bitmap = data->win->RPort->BitMap;
	ULONG depth = GetBitMapAttr(friend_bitmap, BMA_DEPTH);

	if (videodata->CustomScreen) {
		D(kprintf("[%s] (fullscreen) displaydata->screen %p detected %d x %d \n", __FUNCTION__, videodata->CustomScreen, videodata->CustomScreen->Width, videodata->CustomScreen->Height));
		//w = videodata->CustomScreen->Width;
		//h = videodata->CustomScreen->Height;
	}
	int w = 0;
	int h = 0;
	MOS_GetWindowSize(data->win, &w, &h);
	
	D(kprintf("[%s] AllocBitMap w=%d h=%d depth=%d\n", __FUNCTION__, w, h, (int)depth));
	return (data->bitmap = AllocBitMap(w, h, depth, BMF_MINPLANES|BMF_DISPLAYABLE|BMF_3DTARGET, friend_bitmap)) != NULL;
}

bool MOS_GL_InitContext(SDL_VideoDevice *_this, SDL_Window * window)
{
	D(kprintf("[%s]\n", __FUNCTION__));
	
	SDL_WindowData *data = (SDL_WindowData *) window->internal;
	
	if (data->__tglContext != NULL) {
		GLADestroyContext(data->__tglContext);
		data->__tglContext = NULL;
        MOS_GL_FreeBitMap(_this, window);
	}

	struct TagItem tgltags[] =
	{
		{TAG_IGNORE, 0},
		{TGL_CONTEXT_STENCIL, TRUE},
		{TAG_DONE}
	};	
	
	if (MOS_GL_AllocBitmap(_this, window))
	{	
		D(kprintf("[%s] TGL_CONTEXT_BITMAP \n", __FUNCTION__));			
		tgltags[0].ti_Tag = TGL_CONTEXT_BITMAP;
		tgltags[0].ti_Data = (IPTR)data->bitmap;
	} else {
		D(kprintf("[%s] Failed to AllocBitmap ! \n", __FUNCTION__));	
		return false;
	}
		
	// Initialize new context
	int success = GLAInitializeContext(__tglContext, tgltags);
	if (success) {
		D(kprintf("[%s] GLAInitializeContext Success \n", __FUNCTION__));	
		data->__tglContext = __tglContext;
		
		// Clean Screen
		if (!window->flags & SDL_WINDOW_FULLSCREEN) {
			GLClearColor(__tglContext, 0.0f, 0.0f, 0.0f, 1.0f);
			GLClear(__tglContext, GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		}
			
	} else
		D(kprintf("[%s] GLAInitializeContext Failed \n", __FUNCTION__));	

	return success ? true : false;		
}

SDL_GLContext
MOS_GL_CreateContext(SDL_VideoDevice *_this, SDL_Window * window)
{
	SDL_WindowData *data = window->internal;
	
	GLContext *glcont = GLInit();
	if (glcont) {
		__tglContext = glcont;
		TGLSetAutomaticContextVersion(TinyGLBase, glcont);
		bool success = MOS_GL_InitContext(_this, window);
		if (success) {
			D(kprintf("[%s] SUCCES 0x%08lx, data->__tglContext=0x%08lx\n", __FUNCTION__, glcont, data->__tglContext));		
			return (SDL_GLContext)glcont;
		} else {
			D(kprintf("[%s] FAILED 0x%08lx, data->__tglContext=0x%08lx\n", __FUNCTION__, glcont, data->__tglContext));
			MOS_GL_FreeBitMap(_this, window);
			GLClose(glcont);
			data->__tglContext = __tglContext = NULL;
			
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
	D(kprintf("[%s] context 0x%08lx\n", __FUNCTION__, context));
	if (context)
		__tglContext = (GLContext*)context;
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
	D(kprintf("[%s] VSYNC=%d\n", __FUNCTION__, (data->vsyncEnabled ? 1 :0)));
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
	if (video->vsyncEnabled) {
		WaitTOF();
	}
	
	GLASwapBuffers(data->__tglContext);
	
	if (data->bitmap != NULL) {	
		BltBitMapRastPort(data->bitmap, 0, 0, data->win->RPort, data->win->BorderLeft, data->win->BorderTop, 
				window->w, window->h, 0xc0);
	}
	
	return true;
}

bool
MOS_GL_DestroyContext(SDL_VideoDevice *_this, SDL_GLContext context)
{
	D(kprintf("[%s] context 0x%08lx\n", __FUNCTION__, context));

	if (TinyGLBase != NULL && context) {
		SDL_Window *sdlwin;
		int deletions = 0;
		
		for (sdlwin = _this->windows; sdlwin; sdlwin = sdlwin->next) {

			SDL_WindowData *data = sdlwin->internal;

			if (data->__tglContext == (GLContext*)context) {
                D(kprintf("[%s] Found TinyGL context 0x%08lx, clearing window binding\n", __FUNCTION__, context));
                GLADestroyContext((GLContext*)context);
				data->__tglContext = NULL;
				MOS_GL_FreeBitMap(_this, sdlwin);
				deletions++;
			}
		}
		if (deletions == 0) {
            D(kprintf("[%s] GL context doesn't seem to have window binding\n", __FUNCTION__));
			return false;
        }
		GLClose((GLContext*)context);
		__tglContext = NULL;
		return true;
		
	}
	
	return false;
}

bool
MOS_GL_ResizeContext(SDL_VideoDevice *_this, SDL_Window *window)
{
	
	SDL_WindowData *data = (SDL_WindowData *) window->internal;
	D(kprintf("[%s] Context=0x%08lx data->__tglContext=0x%08lx data->win=0x%08lx\n", __FUNCTION__, __tglContext, data->__tglContext, data->win ));
	if (data->__tglContext == NULL || __tglContext == NULL || data->win == NULL) {
		return false;
	}
	
	return (MOS_GL_InitContext(_this, window) ? true : false);
}

#endif /* SDL_VIDEO_DRIVER_MORPHOS */
