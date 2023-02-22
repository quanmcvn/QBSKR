#include "qbskr/main.hpp"

#include <SDL_image.h>
#include <string>
#include <iostream>

#include "math/rect.hpp"
#include "util/log.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

Player player;

bool init() {
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}


bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load PNG texture
	player.m_texture = loadTexture( "data/images/creatures/knight/idle0.png");
	if( player.m_texture == NULL ) {
		log_warning << "Failed to load texture image!\n";
		success = false;
	}

	return success;
}

void close() {
	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(std::string path) {
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else {
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL) {
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

int Main::run(int argc, char** argv) {
	if (!init()) {
		std::cout << "couldn't init\n";
		return 1;
	}
	if (!loadMedia()) {
		std::cout << "Failed to load media!\n";
	}

	bool quit = false;

	//Event handler
	SDL_Event e;

	//While application is running
	while (!quit) {
		//Handle events on queue
		// I REALLY hate this current event processing though
		// will probably touch it next
		int dir[2] = {0, 0};
		while (SDL_PollEvent(&e) != 0) {
			//User requests quit
			if (e.type == SDL_QUIT) {
				quit = true;
				break;
			}
			if (e.type == SDL_KEYDOWN) {
				dir[0] = dir[1] = 0;
				const Uint8* keystates = SDL_GetKeyboardState(nullptr);
				
				if(keystates[SDL_SCANCODE_A]) {
					dir[0] -= 1;
				} 
				if(keystates[SDL_SCANCODE_D]) {
					dir[0] += 1;
				}

				if(keystates[SDL_SCANCODE_W]) {
					dir[1] -= 1;
				} 
				if(keystates[SDL_SCANCODE_S]) {
					dir[1] += 1;
				}
			}
			if (e.type == SDL_KEYUP) {
				const Uint8* keystates = SDL_GetKeyboardState(nullptr);
				
				if(keystates[SDL_SCANCODE_A]) {
					dir[0] -= 1;
				} 
				if(keystates[SDL_SCANCODE_D]) {
					dir[0] += 1;
				}

				if(keystates[SDL_SCANCODE_W]) {
					dir[1] -= 1;
				} 
				if(keystates[SDL_SCANCODE_S]) {
					dir[1] += 1;
				}
			}
		}

		player.set_movement(Vector(dir[0] * 8, dir[1] * 8));
		player.update();

		//Clear screen
		SDL_RenderClear(gRenderer);

		//Render texture to screen
		SDL_Rect* temp = new SDL_Rect((Rect(Rectf(player.m_pos, Sizef(player.m_texture_size)))).to_sdl());
		SDL_RenderCopy(gRenderer, player.m_texture, nullptr, temp);

		//Update screen
		SDL_RenderPresent(gRenderer);
	}

	close();
	return 0;
}