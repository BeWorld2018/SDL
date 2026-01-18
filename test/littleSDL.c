#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned long __stack = 1024 * 1024 * 2;

bool isFullscreen = false;

static void ToggleFullscreen(SDL_Window* w)
{
    isFullscreen = !isFullscreen;
	SDL_SetWindowFullscreenMode(w, NULL);
    SDL_SetWindowFullscreen(w, isFullscreen ? true : false);
}

/* ---- helpers drawing ---- */

static void DrawGrid(SDL_Renderer *r, int w, int h, int step)
{
    // Lines slightly lighter than background
    SDL_SetRenderDrawColor(r, 30, 30, 255, 255);
    for (int x = 0; x < w; x += step) {
        SDL_RenderLine(r, (float)x, 0.0f, (float)x, (float)h);
    }
    for (int y = 0; y < h; y += step) {
        SDL_RenderLine(r, 0.0f, (float)y, (float)w, (float)y);
    }
}

static void DrawCircle(SDL_Renderer *r, float cx, float cy, float radius, int segments)
{
    if (segments < 12) segments = 12;
    float prevx = cx + radius;
    float prevy = cy;

    for (int i = 1; i <= segments; i++) {
        float a = (float)i * (2.0f * (float)M_PI / (float)segments);
        float x = cx + cosf(a) * radius;
        float y = cy + sinf(a) * radius;
        SDL_RenderLine(r, prevx, prevy, x, y);
        prevx = x;
        prevy = y;
    }
}

static void DrawCrosshair(SDL_Renderer *r, float x, float y, float size)
{
    SDL_RenderLine(r, x - size, y, x + size, y);
    SDL_RenderLine(r, x, y - size, x, y + size);
}

static void DrawBar(SDL_Renderer *r, int x, int y, int w, int h, float t01)
{
    if (t01 < 0.0f) t01 = 0.0f;
    if (t01 > 1.0f) t01 = 1.0f;

    SDL_FRect outline = { (float)x, (float)y, (float)w, (float)h };
    SDL_RenderRect(r, &outline);

    SDL_FRect fill = { (float)x + 2.0f, (float)y + 2.0f, (float)(w - 4) * t01, (float)h - 4.0f };
    SDL_RenderFillRect(r, &fill);
}

int main(int argc, char *argv[])
{
    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

    gWindow = SDL_CreateWindow("SDL3 Little Example", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!gWindow) {
        SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    gRenderer = SDL_CreateRenderer(gWindow, NULL);
    if (!gRenderer) {
        SDL_Log("Renderer could not be created! SDL error: %s\n", SDL_GetError());
        SDL_DestroyWindow(gWindow);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Log("Renderer utilisé : %s\n", SDL_GetRendererName(gRenderer));
    SDL_SetRenderVSync(gRenderer, 1);

    bool quit = false;
    SDL_Event e;

    bool relative = false;
    bool resize = false;
    bool border = true;

    float mousex = 0.0f, mousey = 0.0f;

    // petit objet qui rebondit
    float bx = 100.0f, by = 100.0f;
    float bvx = 220.0f, bvy = 160.0f;
    float bsz = 60.0f;

    Uint64 prev = SDL_GetTicks();

    while (!quit) {

        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_EVENT_MOUSE_MOTION:
                    mousex = e.motion.x;
                    mousey = e.motion.y;
                    break;

                case SDL_EVENT_QUIT:
                    quit = true;
                    break;

                case SDL_EVENT_KEY_DOWN:
                    if (e.key.key == SDLK_F) ToggleFullscreen(gWindow);
                    if (e.key.key == SDLK_ESCAPE) quit = true;
                    if (e.key.key == SDLK_M) SDL_MaximizeWindow(gWindow);
                    if (e.key.key == SDLK_R) SDL_RestoreWindow(gWindow);
                    if (e.key.key == SDLK_I) SDL_MinimizeWindow(gWindow);

                    if (e.key.key == SDLK_A) {
                        relative = !relative;
                        SDL_SetWindowRelativeMouseMode(gWindow, relative);
                    }

                    if (e.key.key == SDLK_Y) {
                        resize = !resize;
                        SDL_SetWindowResizable(gWindow, resize);
                    }

                    if (e.key.key == SDLK_U) {
                        border = !border;
                        SDL_SetWindowBordered(gWindow, border);
                    }
                    break;
            }
        }

        // timing (dt en secondes)
        Uint64 now = SDL_GetTicks();
        float dt = (float)(now - prev) / 1000.0f;
        if (dt > 0.05f) dt = 0.05f; // clamp (evite gros saut si pause/debug)
        prev = now;

        int ww = 0, wh = 0;
        SDL_GetWindowSize(gWindow, &ww, &wh);

        // update rebond
        bx += bvx * dt;
        by += bvy * dt;

        if (bx < 0) { bx = 0; bvx = -bvx; }
        if (by < 0) { by = 0; bvy = -bvy; }
        if (bx + bsz > ww) { bx = (float)ww - bsz; bvx = -bvx; }
        if (by + bsz > wh) { by = (float)wh - bsz; bvy = -bvy; }

        // ---- RENDER ----

        // fond bleu
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        // grille
        DrawGrid(gRenderer, ww, wh, 40);

        // cercle (au centre)
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        DrawCircle(gRenderer, (float)ww * 0.5f, (float)wh * 0.5f, 120.0f, 64);

        // carré qui rebondit
        SDL_SetRenderDrawColor(gRenderer, 255, 220, 40, 255);
        SDL_FRect rect = { bx, by, bsz, bsz };
        SDL_RenderFillRect(gRenderer, &rect);

        // contour du carré
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
        SDL_RenderRect(gRenderer, &rect);

        // crosshair souris
        SDL_SetRenderDrawColor(gRenderer, 255, 80, 80, 255);
        DrawCrosshair(gRenderer, mousex, mousey, 12.0f);

        // barres “debug” (dt et vitesse) en haut à gauche
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        DrawBar(gRenderer, 10, 10, 180, 16, dt / (1.0f / 30.0f)); // 0..~1 quand ~30fps

        float speed = sqrtf(bvx*bvx + bvy*bvy);
        DrawBar(gRenderer, 10, 30, 180, 16, speed / 400.0f);

        SDL_RenderPresent(gRenderer);
    }

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
    return EXIT_SUCCESS;
}
