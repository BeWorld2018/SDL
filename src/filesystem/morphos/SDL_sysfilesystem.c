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

#ifdef SDL_FILESYSTEM_MORPHOS

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* System dependent filesystem routines                                */

#include "../SDL_sysfilesystem.h"

char *SDL_SYS_GetBasePath(void)
{
    char* buffer = NULL;
    const char* const basePath = "PROGDIR:";

    size_t len = SDL_strlen(basePath) + 1;

    buffer = (char *) SDL_malloc(len);
    if (!buffer) {
        SDL_OutOfMemory();
        return NULL;
    }

    SDL_memset(buffer, 0, len);
    SDL_snprintf(buffer, len, "%s", basePath);

    return buffer;
}

char *SDL_SYS_GetPrefPath(const char *org, const char *app)
{
    const char* const envPath = "ENVARC:";
    size_t len = SDL_strlen(envPath) + 1;
    char* buffer = NULL;

    if (org) {
        len += SDL_strlen(org) + 1;
    }

    if (app) {
        len += SDL_strlen(app) + 1;
    }

    buffer = (char *) SDL_malloc(len);
    if (!buffer) {
        SDL_OutOfMemory();
        return NULL;
    }

    SDL_memset(buffer, 0, len);
    SDL_snprintf(buffer, len, envPath);

    if (org) {
        SDL_snprintf(buffer + SDL_strlen(buffer), len - SDL_strlen(buffer), "%s/", org);
    }

    if (app) {
        SDL_snprintf(buffer + SDL_strlen(buffer), len - SDL_strlen(buffer), "%s/", app);
    }

	//D("[%s] SDL_CreateDirectory: %s\n", __FUNCTION__, buffer);
    if (SDL_CreateDirectory(buffer)) {
        return buffer;
    }

    SDL_free(buffer);
    return NULL;
}

char *SDL_SYS_GetUserFolder(SDL_Folder folder)
{
    const char* const path = "PROGDIR:";
    const size_t pathLen = strlen(path) + 1;
    char* folderPath = NULL;

    switch (folder) {
        case SDL_FOLDER_HOME:
        case SDL_FOLDER_DOCUMENTS:
            folderPath = SDL_malloc(pathLen);
            if (folderPath) {
                SDL_snprintf(folderPath, pathLen, "%s", path);
            } else {
                SDL_OutOfMemory();
            }
            break;
        default:
            //D("[%s] Unsupported user folder %d\n", __FUNCTION__, folder);
            SDL_SetError("Unsupported user folder");
            break;
    }

    return folderPath;
}

#endif /* SDL_FILESYSTEM_MORPHOS */

