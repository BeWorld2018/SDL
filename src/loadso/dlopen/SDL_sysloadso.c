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

#ifdef SDL_LOADSO_DLOPEN

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// System dependent library loading routines

#ifdef __MORPHOS__
#include <proto/exec.h>
#include <proto/dynload.h>
#else
#include <stdio.h>
#include <dlfcn.h>
#endif

#ifdef SDL_VIDEO_DRIVER_UIKIT
#include "../../video/uikit/SDL_uikitvideo.h"
#endif

#if defined(__MORPHOS__) && defined(BUILD_SDL3_LIBRARY)
struct Library *DynLoadBase = NULL;

static SDL_SpinLock mos_handles_lock;
static void **mos_handles = NULL;
static int mos_num_handles = 0;
static int mos_max_handles = 0;

static bool MOS_OpenDynLoad(void)
{
    if (!DynLoadBase) {
        DynLoadBase = OpenLibrary("dynload.library", 0);
        if (!DynLoadBase) {
            return SDL_SetError("dynload.library is not available");
        }
    }
    return true;
}

static void MOS_AddHandle(void *handle)
{
    SDL_LockSpinlock(&mos_handles_lock);
    if (mos_num_handles == mos_max_handles) {
        const int newmax = mos_max_handles ? mos_max_handles * 2 : 8;
        void **list = (void **)SDL_realloc(mos_handles, newmax * sizeof(void *));
        if (list) {
            mos_handles = list;
            mos_max_handles = newmax;
        }
    }
    if (mos_num_handles < mos_max_handles) {
        mos_handles[mos_num_handles++] = handle;
    }
    SDL_UnlockSpinlock(&mos_handles_lock);
}

static void MOS_RemoveHandle(void *handle)
{
    int i;

    SDL_LockSpinlock(&mos_handles_lock);
    for (i = mos_num_handles - 1; i >= 0; i--) {
        if (mos_handles[i] == handle) {
            mos_handles[i] = mos_handles[--mos_num_handles];
            break;
        }
    }
    SDL_UnlockSpinlock(&mos_handles_lock);
}

void MOS_LoadSO_Cleanup(void)
{
    /* What dynload does with modules still open when its base is closed
       is unknown: close them first, last loaded first */
    while (mos_num_handles > 0) {
        dlclose(mos_handles[--mos_num_handles]);
    }
    SDL_free(mos_handles);
    mos_handles = NULL;
    mos_max_handles = 0;

    if (DynLoadBase) {
        CloseLibrary(DynLoadBase);
        DynLoadBase = NULL;
    }
}
#endif // __MORPHOS__ && BUILD_SDL3_LIBRARY

SDL_SharedObject *SDL_LoadObject(const char *sofile)
{
    void *handle;
    const char *loaderror;

#ifdef SDL_VIDEO_DRIVER_UIKIT
    if (!UIKit_IsSystemVersionAtLeast(8.0)) {
        SDL_SetError("SDL_LoadObject requires iOS 8+");
        return NULL;
    }
#endif

#if defined(__MORPHOS__) && defined(BUILD_SDL3_LIBRARY)
    if (!MOS_OpenDynLoad()) {
        return NULL;
    }
#endif

    handle = dlopen(sofile, RTLD_NOW | RTLD_LOCAL);
    loaderror = dlerror();
    if (!handle) {
        SDL_SetError("Failed loading %s: %s", sofile, loaderror);
    }
#if defined(__MORPHOS__) && defined(BUILD_SDL3_LIBRARY)
    else {
        MOS_AddHandle(handle);
    }
#endif
    return (SDL_SharedObject *) handle;
}

SDL_FunctionPointer SDL_LoadFunction(SDL_SharedObject *handle, const char *name)
{
    void *symbol = dlsym(handle, name);
    if (!symbol) {
        // prepend an underscore for platforms that need that.
        bool isstack;
        size_t len = SDL_strlen(name) + 1;
        char *_name = SDL_small_alloc(char, len + 1, &isstack);
        _name[0] = '_';
        SDL_memcpy(&_name[1], name, len);
        symbol = dlsym(handle, _name);
        SDL_small_free(_name, isstack);
        if (!symbol) {
            SDL_SetError("Failed loading %s: %s", name,
                         (const char *)dlerror());
        }
    }
    return symbol;
}

void SDL_UnloadObject(SDL_SharedObject *handle)
{
    if (handle) {
#if defined(__MORPHOS__) && defined(BUILD_SDL3_LIBRARY)
        MOS_RemoveHandle(handle);
#endif
        dlclose(handle);
    }
}

#endif // SDL_LOADSO_DLOPEN
