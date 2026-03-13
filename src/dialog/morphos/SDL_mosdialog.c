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

#ifdef SDL_DIALOG_MORPHOS

#include "../../thread/SDL_systhread.h"

#include <proto/asl.h>

typedef struct
{
    SDL_DialogFileFilter *filters;
    int nfilters;
    char *title;
    char *accept;
    char *cancel;
    char *default_file;
    char *default_dir;
    struct Window *window;
    bool allow_many;
    bool save;
    bool dir_only;
    SDL_DialogFileCallback callback;
    void *userdata;
} MOS_DialogArgs;

static const char *MOS_DefaultAccept(void)
{
    return "Ok";
}

static const char *MOS_DefaultCancel(void)
{
    return "Cancel";
}

static void MOS_FreePathList(char **paths, int count)
{
    int i;

    if (!paths) {
        return;
    }

    for (i = 0; i < count; i++) {
        SDL_free(paths[i]);
    }
    SDL_free(paths);
}

static void MOS_FreeFilters(SDL_DialogFileFilter *filters, int nfilters)
{
    int i;

    if (!filters) {
        return;
    }

    for (i = 0; i < nfilters; i++) {
        SDL_free((void *)filters[i].name);
        SDL_free((void *)filters[i].pattern);
    }

    SDL_free(filters);
}

static void MOS_FreeDialogArgs(MOS_DialogArgs *args)
{
    if (!args) {
        return;
    }

    MOS_FreeFilters(args->filters, args->nfilters);
    SDL_free(args->title);
    SDL_free(args->accept);
    SDL_free(args->cancel);
    SDL_free(args->default_file);
    SDL_free(args->default_dir);

    SDL_free(args);
}

static bool MOS_CopyFilters(MOS_DialogArgs *args, const SDL_DialogFileFilter *filters, int nfilters)
{
    int i;

    args->filters = NULL;
    args->nfilters = 0;

    if (!filters || nfilters <= 0) {
        return true;
    }

    args->filters = (SDL_DialogFileFilter *)SDL_calloc((size_t)nfilters, sizeof(*args->filters));
    if (!args->filters) {
        SDL_OutOfMemory();
        return false;
    }

    args->nfilters = nfilters;

    for (i = 0; i < nfilters; i++) {
        if (filters[i].name) {
            args->filters[i].name = SDL_strdup(filters[i].name);
            if (!args->filters[i].name) {
                SDL_OutOfMemory();
                return false;
            }
        }

        if (filters[i].pattern) {
            args->filters[i].pattern = SDL_strdup(filters[i].pattern);
            if (!args->filters[i].pattern) {
                SDL_OutOfMemory();
                return false;
            }
        }
    }

    return true;
}

static bool MOS_SplitLocation(const char *loc, char **out_dir, char **out_file)
{
    const char *last_slash;
    const char *last_colon;
    const char *sep;
    size_t len;

    *out_dir = SDL_strdup("");
    *out_file = SDL_strdup("");

    if (!*out_dir || !*out_file) {
        SDL_free(*out_dir);
        SDL_free(*out_file);
        *out_dir = NULL;
        *out_file = NULL;
        SDL_OutOfMemory();
        return false;
    }

    if (!loc || !loc[0]) {
        return true;
    }

    len = SDL_strlen(loc);

    if (loc[len - 1] == ':' || loc[len - 1] == '/') {
        char *dir = SDL_strdup(loc);
        if (!dir) {
            SDL_OutOfMemory();
            return false;
        }

        SDL_free(*out_dir);
        *out_dir = dir;
        return true;
    }

    last_slash = SDL_strrchr(loc, '/');
    last_colon = SDL_strrchr(loc, ':');
    sep = last_slash;

    if (!sep || (last_colon && last_colon > sep)) {
        sep = last_colon;
    }

    if (sep) {
        const size_t dirlen = (size_t)(sep - loc) + 1;
        char *dir = (char *)SDL_calloc(dirlen + 1, 1);
        char *file = SDL_strdup(sep + 1);

        if (!dir || !file) {
            SDL_free(dir);
            SDL_free(file);
            SDL_OutOfMemory();
            return false;
        }

        SDL_memcpy(dir, loc, dirlen);
        dir[dirlen] = '\0';

        SDL_free(*out_dir);
        SDL_free(*out_file);
        *out_dir = dir;
        *out_file = file;
    } else {
        char *file = SDL_strdup(loc);
        if (!file) {
            SDL_OutOfMemory();
            return false;
        }

        SDL_free(*out_file);
        *out_file = file;
    }

    return true;
}

static void MOS_HandleMultiselection(struct FileRequester *req, MOS_DialogArgs *args)
{
    SDL_DialogFileCallback callback = args->callback;
    char **paths;
    size_t pathLen;
    const char *separator = "/";
    int i;

    paths = (char **)SDL_calloc((size_t)req->fr_NumArgs + 1, sizeof(char *));
    if (!paths) {
        D("Failed to allocate memory");
        SDL_OutOfMemory();
        callback(args->userdata, NULL, -1);
        return;
    }

    pathLen = SDL_strlen(req->fr_Drawer);

    if (pathLen > 0) {
        const char lastChar = req->fr_Drawer[pathLen - 1];
        if (lastChar == ':' || lastChar == '/') {
            separator = "";
        }
        pathLen++;
    }

    for (i = 0; i < req->fr_NumArgs; i++) {
        const char *filename = req->fr_ArgList[i].wa_Name;
        const size_t totalLen = pathLen + SDL_strlen(filename) + 1;

        paths[i] = (char *)SDL_calloc(totalLen, 1);
        if (!paths[i]) {
            D("Failed to allocate memory");
            MOS_FreePathList(paths, req->fr_NumArgs);
            SDL_OutOfMemory();
            callback(args->userdata, NULL, -1);
            return;
        }

        if (pathLen) {
            SDL_snprintf(paths[i], totalLen, "%s%s%s", req->fr_Drawer, separator, filename);
        } else {
            SDL_snprintf(paths[i], totalLen, "%s", filename);
        }

        D("[%d] '%s'", i, paths[i]);
    }

    paths[req->fr_NumArgs] = NULL;
    callback(args->userdata, (const char * const *)paths, -1);
    MOS_FreePathList(paths, req->fr_NumArgs);
}

static void MOS_HandleSingleFile(struct FileRequester *req, MOS_DialogArgs *args)
{
    SDL_DialogFileCallback callback = args->callback;
    size_t pathLen = SDL_strlen(req->fr_Drawer);

    if (pathLen > 0) {
        const char *separator = "/";
        const char *filename = req->fr_File;
        char *path;
        size_t totalLen;

        if (req->fr_Drawer[pathLen - 1] == ':' || req->fr_Drawer[pathLen - 1] == '/') {
            separator = "";
        }

        pathLen++;
        totalLen = pathLen + SDL_strlen(filename) + 1;
        path = (char *)SDL_calloc(totalLen, 1);

        if (!path) {
            D("Failed to allocate memory");
            SDL_OutOfMemory();
            callback(args->userdata, NULL, -1);
            return;
        }

        SDL_snprintf(path, totalLen, "%s%s%s", req->fr_Drawer, separator, filename);
        D("'%s'", path);

        {
            const char *paths[2] = { path, NULL };
            callback(args->userdata, paths, -1);
        }

        SDL_free(path);
    } else {
        const char *paths[2] = { req->fr_File, NULL };
        callback(args->userdata, paths, -1);
    }
}

static void MOS_ShowDialog(MOS_DialogArgs *args)
{
    SDL_DialogFileCallback callback = args->callback;
    struct FileRequester *req;

    D("title '%s', accept '%s', cancel '%s', default_dir '%s', default_file '%s'",
      args->title ? args->title : "(null)",
      args->accept ? args->accept : "(null)",
      args->cancel ? args->cancel : "(null)",
      args->default_dir ? args->default_dir : "(null)",
      args->default_file ? args->default_file : "(null)");

    req = AllocAslRequestTags(ASL_FileRequest,
        ASLFR_Window, args->window,
        ASLFR_TitleText, args->title,
        ASLFR_PositiveText, args->accept,
        ASLFR_NegativeText, args->cancel,
        ASLFR_InitialFile, args->default_file,
        ASLFR_InitialDrawer, args->default_dir,
        /* ASLFR_InitialPattern, TODO: filters */
        ASLFR_DoMultiSelect, args->allow_many,
        ASLFR_DoSaveMode, args->save,
        ASLFR_DrawersOnly, args->dir_only,
        ASLFR_PrivateIDCMP, TRUE,
        TAG_DONE);

    if (!req) {
        callback(args->userdata, NULL, -1);
        return;
    }

    if (AslRequestTags(req, TAG_DONE)) {
        D("Drawer '%s', file '%s', args %ld", req->fr_Drawer, req->fr_File, req->fr_NumArgs);

        if (req->fr_NumArgs > 0) {
            MOS_HandleMultiselection(req, args);
        } else if (args->dir_only) {
            const char *paths[2] = { req->fr_Drawer, NULL };
            callback(args->userdata, paths, -1);
        } else {
            MOS_HandleSingleFile(req, args);
        }
    } else {
        const char *files[1] = { NULL };
        callback(args->userdata, files, -1);
    }

    FreeAslRequest(req);
}

static int MOS_DialogThread(void *ptr)
{
    MOS_ShowDialog((MOS_DialogArgs *)ptr);
    MOS_FreeDialogArgs((MOS_DialogArgs *)ptr);
    return 0;
}

static MOS_DialogArgs *MOS_AllocDialogArgs(SDL_DialogFileCallback callback, void *userdata, struct Window *window,
                                           const char *title, const char *accept, const char *cancel)
{
    MOS_DialogArgs *args = (MOS_DialogArgs *)SDL_calloc(1, sizeof(*args));

    if (!args) {
        SDL_OutOfMemory();
        return NULL;
    }

    args->title = SDL_strdup(title ? title : "");
    args->accept = SDL_strdup(accept ? accept : MOS_DefaultAccept());
    args->cancel = SDL_strdup(cancel ? cancel : MOS_DefaultCancel());

    if (!args->title || !args->accept || !args->cancel) {
        SDL_OutOfMemory();
        MOS_FreeDialogArgs(args);
        return NULL;
    }

    args->window = window;
    args->callback = callback;
    args->userdata = userdata;

    return args;
}

static void MOS_StartDialogThread(MOS_DialogArgs *args, const char *thread_name)
{
    SDL_Thread *thread = SDL_CreateThread(MOS_DialogThread, thread_name, args);

    if (!thread) {
        args->callback(args->userdata, NULL, -1);
        MOS_FreeDialogArgs(args);
        return;
    }

    SDL_DetachThread(thread);
}

static void MOS_ShowFileDialog(SDL_DialogFileCallback callback, void *userdata, struct Window *window,
                               const SDL_DialogFileFilter *filters, int nfilters, const char *default_location,
                               bool allow_many, bool is_save, const char *title, const char *accept, const char *cancel)
{
    MOS_DialogArgs *args = MOS_AllocDialogArgs(callback, userdata, window,
                                               title ? title : (is_save ? "Save file..." : "Open file..."),
                                               accept,
                                               cancel);

    if (!args) {
        callback(userdata, NULL, -1);
        return;
    }

    if (!MOS_CopyFilters(args, filters, nfilters)) {
        callback(userdata, NULL, -1);
        MOS_FreeDialogArgs(args);
        return;
    }

    if (!MOS_SplitLocation(default_location, &args->default_dir, &args->default_file)) {
        callback(userdata, NULL, -1);
        MOS_FreeDialogArgs(args);
        return;
    }

    args->allow_many = allow_many;
    args->save = is_save;
    args->dir_only = false;

    MOS_StartDialogThread(args, "SDL_ShowFileDialog");
}

static void MOS_ShowFolderDialog(SDL_DialogFileCallback callback, void *userdata, struct Window *window,
                                 const char *default_location, bool allow_many,
                                 const char *title, const char *accept, const char *cancel)
{
    MOS_DialogArgs *args = MOS_AllocDialogArgs(callback, userdata, window,
                                               title ? title : "Open folder...",
                                               accept,
                                               cancel);

    if (!args) {
        callback(userdata, NULL, -1);
        return;
    }

    args->default_file = SDL_strdup("");
    args->default_dir = SDL_strdup(default_location ? default_location : "");

    if (!args->default_file || !args->default_dir) {
        SDL_OutOfMemory();
        callback(userdata, NULL, -1);
        MOS_FreeDialogArgs(args);
        return;
    }

    args->allow_many = allow_many;
    args->save = false;
    args->dir_only = true;

    MOS_StartDialogThread(args, "SDL_ShowFolderDialog");
}

void SDL_SYS_ShowFileDialogWithProperties(SDL_FileDialogType type, SDL_DialogFileCallback callback, void *userdata, SDL_PropertiesID props)
{
    SDL_Window *window = SDL_GetPointerProperty(props, SDL_PROP_FILE_DIALOG_WINDOW_POINTER, NULL);
    const SDL_DialogFileFilter *filters = (const SDL_DialogFileFilter *)
        SDL_GetPointerProperty(props, SDL_PROP_FILE_DIALOG_FILTERS_POINTER, NULL);
    const int nfilters = (int)SDL_GetNumberProperty(props, SDL_PROP_FILE_DIALOG_NFILTERS_NUMBER, 0);
    const bool allow_many = SDL_GetBooleanProperty(props, SDL_PROP_FILE_DIALOG_MANY_BOOLEAN, false);
    const char *default_location = SDL_GetStringProperty(props, SDL_PROP_FILE_DIALOG_LOCATION_STRING, NULL);
    const char *title = SDL_GetStringProperty(props, SDL_PROP_FILE_DIALOG_TITLE_STRING, NULL);
    const char *accept = SDL_GetStringProperty(props, SDL_PROP_FILE_DIALOG_ACCEPT_STRING, NULL);
    const char *cancel = SDL_GetStringProperty(props, SDL_PROP_FILE_DIALOG_CANCEL_STRING, NULL);
    struct Window *syswin = NULL;

    if (window) {
        SDL_PropertiesID windowProps = SDL_GetWindowProperties(window);
        syswin = SDL_GetPointerProperty(windowProps, "SDL.window.morphos.window", NULL);
    }

    switch (type) {
    case SDL_FILEDIALOG_SAVEFILE:
        MOS_ShowFileDialog(callback, userdata, syswin, filters, nfilters,
                           default_location, allow_many, true,
                           title, accept, cancel);
        break;

    case SDL_FILEDIALOG_OPENFILE:
        MOS_ShowFileDialog(callback, userdata, syswin, filters, nfilters,
                           default_location, allow_many, false,
                           title, accept, cancel);
        break;

    case SDL_FILEDIALOG_OPENFOLDER:
        MOS_ShowFolderDialog(callback, userdata, syswin,
                             default_location, allow_many,
                             title, accept, cancel);
        break;

    default:
        SDL_SetError("Unsupported file dialog type");
        callback(userdata, NULL, -1);
        break;
    }
}

#endif