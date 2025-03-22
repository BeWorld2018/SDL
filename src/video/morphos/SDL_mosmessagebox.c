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

#include "SDL_messagebox.h"
#include "../SDL_sysvideo.h"
#include "SDL_misc.h"
#include "SDL_moswindow.h"

#include <proto/charsets.h>
#include <proto/exec.h>
#include <proto/muimaster.h>

bool
MOS_ShowMessageBox(const SDL_MessageBoxData *mbd, int *buttonID)
{
	D(kprintf("[%s]\n", __FUNCTION__));

	if (MUIMasterBase) {
		char *title = MOS_ConvertText(mbd->title, MIBENUM_UTF_8, MIBENUM_SYSTEM);
        if (title) {

            char *message = MOS_ConvertText(mbd->message, MIBENUM_UTF_8, MIBENUM_SYSTEM);
            if (message) {
   
                char *btxt = SDL_malloc(1024);

                if (btxt) {
                	int rc = -1;
                    char *buf = btxt;
                    for (size_t i = 0; i < mbd->numbuttons; i++) {
                        if (i > 0)
                            *buf++ = '|';

                        if (mbd->buttons[i].flags & SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT)
                            *buf++ = '*';

                        buf += ConvertTagList((APTR)mbd->buttons[i].text, -1, buf, -1, MIBENUM_UTF_8, MIBENUM_SYSTEM, NULL);
                    }

                    *buf = '\0';
					
					/*struct Window * syswin = NULL;
					if (mbd->window) {
						// TODO: TEST... crash actualy
						SDL_WindowData *data = mbd->window->internal;
						syswin = data->win;
					}*/
					
                    rc = MUI_RequestA(NULL, NULL, 0, title == NULL ? "SDL3" : title, btxt, message, NULL);
					D(kprintf("[%s] MUI_REquestA\n", __FUNCTION__));
						
                    if (rc == 0)
                        rc = mbd->numbuttons - 1;
                    else
                        rc -= 1;

					*buttonID = mbd->buttons[rc].buttonID;

                    SDL_free(btxt);
                }

                SDL_free(message);
            }

            SDL_free(title);
        }

		return true;
	}
	
	return false;
}
