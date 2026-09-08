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


/*
 * MorphOS debug tracing back-end for the D(...) macro
 * (see include/build_config/SDL_build_config_morphos.h).
 *
 * The tracing code is ALWAYS compiled in.  It stays completely silent unless
 * the MorphOS environment variable SDL3_HINT_DEBUG is set to "1":
 *
 *     setenv SDL3_HINT_DEBUG 1        (shell-local, this session)
 *     SetEnv SDL3_HINT_DEBUG 1        (ENV:, persists)
 *
 */

#include "SDL_internal.h"

#include <stdarg.h>

#include <proto/exec.h>
#include <proto/dos.h>

int MOS_DebugLevel = -1;   /* <0 = not checked yet, 0 = off, 1 = on */

int MOS_DebugCheck(void)
{
    char buf[4];

    if (!DOSBase) {
         return 0;
    }

    if (GetVar("SDL3_HINT_DEBUG", buf, sizeof(buf), 0) == 1 && buf[0] == '1') {
        MOS_DebugLevel = 1;
    } else {
        MOS_DebugLevel = 0;
    }

    return MOS_DebugLevel;
}

void MOS_DebugTrace(const char *func, const char *fmt, ...)
{
    static int busy = 0;   /* guard: the formatting path must not trace itself */
    char line[1024];
    va_list ap;
    int n;
    size_t l;
    const char *p;

    if (busy) {
        return;
    }
    busy = 1;

    n = SDL_snprintf(line, sizeof(line), "[%s] ", func ? func : "?");
    if (n < 0 || n >= (int)sizeof(line)) {
        n = 0;
    }

    va_start(ap, fmt);
    SDL_vsnprintf(line + n, sizeof(line) - (size_t)n, fmt ? fmt : "", ap);
    va_end(ap);

    l = SDL_strlen(line);
    if (l == 0 || line[l - 1] != '\n') {
        if (l + 1 < sizeof(line)) {
            line[l++] = '\n';
            line[l] = '\0';
        }
    }

    for (p = line; *p; p++) {
        RawPutChar(*p);
    }

    busy = 0;
}
