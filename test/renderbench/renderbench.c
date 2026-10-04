/*
  Copyright (C) 1997-2026 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.
*/

/* renderbench: pure 2D SDL_Renderer benchmark.
 *
 * Runs the same 2D scenes with each requested render driver (by default
 * "software", "opengl" and "overlay") in a new window, and prints the frame
 * rate of every scene, then a summary table.
 *
 * ESC skips the current renderer, closing the window stops everything.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "SDL.h"

#ifdef __MORPHOS__
unsigned long __stack = 256 * 1024;
const char *version_tag = "$VER: renderbench 1.0 (4.10.2026)";
#endif

#define MAX_RENDERERS 8
#define NUM_SCENES    5
#define SPRITE_SIZE   32
#define TILE_SIZE     16
#define MAP_SIZE      64
#define WARMUP_MS     500

typedef struct
{
    const char *name;
    const char *description;
} SceneInfo;

static const SceneInfo scene_info[NUM_SCENES] = {
    { "stream",     "1 streaming texture updated and copied per frame (emulators, DOS ports)" },
    { "tiles",      "scrolling 16x16 tile map, opaque copies" },
    { "sprites",    "opaque background + alpha blended 32x32 sprites" },
    { "primitives", "alpha blended rectangles, lines and points" },
    { "rotate",     "background + rotated and scaled sprites (RenderCopyEx)" },
};

typedef struct
{
    SDL_bool ran;
    int frames;
    double seconds;
    double present_ms;   /* average time spent in SDL_RenderPresent() */
} Result;

typedef struct
{
    float x, y, vx, vy;
    float angle, spin;
    SDL_Color color;
} Item;

/* Options */
static int window_w = 960, window_h = 720;
static int logical_w = 320, logical_h = 240;
static SDL_bool fullscreen = SDL_FALSE;
static SDL_bool vsync = SDL_FALSE;
static SDL_bool batching = SDL_TRUE;
static SDL_bool linear = SDL_FALSE;
static int seconds = 5;
static int num_items = 200;
static SDL_bool scene_enabled[NUM_SCENES];

static const char *renderer_names[MAX_RENDERERS];
static int num_renderers = 0;
static Result results[MAX_RENDERERS][NUM_SCENES];
static SDL_bool quit_all = SDL_FALSE;

/* Per renderer state */
static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *stream_texture, *background, *sprite, *tiles;
static Uint32 *stream_source;           /* (2 * w) x (2 * h) image scrolled into the stream texture */
static Uint8 tile_map[MAP_SIZE][MAP_SIZE];
static Item *items;
static int scene_w, scene_h;            /* drawing area: logical size, or window size */

static Uint32 random_state;

static Uint32 Random(void)
{
    random_state ^= random_state << 13;
    random_state ^= random_state >> 17;
    random_state ^= random_state << 5;
    return random_state;
}

static float RandomFloat(float min, float max)
{
    return min + (max - min) * (float)(Random() % 10000) / 10000.0f;
}

/* -------------------------------------------------------------------------
 * Generated images
 * ------------------------------------------------------------------------- */

static SDL_Texture *CreateTextureFromPixels(Uint32 format, int w, int h, const Uint32 *pixels)
{
    SDL_Texture *texture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_STATIC, w, h);
    if (texture) {
        SDL_UpdateTexture(texture, NULL, pixels, w * 4);
    }
    return texture;
}

static SDL_bool CreateTextures(void)
{
    const int sw = scene_w * 2, sh = scene_h * 2;
    Uint32 *pixels;
    int x, y, i;

    /* Plasma for the streaming texture */
    stream_source = (Uint32 *)SDL_malloc((size_t)sw * sh * sizeof(Uint32));
    if (!stream_source) {
        return SDL_FALSE;
    }
    for (y = 0; y < sh; y++) {
        for (x = 0; x < sw; x++) {
            const double v = sin(x * 0.031) + sin(y * 0.047) + sin((x + y) * 0.023) + sin(sqrt((double)(x * x + y * y)) * 0.05);
            const int r = (int)(128 + 127 * sin(v * 3.14159));
            const int g = (int)(128 + 127 * sin(v * 3.14159 + 2.094));
            const int b = (int)(128 + 127 * sin(v * 3.14159 + 4.188));
            stream_source[y * sw + x] = 0xFF000000 | (r << 16) | (g << 8) | b;
        }
    }
    stream_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, scene_w, scene_h);

    /* Background: gradient with a grid */
    pixels = (Uint32 *)SDL_malloc((size_t)scene_w * scene_h * sizeof(Uint32));
    if (!pixels) {
        return SDL_FALSE;
    }
    for (y = 0; y < scene_h; y++) {
        for (x = 0; x < scene_w; x++) {
            Uint32 c = 0xFF000000 | ((x * 255 / scene_w) << 16) | ((y * 128 / scene_h) << 8) | 96;
            if ((x % 32) == 0 || (y % 32) == 0) {
                c = 0xFFE0E0E0;
            }
            pixels[y * scene_w + x] = c;
        }
    }
    background = CreateTextureFromPixels(SDL_PIXELFORMAT_RGB888, scene_w, scene_h, pixels);
    SDL_free(pixels);

    /* Sprite: soft edged ball */
    pixels = (Uint32 *)SDL_malloc(SPRITE_SIZE * SPRITE_SIZE * sizeof(Uint32));
    if (!pixels) {
        return SDL_FALSE;
    }
    for (y = 0; y < SPRITE_SIZE; y++) {
        for (x = 0; x < SPRITE_SIZE; x++) {
            const float dx = x - SPRITE_SIZE / 2 + 0.5f, dy = y - SPRITE_SIZE / 2 + 0.5f;
            const float d = SDL_sqrtf(dx * dx + dy * dy) / (SPRITE_SIZE / 2);
            const int a = d >= 1.0f ? 0 : (int)(255 * (1.0f - d * d));
            const int l = (int)(255 - 160 * d);
            pixels[y * SPRITE_SIZE + x] = ((Uint32)a << 24) | (SDL_max(l, 0) << 16) | (SDL_max(l - 60, 0) << 8) | 40;
        }
    }
    sprite = CreateTextureFromPixels(SDL_PIXELFORMAT_ARGB8888, SPRITE_SIZE, SPRITE_SIZE, pixels);
    SDL_free(pixels);
    if (sprite) {
        SDL_SetTextureBlendMode(sprite, SDL_BLENDMODE_BLEND);
    }

    /* Tile atlas: 4x4 tiles of 16x16 */
    pixels = (Uint32 *)SDL_malloc(4 * TILE_SIZE * 4 * TILE_SIZE * sizeof(Uint32));
    if (!pixels) {
        return SDL_FALSE;
    }
    for (y = 0; y < 4 * TILE_SIZE; y++) {
        for (x = 0; x < 4 * TILE_SIZE; x++) {
            const int tile = (y / TILE_SIZE) * 4 + x / TILE_SIZE;
            const int tx = x % TILE_SIZE, ty = y % TILE_SIZE;
            Uint32 c = 0xFF000000 | ((tile * 53 % 256) << 16) | ((tile * 97 % 256) << 8) | (tile * 151 % 256);
            if (tx == 0 || ty == 0 || ((tx + ty + tile) % 7) == 0) {
                c = (c & 0xFF000000) | ((c >> 1) & 0x7F7F7F);
            }
            pixels[y * 4 * TILE_SIZE + x] = c;
        }
    }
    tiles = CreateTextureFromPixels(SDL_PIXELFORMAT_RGB888, 4 * TILE_SIZE, 4 * TILE_SIZE, pixels);
    SDL_free(pixels);

    random_state = 12345;
    for (y = 0; y < MAP_SIZE; y++) {
        for (x = 0; x < MAP_SIZE; x++) {
            tile_map[y][x] = (Uint8)(Random() % 16);
        }
    }

    items = (Item *)SDL_calloc(num_items, sizeof(Item));
    if (!items) {
        return SDL_FALSE;
    }
    (void)i;
    return (stream_texture && background && sprite && tiles) ? SDL_TRUE : SDL_FALSE;
}

static void DestroyTextures(void)
{
    if (stream_texture) {
        SDL_DestroyTexture(stream_texture);
    }
    if (background) {
        SDL_DestroyTexture(background);
    }
    if (sprite) {
        SDL_DestroyTexture(sprite);
    }
    if (tiles) {
        SDL_DestroyTexture(tiles);
    }
    stream_texture = background = sprite = tiles = NULL;
    SDL_free(stream_source);
    stream_source = NULL;
    SDL_free(items);
    items = NULL;
}

/* Same positions and colors for every renderer */
static void ResetItems(void)
{
    int i;

    random_state = 2026;
    for (i = 0; i < num_items; i++) {
        Item *it = &items[i];
        it->x = RandomFloat(0, (float)(scene_w - SPRITE_SIZE));
        it->y = RandomFloat(0, (float)(scene_h - SPRITE_SIZE));
        it->vx = RandomFloat(-2.0f, 2.0f);
        it->vy = RandomFloat(-2.0f, 2.0f);
        it->angle = RandomFloat(0, 360);
        it->spin = RandomFloat(-4.0f, 4.0f);
        it->color.r = (Uint8)(Random() % 256);
        it->color.g = (Uint8)(Random() % 256);
        it->color.b = (Uint8)(Random() % 256);
        it->color.a = 128;
    }
}

static void MoveItems(int count, int size)
{
    int i;

    for (i = 0; i < count; i++) {
        Item *it = &items[i];
        it->x += it->vx;
        it->y += it->vy;
        if (it->x < 0 || it->x > scene_w - size) {
            it->vx = -it->vx;
            it->x += it->vx;
        }
        if (it->y < 0 || it->y > scene_h - size) {
            it->vy = -it->vy;
            it->y += it->vy;
        }
        it->angle += it->spin;
    }
}

/* -------------------------------------------------------------------------
 * Scenes
 * ------------------------------------------------------------------------- */

static void DrawStream(int frame)
{
    const int sw = scene_w * 2;
    const int ox = (int)((sin(frame * 0.013) + 1.0) * 0.5 * scene_w);
    const int oy = (int)((cos(frame * 0.017) + 1.0) * 0.5 * scene_h);
    void *pixels;
    int pitch, y;

    if (SDL_LockTexture(stream_texture, NULL, &pixels, &pitch) == 0) {
        for (y = 0; y < scene_h; y++) {
            SDL_memcpy((Uint8 *)pixels + y * pitch, &stream_source[(y + oy) * sw + ox], scene_w * 4);
        }
        SDL_UnlockTexture(stream_texture);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, stream_texture, NULL, NULL);
}

static void DrawTiles(int frame)
{
    const int px = frame % (MAP_SIZE * TILE_SIZE);
    const int py = (frame / 2) % (MAP_SIZE * TILE_SIZE);
    const int cols = scene_w / TILE_SIZE + 2, rows = scene_h / TILE_SIZE + 2;
    int x, y;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    for (y = 0; y < rows; y++) {
        for (x = 0; x < cols; x++) {
            const int tile = tile_map[(py / TILE_SIZE + y) % MAP_SIZE][(px / TILE_SIZE + x) % MAP_SIZE];
            SDL_Rect src, dst;
            src.x = (tile % 4) * TILE_SIZE;
            src.y = (tile / 4) * TILE_SIZE;
            src.w = src.h = TILE_SIZE;
            dst.x = x * TILE_SIZE - px % TILE_SIZE;
            dst.y = y * TILE_SIZE - py % TILE_SIZE;
            dst.w = dst.h = TILE_SIZE;
            SDL_RenderCopy(renderer, tiles, &src, &dst);
        }
    }
}

static void DrawSprites(int frame)
{
    int i;

    MoveItems(num_items, SPRITE_SIZE);
    SDL_RenderCopy(renderer, background, NULL, NULL);
    for (i = 0; i < num_items; i++) {
        SDL_Rect dst;
        dst.x = (int)items[i].x;
        dst.y = (int)items[i].y;
        dst.w = dst.h = SPRITE_SIZE;
        SDL_RenderCopy(renderer, sprite, NULL, &dst);
    }
    (void)frame;
}

static void DrawPrimitives(int frame)
{
    SDL_Point points[64];
    int i, j;

    MoveItems(num_items, SPRITE_SIZE);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, 16, 16, 48, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    for (i = 0; i < num_items; i++) {
        const Item *it = &items[i];
        SDL_Rect r;
        r.x = (int)it->x;
        r.y = (int)it->y;
        r.w = r.h = SPRITE_SIZE;
        SDL_SetRenderDrawColor(renderer, it->color.r, it->color.g, it->color.b, it->color.a);
        SDL_RenderFillRect(renderer, &r);
    }
    for (i = 0; i + 1 < num_items; i += 2) {
        const Item *a = &items[i], *b = &items[i + 1];
        SDL_SetRenderDrawColor(renderer, a->color.r, a->color.g, a->color.b, 200);
        SDL_RenderDrawLine(renderer, (int)a->x, (int)a->y, (int)b->x, (int)b->y);
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (i = 0; i < num_items; i += 64) {
        const int n = SDL_min(64, num_items - i);
        for (j = 0; j < n; j++) {
            points[j].x = (int)items[i + j].x + SPRITE_SIZE / 2;
            points[j].y = (int)items[i + j].y + SPRITE_SIZE / 2;
        }
        SDL_RenderDrawPoints(renderer, points, n);
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    (void)frame;
}

static void DrawRotate(int frame)
{
    const int count = SDL_max(num_items / 4, 1);
    const int size = SPRITE_SIZE * 3 / 2;
    int i;

    MoveItems(count, size);
    SDL_RenderCopy(renderer, background, NULL, NULL);
    for (i = 0; i < count; i++) {
        SDL_Rect dst;
        dst.x = (int)items[i].x;
        dst.y = (int)items[i].y;
        dst.w = dst.h = size;
        SDL_RenderCopyEx(renderer, sprite, NULL, &dst, items[i].angle, NULL, SDL_FLIP_NONE);
    }
    (void)frame;
}

static void DrawScene(int scene, int frame)
{
    switch (scene) {
    case 0:
        DrawStream(frame);
        break;
    case 1:
        DrawTiles(frame);
        break;
    case 2:
        DrawSprites(frame);
        break;
    case 3:
        DrawPrimitives(frame);
        break;
    case 4:
        DrawRotate(frame);
        break;
    }
}

/* -------------------------------------------------------------------------
 * Benchmark
 * ------------------------------------------------------------------------- */

/* SDL_FALSE when the renderer has to stop */
static SDL_bool HandleEvents(void)
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT ||
            (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)) {
            quit_all = SDL_TRUE;
            return SDL_FALSE;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            return SDL_FALSE;
        }
    }
    return SDL_TRUE;
}

static SDL_bool RunScene(const char *renderer_name, int scene, Result *result)
{
    const Uint64 freq = SDL_GetPerformanceFrequency();
    const Uint64 duration = freq * seconds;
    Uint64 start, now, last_title, present = 0;
    int frame = 0, frames = 0, title_frames = 0;
    char title[128];

    ResetItems();

    /* Warm up: first texture uploads, overlay setup... */
    start = SDL_GetPerformanceCounter();
    while (SDL_GetPerformanceCounter() - start < freq * WARMUP_MS / 1000) {
        if (!HandleEvents()) {
            return SDL_FALSE;
        }
        DrawScene(scene, frame++);
        SDL_RenderPresent(renderer);
    }

    start = last_title = SDL_GetPerformanceCounter();
    do {
        Uint64 t0;

        if (!HandleEvents()) {
            return SDL_FALSE;
        }
        DrawScene(scene, frame++);
        t0 = SDL_GetPerformanceCounter();
        SDL_RenderPresent(renderer);
        now = SDL_GetPerformanceCounter();
        present += now - t0;
        frames++;
        title_frames++;

        if (now - last_title >= freq) {
            SDL_snprintf(title, sizeof(title), "renderbench - %s - %s - %.1f fps", renderer_name, scene_info[scene].name,
                         title_frames * (double)freq / (now - last_title));
            SDL_SetWindowTitle(window, title);
            last_title = now;
            title_frames = 0;
        }
    } while (now - start < duration);

    result->ran = SDL_TRUE;
    result->frames = frames;
    result->seconds = (double)(now - start) / freq;
    result->present_ms = 1000.0 * present / freq / frames;

    printf("  %-10s %6d frames in %5.2f s = %7.1f fps  (%6.2f ms/frame, %6.2f ms in present)\n",
           scene_info[scene].name, frames, result->seconds, frames / result->seconds,
           1000.0 * result->seconds / frames, result->present_ms);
    fflush(stdout);
    return SDL_TRUE;
}

static void RunRenderer(int index)
{
    const char *name = renderer_names[index];
    Uint32 window_flags = SDL_WINDOW_SHOWN;
    SDL_RendererInfo info;
    int output_w, output_h, scene;

    printf("\n%s\n", name);

    if (SDL_strcasecmp(name, "opengl") == 0) {
        window_flags |= SDL_WINDOW_OPENGL;
    }
    if (fullscreen) {
        window_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    /* Override: the MorphOS window menu sets these hints from ENV: variables */
    SDL_SetHintWithPriority(SDL_HINT_RENDER_DRIVER, name, SDL_HINT_OVERRIDE);

    window = SDL_CreateWindow("renderbench", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              window_w, window_h, window_flags);
    if (!window) {
        printf("  no window: %s\n", SDL_GetError());
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, vsync ? SDL_RENDERER_PRESENTVSYNC : 0);
    if (!renderer) {
        printf("  no renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        window = NULL;
        return;
    }
    SDL_GetRendererInfo(renderer, &info);
    if (SDL_strcasecmp(info.name, name) != 0) {
        printf("  not available (SDL gave \"%s\"), skipped\n", info.name);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        renderer = NULL;
        window = NULL;
        return;
    }

    SDL_GetRendererOutputSize(renderer, &output_w, &output_h);
    if (logical_w > 0 && logical_h > 0) {
        SDL_RenderSetLogicalSize(renderer, logical_w, logical_h);
        scene_w = logical_w;
        scene_h = logical_h;
    } else {
        scene_w = output_w;
        scene_h = output_h;
    }
    printf("  output %dx%d, drawing %dx%d, flags:%s%s%s%s\n", output_w, output_h, scene_w, scene_h,
           (info.flags & SDL_RENDERER_SOFTWARE) ? " software" : "",
           (info.flags & SDL_RENDERER_ACCELERATED) ? " accelerated" : "",
           (info.flags & SDL_RENDERER_PRESENTVSYNC) ? " vsync" : "",
           (info.flags & SDL_RENDERER_TARGETTEXTURE) ? " targets" : "");
    fflush(stdout);

    if (CreateTextures()) {
        for (scene = 0; scene < NUM_SCENES && !quit_all; scene++) {
            if (scene_enabled[scene] && !RunScene(name, scene, &results[index][scene])) {
                break;
            }
        }
    } else {
        printf("  couldn't create the textures: %s\n", SDL_GetError());
    }

    DestroyTextures();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = NULL;
    window = NULL;
}

static void PrintSummary(void)
{
    int i, scene;

    printf("\nframes per second (ms per frame)\n%-11s", "");
    for (i = 0; i < num_renderers; i++) {
        printf(" %21s", renderer_names[i]);
    }
    printf("\n");
    for (scene = 0; scene < NUM_SCENES; scene++) {
        if (!scene_enabled[scene]) {
            continue;
        }
        printf("%-11s", scene_info[scene].name);
        for (i = 0; i < num_renderers; i++) {
            const Result *r = &results[i][scene];
            if (r->ran) {
                printf("  %8.1f (%6.2f ms)", r->frames / r->seconds, 1000.0 * r->seconds / r->frames);
            } else {
                printf(" %21s", "-");
            }
        }
        printf("\n");
    }
}

static void Usage(const char *argv0)
{
    int i;

    printf("Usage: %s [options] [renderer...]\n"
           "Renderers: software opengl overlay (default: all three)\n"
           "  -window WxH    window size (default 960x720)\n"
           "  -logical WxH   logical size, 0x0 to draw at window size (default 320x240)\n"
           "  -fullscreen    fullscreen desktop window\n"
           "  -vsync         wait for the vertical blank (off by default to measure throughput)\n"
           "  -nobatch       disable render batching\n"
           "  -linear        linear texture filtering (nearest by default)\n"
           "  -seconds N     duration of each scene (default 5)\n"
           "  -items N       sprites, rectangles... per frame (default 200)\n"
           "  -scene NAME    only run this scene, can be repeated\n"
           "Scenes:\n", argv0);
    for (i = 0; i < NUM_SCENES; i++) {
        printf("  %-10s %s\n", scene_info[i].name, scene_info[i].description);
    }
    printf("ESC skips the current renderer.\n");
}

static SDL_bool ParseSize(const char *s, int *w, int *h)
{
    return (s && sscanf(s, "%dx%d", w, h) == 2 && *w >= 0 && *h >= 0) ? SDL_TRUE : SDL_FALSE;
}

int main(int argc, char *argv[])
{
    SDL_bool scene_option = SDL_FALSE;
    SDL_DisplayMode mode;
    SDL_version linked;
    int i, scene;

    for (scene = 0; scene < NUM_SCENES; scene++) {
        scene_enabled[scene] = SDL_TRUE;
    }

    for (i = 1; i < argc; i++) {
        const char *arg = argv[i];
        const char *next = (i + 1 < argc) ? argv[i + 1] : NULL;

        if (SDL_strcmp(arg, "-window") == 0 && ParseSize(next, &window_w, &window_h)) {
            i++;
        } else if (SDL_strcmp(arg, "-logical") == 0 && ParseSize(next, &logical_w, &logical_h)) {
            i++;
        } else if (SDL_strcmp(arg, "-fullscreen") == 0) {
            fullscreen = SDL_TRUE;
        } else if (SDL_strcmp(arg, "-vsync") == 0) {
            vsync = SDL_TRUE;
        } else if (SDL_strcmp(arg, "-nobatch") == 0) {
            batching = SDL_FALSE;
        } else if (SDL_strcmp(arg, "-linear") == 0) {
            linear = SDL_TRUE;
        } else if (SDL_strcmp(arg, "-seconds") == 0 && next && SDL_atoi(next) > 0) {
            seconds = SDL_atoi(next);
            i++;
        } else if (SDL_strcmp(arg, "-items") == 0 && next && SDL_atoi(next) > 0) {
            num_items = SDL_atoi(next);
            i++;
        } else if (SDL_strcmp(arg, "-scene") == 0 && next) {
            int found = -1;
            for (scene = 0; scene < NUM_SCENES; scene++) {
                if (SDL_strcasecmp(next, scene_info[scene].name) == 0) {
                    found = scene;
                }
            }
            if (found < 0) {
                Usage(argv[0]);
                return 1;
            }
            if (!scene_option) {
                for (scene = 0; scene < NUM_SCENES; scene++) {
                    scene_enabled[scene] = SDL_FALSE;
                }
                scene_option = SDL_TRUE;
            }
            scene_enabled[found] = SDL_TRUE;
            i++;
        } else if (arg[0] != '-' && num_renderers < MAX_RENDERERS) {
            renderer_names[num_renderers++] = arg;
        } else {
            Usage(argv[0]);
            return 1;
        }
    }
    if (num_renderers == 0) {
        renderer_names[num_renderers++] = "software";
        renderer_names[num_renderers++] = "opengl";
        renderer_names[num_renderers++] = "overlay";
    }

    SDL_SetHintWithPriority(SDL_HINT_RENDER_VSYNC, vsync ? "1" : "0", SDL_HINT_OVERRIDE);
    SDL_SetHintWithPriority(SDL_HINT_RENDER_BATCHING, batching ? "1" : "0", SDL_HINT_OVERRIDE);
    SDL_SetHintWithPriority(SDL_HINT_RENDER_SCALE_QUALITY, linear ? "linear" : "nearest", SDL_HINT_OVERRIDE);
    SDL_SetHintWithPriority(SDL_HINT_RENDER_LOGICAL_SIZE_MODE, "letterbox", SDL_HINT_OVERRIDE);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GetVersion(&linked);
    printf("renderbench, SDL %d.%d.%d, video driver %s\n", linked.major, linked.minor, linked.patch,
           SDL_GetCurrentVideoDriver());
    if (SDL_GetDesktopDisplayMode(0, &mode) == 0) {
        printf("desktop %dx%d %d Hz %s\n", mode.w, mode.h, mode.refresh_rate, SDL_GetPixelFormatName(mode.format));
    }
    printf("window %dx%d%s, logical %dx%d, %d items, vsync %s, batching %s, %s filtering, %d s per scene\n",
           window_w, window_h, fullscreen ? " fullscreen" : "", logical_w, logical_h, num_items,
           vsync ? "on" : "off", batching ? "on" : "off", linear ? "linear" : "nearest", seconds);
    fflush(stdout);

    for (i = 0; i < num_renderers && !quit_all; i++) {
        RunRenderer(i);
    }

    PrintSummary();
    SDL_Quit();
    return 0;
}

/* vi: set ts=4 sw=4 expandtab: */
