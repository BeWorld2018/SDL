/*
 * Variadic SDL3 functions: a library vector can't take "...", so they are
 * built here, on the caller's side, around their va_list version.
 * Included by sdl-startup.c.
 */

/* SDL_iostream.c */

size_t SDL_IOprintf(SDL_IOStream *context, SDL_PRINTF_FORMAT_STRING const char *fmt, ...)
{
    va_list ap;
    size_t retval;

    va_start(ap, fmt);
    retval = SDL_IOvprintf(context, fmt, ap);
    va_end(ap);

    return retval;
}

/* SDL_log.c */

void SDL_Log(SDL_PRINTF_FORMAT_STRING const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, fmt, ap);
    va_end(ap);
}

void SDL_LogMessage(int category, SDL_LogPriority priority, SDL_PRINTF_FORMAT_STRING const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    SDL_LogMessageV(category, priority, fmt, ap);
    va_end(ap);
}

#define SDL_LOG_IMPL(name, priority)                                              \
    void SDL_Log##name(int category, SDL_PRINTF_FORMAT_STRING const char *fmt, ...) \
    {                                                                             \
        va_list ap;                                                               \
        va_start(ap, fmt);                                                        \
        SDL_LogMessageV(category, SDL_LOG_PRIORITY_##priority, fmt, ap);          \
        va_end(ap);                                                               \
    }

SDL_LOG_IMPL(Trace, TRACE)
SDL_LOG_IMPL(Verbose, VERBOSE)
SDL_LOG_IMPL(Debug, DEBUG)
SDL_LOG_IMPL(Info, INFO)
SDL_LOG_IMPL(Warn, WARN)
SDL_LOG_IMPL(Error, ERROR)
SDL_LOG_IMPL(Critical, CRITICAL)

/* SDL_error.c */

bool SDL_SetError(SDL_PRINTF_FORMAT_STRING const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    SDL_SetErrorV(fmt, ap);
    va_end(ap);

    return false;
}

/* SDL_string.c */

int SDL_asprintf(char **strp, SDL_PRINTF_FORMAT_STRING const char *fmt, ...)
{
    va_list ap;
    int retval;

    va_start(ap, fmt);
    retval = SDL_vasprintf(strp, fmt, ap);
    va_end(ap);

    return retval;
}

int SDL_snprintf(SDL_OUT_Z_CAP(maxlen) char *text, size_t maxlen, SDL_PRINTF_FORMAT_STRING const char *fmt, ...)
{
    va_list ap;
    int retval;

    va_start(ap, fmt);
    retval = SDL_vsnprintf(text, maxlen, fmt, ap);
    va_end(ap);

    return retval;
}

int SDL_sscanf(const char *text, SDL_SCANF_FORMAT_STRING const char *fmt, ...)
{
    va_list ap;
    int retval;

    va_start(ap, fmt);
    retval = SDL_vsscanf(text, fmt, ap);
    va_end(ap);

    return retval;
}

int SDL_swprintf(SDL_OUT_Z_CAP(maxlen) wchar_t *text, size_t maxlen, SDL_PRINTF_FORMAT_STRING const wchar_t *fmt, ...)
{
    va_list ap;
    int retval;

    va_start(ap, fmt);
    retval = SDL_vswprintf(text, maxlen, fmt, ap);
    va_end(ap);

    return retval;
}

/* SDL_render.c */

bool SDL_RenderDebugTextFormat(SDL_Renderer *renderer, float x, float y, SDL_PRINTF_FORMAT_STRING const char *fmt, ...)
{
    va_list ap;
    char *str = NULL;
    bool retval;
    int rc;

    va_start(ap, fmt);

    /* same fast path as SDL: no allocation for a plain "%s" */
    if (SDL_strcmp(fmt, "%s") == 0) {
        const char *s = va_arg(ap, const char *);
        va_end(ap);
        return SDL_RenderDebugText(renderer, x, y, s);
    }

    rc = SDL_vasprintf(&str, fmt, ap);
    va_end(ap);

    if (rc == -1) {
        return false;
    }

    retval = SDL_RenderDebugText(renderer, x, y, str);
    SDL_free(str);

    return retval;
}
