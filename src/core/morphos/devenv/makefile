# Client-side auto-open glue for sdl3.library.
#
# Produces the small -lSDL3 archives an application links against:
#
#   lib/libSDL3.a           normal
#   lib/libb32/libSDL3.a    -mresident32 (baserel32 executables)
#   lib/libb32/libSDL3-nc.a -mresident32, application opens the library itself
#
# Each archive = cvinclude-generated LVO glue (from ../sdk/fd/sdl3_lib.fd)
#              + sdl-startup.o (constructor + variadic wrappers).

CC      = ppc-morphos-gcc-9 -noixemul
INCLUDE = -I../../../../include -I../../../../include/build_config \
          -I../../../../include/SDL3 -I../sdk
CFLAGS  = -mcpu=750 -mtune=7450 -O2 $(INCLUDE) -Wall -fno-strict-aliasing
AR      = ppc-morphos-ar
CVI     = /gg/bin/cvinclude.pl
FD      = ../sdk/fd/sdl3_lib.fd
CLIB    = ../sdk/clib/sdl3_protos.h

all: lib/libSDL3.a lib/libb32/libSDL3.a lib/libb32/libSDL3-nc.a

install: all
install-iso: all

sdl-startup.o: sdl-startup.c sdl-stubs.c
	$(CC) $(CFLAGS) -o $@ -c sdl-startup.c

sdl-startup-brel.o: sdl-startup.c sdl-stubs.c
	$(CC) $(CFLAGS) -mresident32 -o $@ -c sdl-startup.c

sdl-startup-brel-nc.o: sdl-startup.c sdl-stubs.c
	$(CC) $(CFLAGS) -mresident32 -D__NO_SDL_CONSTRUCTORS -o $@ -c sdl-startup.c

lib/libSDL3.a: sdl-startup.o $(FD)
	@mkdir -p lib
	@-rm -f $@
	$(CVI) --fd=$(FD) --clib=$(CLIB) --gluelib=$@
	$(AR) crus $@ sdl-startup.o

lib/libb32/libSDL3.a: sdl-startup-brel.o $(FD)
	@mkdir -p lib/libb32
	@-rm -f $@
	$(CVI) --fd=$(FD) --clib=$(CLIB) --brelgluelib=$@
	$(AR) crus $@ sdl-startup-brel.o

lib/libb32/libSDL3-nc.a: sdl-startup-brel-nc.o $(FD)
	@mkdir -p lib/libb32
	@-rm -f $@
	$(CVI) --fd=$(FD) --clib=$(CLIB) --brelgluelib=$@
	$(AR) crus $@ sdl-startup-brel-nc.o

clean:
	@-rm -rf lib *.o
