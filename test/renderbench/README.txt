renderbench - pure 2D SDL_Renderer benchmark
=============================================

Runs the same 2D scenes with several render drivers, each in a new window,
and prints frames per second and milliseconds per frame:

  stream      1 streaming texture updated and copied per frame
              (emulators, DOS ports like Raptor)
  tiles       scrolling 16x16 tile map, opaque copies
  sprites     opaque background + alpha blended 32x32 sprites
  primitives  alpha blended rectangles, lines and points
  rotate      background + rotated and scaled sprites (RenderCopyEx)

Build (cross compiler):  make -f Makefile.mos

Usage:  renderbench [options] [renderer...]

  renderer       software, opengl, overlay (default: all three)
  -window WxH    window size (default 960x720)
  -logical WxH   logical size, 0x0 to draw at window size (default 320x240)
  -fullscreen    fullscreen desktop window
  -vsync         wait for the vertical blank (off by default: throughput)
  -nobatch       disable render batching
  -linear        linear texture filtering (nearest by default)
  -seconds N     duration of each scene (default 5)
  -items N       sprites, rectangles... per frame (default 200)
  -scene NAME    only run this scene, can be repeated

ESC skips the current renderer, closing the window stops everything.

The renderer, vsync, batching and filtering hints are set with
SDL_HINT_OVERRIDE, so the ENV: variables saved by the SDL window menu don't
change what is measured.

Examples:
  renderbench                          all renderers, all scenes
  renderbench -fullscreen overlay      overlay only, fullscreen
  renderbench -vsync -scene stream     check vsync pacing (expect the refresh rate)
  renderbench -logical 640x480 -items 500
