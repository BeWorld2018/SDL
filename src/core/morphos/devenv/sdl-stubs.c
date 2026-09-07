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

#include <stdarg.h>
#include <proto/sdl3.h>

void SDL_Log(const char *fmt, ...)
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

void SDL_LogVerbose(int category, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	SDL_LogMessageV(category, SDL_LOG_PRIORITY_VERBOSE, fmt, ap);
	va_end(ap);
}

void SDL_LogDebug(int category, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	SDL_LogMessageV(category, SDL_LOG_PRIORITY_DEBUG, fmt, ap);
	va_end(ap);
}

void SDL_LogInfo(int category, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	SDL_LogMessageV(category, SDL_LOG_PRIORITY_INFO, fmt, ap);
	va_end(ap);
}

void SDL_LogWarn(int category, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	SDL_LogMessageV(category, SDL_LOG_PRIORITY_WARN, fmt, ap);
	va_end(ap);
}

void SDL_LogError(int category, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	SDL_LogMessageV(category, SDL_LOG_PRIORITY_ERROR, fmt, ap);
	va_end(ap);
}

void SDL_LogCritical(int category, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	SDL_LogMessageV(category, SDL_LOG_PRIORITY_CRITICAL, fmt, ap);
	va_end(ap);
}

void SDL_LogMessage(int category, SDL_LogPriority priority, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	SDL_LogMessageV(category, priority, fmt, ap);
	va_end(ap);
}

bool SDL_SetError(const char *fmt, ...)
{
	bool rc;
	va_list ap;
	va_start(ap, fmt);
	rc = SDL_SetErrorV(fmt, ap);
	va_end(ap);
	return rc;
}

int SDL_snprintf(char *text, size_t maxlen, const char *fmt, ...)
{
	int rc;
	va_list ap;
	va_start(ap, fmt);
	rc = SDL_vsnprintf(text, maxlen, fmt, ap);
	va_end(ap);
	return rc;
}

int SDL_swprintf(wchar_t *text, size_t maxlen, const wchar_t *fmt, ...)
{
	int rc;
	va_list ap;
	va_start(ap, fmt);
	rc = SDL_vswprintf(text, maxlen, fmt, ap);
	va_end(ap);
	return rc;
}

int SDL_asprintf(char **strp, const char *fmt, ...)
{
	int rc;
	va_list ap;
	va_start(ap, fmt);
	rc = SDL_vasprintf(strp, fmt, ap);
	va_end(ap);
	return rc;
}

int SDL_sscanf(const char *text, const char *fmt, ...)
{
	int rc;
	va_list ap;
	va_start(ap, fmt);
	rc = SDL_vsscanf(text, fmt, ap);
	va_end(ap);
	return rc;
}

size_t SDL_IOprintf(SDL_IOStream *context, const char *fmt, ...)
{
	size_t rc;
	va_list ap;
	va_start(ap, fmt);
	rc = SDL_IOvprintf(context, fmt, ap);
	va_end(ap);
	return rc;
}

bool SDL_RenderDebugTextFormat(SDL_Renderer *renderer, float x, float y, const char *fmt, ...)
{
	bool rc = false;
	char *str = NULL;
	va_list ap;
	va_start(ap, fmt);
	if (SDL_vasprintf(&str, fmt, ap) >= 0 && str) {
		rc = SDL_RenderDebugText(renderer, x, y, str);
		SDL_free(str);
	}
	va_end(ap);
	return rc;
}
