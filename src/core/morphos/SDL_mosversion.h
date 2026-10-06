/*
 * sdl3.library version = SDL version: VERSION = SDL major,
 * REVISION = minor * 100 + micro (SDL 3.4.17 -> sdl3.library 3.417).
 * A program linked against a given SDL needs that sdl3.library or later
 * (LIB_MINVER in devenv/sdl-startup.c).
 *
 * SDL_LIB_MAJOR/MINOR/MICRO/REVISION come from include/SDL3/SDL_version.h,
 * passed by Makefile.mos and devenv/makefile: SDL_library.c is built
 * without the SDL include path.
 */
#if !defined(SDL_LIB_MAJOR) || !defined(SDL_LIB_MINOR) || !defined(SDL_LIB_MICRO) || !defined(SDL_LIB_REVISION)
#error "SDL_LIB_MAJOR/MINOR/MICRO/REVISION not defined (see Makefile.mos)"
#endif

#define	str(s) #s
#define xstr(s) str(s)
#define	VERSION		SDL_LIB_MAJOR
#define	REVISION	SDL_LIB_REVISION
#define	VERSTAG	"\0$VER: sdl3.library " xstr(VERSION) "." xstr(REVISION) " (" __AMIGADATE__ ") SDL " xstr(SDL_LIB_MAJOR) "." xstr(SDL_LIB_MINOR) "." xstr(SDL_LIB_MICRO) " (c) Bruno Peloille"
