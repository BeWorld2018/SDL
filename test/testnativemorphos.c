/*
  Copyright (C) 1997-2017 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.
*/

#include "testnative.h"

#ifdef TEST_NATIVE_MORPHOS

#include <proto/intuition.h>

static void *
CreateWindowMOS(int w, int h)
{
	struct Window *window = OpenWindowTags(NULL,
		WA_InnerWidth, w,
		WA_InnerHeight, h,
		WA_Title, "Native window",
		WA_IDCMP, IDCMP_CLOSEWINDOW,
		TAG_DONE);

	return (void *) window;
}

static void
DestroyWindowMOS(void *window)
{
	CloseWindow(window);
}

NativeWindowFactory MorphOSWindowFactory = {
    "mos",
    CreateWindowMOS,
    DestroyWindowMOS
};

#endif

