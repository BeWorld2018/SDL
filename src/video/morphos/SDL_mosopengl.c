/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2022 Sam Lantinga <slouken@libsdl.org>

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
#include <tgl/glu.h>

GLContext *__tglContext;

extern void *AmiGetGLProc(const char *proc);

extern struct SDL_Library *SDL2Base;

UWORD TinyGl_NewVersion = 0;

int
MOS_GL_LoadLibrary(_THIS, const char *path)
{
	if (SDL2Base->MyTinyGLBase) {
		if (!TinyGLBase)
			TinyGLBase = OpenLibrary("tinygl.library", 52); 

		if (TinyGLBase) {
				UWORD TinyGl_Version = TinyGLBase->lib_Version;
				UWORD TinyGl_Revision = TinyGLBase->lib_Revision;
				D("[%s] tinygl.library version %d.%d\n", __FUNCTION__, TinyGl_Version, TinyGl_Revision);
				*SDL2Base->MyTinyGLBase = TinyGLBase;
				TinyGl_NewVersion = 1;
				if (TinyGl_Version < 52 ||	
					(TinyGl_Version == 52 && TinyGl_Revision < 10))
				{
					// tingl < 52.10
					TinyGl_NewVersion = 0;
					SDL_SetError("Failed to open tinygl.library 52.10");
					return -1;
				}
				
				return 0;
		}
	}

	SDL_SetError("Failed to open tinygl.library 52.0");
	return -1;
}

void *
MOS_GL_GetProcAddress(_THIS, const char *proc)
{
	void *func = NULL;
	func = AmiGetGLProc(proc);
	// D("[%s] proc %s func 0x%08lx\n", __FUNCTION__, proc, func);
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

	if (SDL2Base->MyTinyGLBase && *SDL2Base->MyTinyGLBase && TinyGLBase) {
		CloseLibrary(TinyGLBase);
		*SDL2Base->MyTinyGLBase = TinyGLBase = NULL;
	}
}

SDL_GLContext
MOS_GL_CreateContext(_THIS, SDL_Window * window)
{
	SDL_WindowData *data = (SDL_WindowData *) window->driverdata;
	SDL_VideoData *vd = data->videodata;
	BYTE fullscreen = data->winflags & SDL_MOS_WINDOW_FULLSCREEN;

	GLContext *glcont = GLInit();
	D("[%s] new context 0x%08lx\n", __FUNCTION__, glcont);

	if (glcont) {

		int success;
		struct TagItem tgltags[] =
		{
			{TAG_IGNORE, 0},
			{TGL_CONTEXT_STENCIL, TRUE}, // TODO check if stencil and depth are needed
			{TAG_DONE}
		};

		if (fullscreen) {
			tgltags[0].ti_Tag = TGL_CONTEXT_SCREEN;
			tgltags[0].ti_Data = (IPTR)vd->CustomScreen;
		} if (data->win) {
			tgltags[0].ti_Tag = TGL_CONTEXT_WINDOW;
			tgltags[0].ti_Data = (IPTR)data->win;
		} else {
			// window not exist but you want createContext (example: SDL_WINDOW_HIDDEN)
			// so use Ambient screen and Context Bitmap	?!		
			if (vd->WScreen == NULL)
				MOS_GetScreen(vd->VideoDevice, 0, (window->flags & SDL_WINDOW_OPENGL) != 0);
			
			D("[%s] new context unless window 0x%08lx - 0x%08lx\n", __FUNCTION__, glcont, vd->WScreen);
			tgltags[0].ti_Tag = TGL_CONTEXT_BITMAP;
			tgltags[0].ti_Data = (IPTR)vd->WScreen->RastPort.BitMap;
		}

		success = GLAInitializeContext(glcont, tgltags);
		if (success) {
			*SDL2Base->MyGLContext = __tglContext = glcont;
			data->__tglContext = glcont;
			D("[%s] new context SUCCES 0x%08lx\n", __FUNCTION__, glcont);
			return glcont;
		}
		D("[%s] new context FAILED 0x%08lx\n", __FUNCTION__, glcont);
		GLClose(glcont);
		data->__tglContext = NULL;
		
		SDL_SetError("Couldn't initialize context TinyGL");
	} else {
		SDL_SetError("Couldn't create TinyGL/OpenGL context");
	}

	return NULL;
}

int
MOS_GL_MakeCurrent(_THIS, SDL_Window * window, SDL_GLContext context)
{
	D("[%s] context 0x%08lx\n", __FUNCTION__, context);
	if (context)
		*SDL2Base->MyGLContext = __tglContext = context;
	return 0;
}

void
MOS_GL_GetDrawableSize(_THIS, SDL_Window * window, int *w, int *h)
{
	if (window) {
		SDL_WindowData * data = window->driverdata;

		if (w)
			*w = data->win->Width - data->win->BorderLeft - data->win->BorderRight;
		if (h)
			*h = data->win->Height - data->win->BorderTop - data->win->BorderBottom;
	}
}

int
MOS_GL_SetSwapInterval(_THIS, int interval)
{
	//D("[%s] interval=%d\n", __FUNCTION__, interval);
	SDL_VideoData *data = _this->driverdata;
	
	switch (interval) {
		case 0:
			// always VSYNC in fullscreen
			data->vsyncEnabled = data->CustomScreen != NULL ? TRUE : FALSE;
			return 0;
		break;
		case 1:

			data->vsyncEnabled = TRUE;
			return 0;
		break;
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
	//D("[%s] context 0x%08lx\n", __FUNCTION__, data->__tglContext, data->win);
	if (!data->win || !data->__tglContext || !__tglContext)
		return -1;

	SDL_VideoData *video = _this->driverdata;
	if (video->vsyncEnabled) {
		BOOL displayed = getv(data->win->WScreen, SA_Displayed);
		if (displayed) {
			WaitBOVP(&data->win->WScreen->ViewPort);
		}
	}
	
	GLASwapBuffers(data->__tglContext);
	return 0;
}

void
MOS_GL_DeleteContext(_THIS, SDL_GLContext context)
{
	D("[%s] context 0x%08lx\n", __FUNCTION__, context);

	if (TinyGLBase == NULL) {
		D("[%s] the library is already closed\n", __FUNCTION__);
		return;
	}
	if (context) {
		SDL_Window *sdlwin;

		for (sdlwin = _this->windows; sdlwin; sdlwin = sdlwin->next) {

			SDL_WindowData *data = sdlwin->driverdata;

			if (data->__tglContext == context) {
                D("[%s] Found TinyGL context 0x%08lx, clearing window binding\n", __FUNCTION__, context);
                GLADestroyContext(context);
				GLClose(context);
                data->__tglContext = NULL;
			}
		}
	}
}

int
MOS_GL_ResizeContext(_THIS, SDL_Window *window)
{
	SDL_WindowData *data = (SDL_WindowData *) window->driverdata;
	SDL_VideoData *vd = data->videodata;
	int success;

	if (data->__tglContext == NULL || __tglContext == NULL) {
		// only if __tglContext exists
		D("[%s] no OpenGL context\n", __FUNCTION__);
		return -1;
	}
	if (data->win == NULL)  {
		D("[%s] no window\n", __FUNCTION__);
		return -1;
	}
	
	if (vd->CustomScreen) {
		struct TagItem tgltags[] =
		{
			{TGL_CONTEXT_SCREEN, (IPTR)vd->CustomScreen},
			{TGL_CONTEXT_STENCIL, TRUE}, // TODO check if stencil and depth are needed
			{TAG_DONE}
		};
		GLADestroyContext(data->__tglContext);
		success = GLAInitializeContext(data->__tglContext, tgltags);
	} else {
		success = GLAReinitializeContextWindowed(data->__tglContext, data->win);
	}
	D("[%s] success %d\n", __FUNCTION__, success);

	return success ? 0 : -1;
}

#endif /* SDL_VIDEO_DRIVER_MORPHOS */
