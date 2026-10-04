/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2024 Sam Lantinga <slouken@libsdl.org>

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

#include "SDL_messagebox.h"
#include "../SDL_sysvideo.h"
#include "../../core/morphos/SDL_misc.h"

#include <proto/charsets.h>
#include <proto/exec.h>
#include <proto/muimaster.h>

int
MOS_ShowMessageBox(const SDL_MessageBoxData *mbd, int *buttonid)
{
	struct Library *MUIMasterBase = OpenLibrary("muimaster.library", 0);
	char *title = NULL, *message = NULL, *btxt = NULL, **labels = NULL;
	size_t args[1], len = sizeof("_OK");
	int i, n = mbd->numbuttons, rc = -1;

	D("[%s]\n", __FUNCTION__);

	if (!MUIMasterBase)
		return -1;

	title = MOS_ConvertText(mbd->title, MIBENUM_UTF_8, MIBENUM_SYSTEM);
	message = MOS_ConvertText(mbd->message, MIBENUM_UTF_8, MIBENUM_SYSTEM);
	if (!title || !message)
		goto done;

	if (n > 0) {
		labels = SDL_calloc(n, sizeof(*labels));
		if (!labels)
			goto done;

		for (i = 0; i < n; i++) {
			labels[i] = MOS_ConvertText(mbd->buttons[i].text ? mbd->buttons[i].text : "", MIBENUM_UTF_8, MIBENUM_SYSTEM);
			if (!labels[i])
				goto done;
			len += SDL_strlen(labels[i]) + 2;  /* '|' and '*' */
		}
	}

	btxt = SDL_malloc(len);
	if (!btxt)
		goto done;

	if (n == 0) {
		SDL_strlcpy(btxt, "_OK", len);
	} else {
		*btxt = '\0';
		for (i = 0; i < n; i++) {
			if (i > 0)
				SDL_strlcat(btxt, "|", len);
			if (mbd->buttons[i].flags & SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT)
				SDL_strlcat(btxt, "*", len);
			SDL_strlcat(btxt, labels[i], len);
		}
	}

	/* The message is an argument, never the format string */
	args[0] = (size_t)message;
	rc = MUI_RequestA(NULL, NULL, 0, title, btxt, "%s", args);

	/* 1..n-1 for the buttons from the left, 0 for the rightmost one */
	rc = rc > 0 ? rc - 1 : n - 1;
	if (rc >= n)
		rc = n - 1;

	*buttonid = n > 0 ? mbd->buttons[rc].buttonid : -1;
	rc = 0;

done:
	if (rc < 0)
		SDL_OutOfMemory();

	if (labels) {
		for (i = 0; i < n; i++)
			SDL_free(labels[i]);
		SDL_free(labels);
	}
	SDL_free(btxt);
	SDL_free(message);
	SDL_free(title);

	CloseLibrary(MUIMasterBase);

	return rc;
}
