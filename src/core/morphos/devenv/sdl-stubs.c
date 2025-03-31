#include <stdio.h>

#include <proto/sdl3.h>

/* SDL_string.c */

int
SDL_sscanf(const char *text, const char *fmt, ...)
{
    int rc;
    va_list ap;
    va_start(ap, fmt);
    rc = SDL_vsscanf(text, fmt, ap);
    va_end(ap);
    return rc;
}

int
SDL_snprintf(char *text, size_t maxlen, const char *fmt, ...)
{
    va_list ap;
    int retval;

    va_start(ap, fmt);
    retval = SDL_vsnprintf(text, maxlen, fmt, ap);
    va_end(ap);

    return retval;
}

/* SDL_error.c */

bool
SDL_SetError(const char *fmt, ...)
{
    va_list ap;
    bool result;

    va_start(ap, fmt);
    result = SDL_VSetError(fmt, ap);
    va_end(ap);
    return result;
}

/* SDL_log.c */

void
SDL_Log(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, fmt, ap);
    va_end(ap);
}

void SDL_LogTrace(int category, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    SDL_LogMessageV(category, SDL_LOG_PRIORITY_TRACE, fmt, ap);
    va_end(ap);
}

void
SDL_LogVerbose(int category, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    SDL_LogMessageV(category, SDL_LOG_PRIORITY_VERBOSE, fmt, ap);
    va_end(ap);
}

void
SDL_LogDebug(int category, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    SDL_LogMessageV(category, SDL_LOG_PRIORITY_DEBUG, fmt, ap);
    va_end(ap);
}

void
SDL_LogInfo(int category, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    SDL_LogMessageV(category, SDL_LOG_PRIORITY_INFO, fmt, ap);
    va_end(ap);
}

void
SDL_LogWarn(int category, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    SDL_LogMessageV(category, SDL_LOG_PRIORITY_WARN, fmt, ap);
    va_end(ap);
}

void
SDL_LogError(int category, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    SDL_LogMessageV(category, SDL_LOG_PRIORITY_ERROR, fmt, ap);
    va_end(ap);
}

void
SDL_LogCritical(int category, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    SDL_LogMessageV(category, SDL_LOG_PRIORITY_CRITICAL, fmt, ap);
    va_end(ap);
}

void
SDL_LogMessage(int category, SDL_LogPriority priority, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    SDL_LogMessageV(category, priority, fmt, ap);
    va_end(ap);
}

int
SDL_swprintf(wchar_t *text, size_t maxlen, const wchar_t *fmt, ...)
{
    va_list ap;
    int retval;

    va_start(ap, fmt);
    retval = SDL_vswprintf(text, maxlen, fmt, ap);
    va_end(ap);

    return retval;
}

int SDL_asprintf(char **strp, const char *fmt, ...)
{
    va_list ap;
    int retval;

    va_start(ap, fmt);
    retval = SDL_vasprintf(strp, fmt, ap);
    va_end(ap);

    return retval;
}

size_t SDL_IOprintf(SDL_IOStream *context, const char *fmt, ...)
{
    va_list ap;
    int size;
    char *string;
    size_t bytes;

    va_start(ap, fmt);
    size = SDL_vasprintf(&string, fmt, ap);
    va_end(ap);
    if (size < 0) {
        return 0;
    }

    bytes = SDL_WriteIO(context, string, (size_t)size);
    SDL_free(string);
    return bytes;
}

/* SDL_render.c */

bool SDL_RenderDebugTextFormat(SDL_Renderer *renderer, float x, float y, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    // fast path to avoid unnecessary allocation and copy. If you're going through the dynapi, there's a good chance
    // you _always_ hit this path, since it probably had to process varargs before calling into the jumptable.
    if (SDL_strcmp(fmt, "%s") == 0) {
        const char *str = va_arg(ap, const char *);
        va_end(ap);
        return SDL_RenderDebugText(renderer, x, y, str);
    }

    char *str = NULL;
    const int rc = SDL_vasprintf(&str, fmt, ap);
    va_end(ap);

    if (rc == -1) {
        return false;
    }

    const bool retval = SDL_RenderDebugText(renderer, x, y, str);
    SDL_free(str);
    return retval;
}
