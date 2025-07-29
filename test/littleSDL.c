/*

*/

#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>

unsigned long __stack = 1024 * 1024 * 2;

bool isFullscreen = false;

static void ToggleFullscreen(SDL_Window* w)
{
    isFullscreen = !isFullscreen;

    //SDL_Log("Toggle fullscreen %d \n",  isFullscreen);
    SDL_SetWindowFullscreen(w, isFullscreen ? SDL_WINDOW_FULLSCREEN : 0);

}

int main(int argc, char *argv[])
{
	
	SDL_Window* gWindow = NULL;
	SDL_Renderer* gRenderer = NULL;;

	if (SDL_Init(SDL_INIT_VIDEO) == 0)
	{
        SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

	//Create window
	
	gWindow = SDL_CreateWindow( "SDL3 Little Example", 800, 600, SDL_WINDOW_RESIZABLE);
	if (!gWindow)
	{
		SDL_Log( "Window could not be created! SDL error: %s\n", SDL_GetError() );
		SDL_Quit();
		return EXIT_FAILURE;
	}
	gRenderer = SDL_CreateRenderer(gWindow, NULL);
	if (!gRenderer) {
		SDL_Log( "Renderer could not be created! SDL error: %s\n", SDL_GetError() );
		SDL_DestroyWindow( gWindow );
		gWindow = NULL;
		SDL_Quit();
		return EXIT_SUCCESS;			
	}

	// Witch REnder ??
    SDL_Log("Renderer utilisé : %s\n", SDL_GetRendererName(gRenderer));
	SDL_SetRenderVSync(gRenderer, 1);
	//The quit flag
	bool quit = false;

	//The event data
	SDL_Event e;
	bool relative = false;
	bool resize = false;
	bool border = false;
	//The main loop
	while( quit == false )
	{
		//Get event data
		while( SDL_PollEvent( &e ) )
		{
			switch (e.type) 
			{
				case SDL_EVENT_MOUSE_MOTION:
					SDL_Log("x:%f y:%f", e.motion.x, e.motion.y);
				break;
				case SDL_EVENT_QUIT:
					//End the main loop
					quit = true;
					break;
				case SDL_EVENT_KEY_DOWN:
                {
					if(e.key.key == SDLK_F) {
                        ToggleFullscreen(gWindow);
					}
					if(e.key.key == SDLK_ESCAPE) {
						quit = true;
					}
					if(e.key.key == SDLK_M) {
						SDL_MaximizeWindow(gWindow);
					}
					if(e.key.key == SDLK_R) {
						SDL_RestoreWindow(gWindow);
					}
					if(e.key.key == SDLK_I) {
						SDL_MinimizeWindow(gWindow);
					}
					
					if(e.key.key == SDLK_A) {
						relative = !relative;
						SDL_SetWindowRelativeMouseMode(gWindow, relative);
					}
					
					if(e.key.key == SDLK_Y) {
						resize = !resize;
						SDL_SetWindowResizable(gWindow, resize);
					}
					if(e.key.key == SDLK_U) {
						border = !border;
						SDL_SetWindowBordered(gWindow, border);
					}
                    
                }
                break;

			}
		}

		//Fill the background white
		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );
		SDL_RenderClear( gRenderer );
	

		//Update screen
		SDL_RenderPresent( gRenderer );
	} 
	

	if (gRenderer) {
		SDL_DestroyRenderer( gRenderer );
		gRenderer = NULL;
	}
	
	if (gWindow) {
		SDL_DestroyWindow( gWindow );
		gWindow = NULL;
	}
	
    SDL_Quit();
    return EXIT_SUCCESS;
}
