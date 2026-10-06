renderbench - pure 2D SDL_Renderer benchmark, SDL3 version
==========================================================

Port of test/renderbench of the SDL2 tree: same scenes, same options, same
output, to compare SDL2 and SDL3 on the same machine. See the SDL2 README
for the scenes and the options.

Build (cross compiler, after building libSDL3.a):  make -f Makefile.mos

Differences with the SDL2 version:
  - logical size: SDL_SetRenderLogicalPresentation(), LETTERBOX mode
    (SDL2: SDL_RenderSetLogicalSize() with the "letterbox" hint)
  - filtering: SDL_SetDefaultTextureScaleMode() on the renderer
    (SDL2: SDL_HINT_RENDER_SCALE_QUALITY)
  - -nobatch and -noshaders are accepted but ignored: SDL3 always batches
    and has no OpenGL shaders hint
  - -format takes the SDL2 names (RGB888, BGR888, RGB555) and the SDL3 ones
    (XRGB8888, XBGR8888, XRGB1555)
  - positions stay integers, as in the SDL2 version, so both draw the same
    pixels

Example, the same command with both:
  renderbench -scene stream -scene yuv -scene static overlay
