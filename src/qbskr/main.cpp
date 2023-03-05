#include "qbskr/main.hpp"

#include <SDL_image.h>
#include <string>
#include <iostream>
#include <sstream>

#include "control/input_manager.hpp"
#include "control/keyboard_config.hpp"
#include "control/mouse_button_config.hpp"
#include "math/rect.hpp"
#include "qbskr/globals.hpp"
#include "util/log.hpp"
#include "video/drawing_request.hpp"
#include "video/sdl/sdl_video_system.hpp"
#include "video/sdl/sdl_renderer.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

Player player;

// bool init()
// {
// 	//Initialization flag
// 	bool success = true;

// 	//Initialize SDL
// 	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
// 		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
// 		success = false;
// 	} else {
// 		//Set texture filtering to linear
// 		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
// 			printf("Warning: Linear texture filtering not enabled!");
// 		}

// 		//Create window
// 		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
// 		if (gWindow == NULL) {
// 			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
// 			success = false;
// 		} else {
// 			//Create renderer for window
// 			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
// 			if (gRenderer == NULL) {
// 				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
// 				success = false;
// 			} else {
// 				//Initialize renderer color
// 				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

// 				//Initialize PNG loading
// 				int imgFlags = IMG_INIT_PNG;
// 				if (!(IMG_Init(imgFlags) & imgFlags)) {
// 					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
// 					success = false;
// 				}
// 			}
// 		}
// 	}

// 	return success;
// }

// bool loadMedia()
// {
// 	//Loading success flag
// 	bool success = true;

// 	//Load PNG texture
// 	// This is really bad but 
// 	// 1) Currently I don't have a way to fix it and 
// 	// 2) Not on my priorities now 
// 	player.m_texture = loadTexture("../data/images/creatures/knight/idle0.png");
// 	if (player.m_texture == NULL) {
// 		log_warning << "Failed to load texture image!\n";
// 		success = false;
// 	}

// 	return success;
// }

// void close()
// {
// 	//Destroy window	
// 	SDL_DestroyRenderer(gRenderer);
// 	SDL_DestroyWindow(gWindow);
// 	gWindow = NULL;
// 	gRenderer = NULL;

// 	//Quit SDL subsystems
// 	IMG_Quit();
// 	SDL_Quit();
// }

// SDL_Texture* loadTexture(std::string path)
// {
// 	//The final texture
// 	SDL_Texture* newTexture = NULL;

// 	//Load image at specified path
// 	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
// 	if (loadedSurface == NULL) {
// 		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
// 	} else {
// 		//Create texture from surface pixels
// 		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
// 		if (newTexture == NULL) {
// 			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
// 		}

// 		//Get rid of old loaded surface
// 		SDL_FreeSurface(loadedSurface);
// 	}

// 	return newTexture;
// }

SDLSubSystem::SDLSubSystem() 
{
	Uint32 flags = SDL_INIT_VIDEO;

	if (SDL_Init(flags) < 0) {
		std::ostringstream msg;
		msg << "Couldn't init SDL: " << SDL_GetError();
		throw std::runtime_error(msg.str());
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		std::ostringstream msg;
		msg << "Couldn't init SDL_Image: " << SDL_GetError();
		throw std::runtime_error(msg.str());
	}

	// apparently I can do this, extra safe
	atexit(IMG_Quit);
	atexit(SDL_Quit);
}

SDLSubSystem::~SDLSubSystem()
{
	IMG_Quit();
	SDL_Quit();
}

Main::Main() :
	m_sdl_subsystem(),
	m_input_manager(),
	m_video_system()
{}

int Main::run([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	m_sdl_subsystem = std::make_unique<SDLSubSystem>();
	m_input_manager = std::make_unique<InputManager>(g_config->keyboard_config, g_config->mouse_button_config);
	m_video_system = VideoSystem::create(VideoSystem::VIDEO_SDL);
	// if (!init()) {
	// 	std::cout << "couldn't init\n";
	// 	return 1;
	// }
	player.m_texture = TextureManager::current()->get("../data/images/creatures/knight/idle0.png");
	// if (!loadMedia()) {
	// 	std::cout << "Failed to load media!\n";
	// }

	bool quit = false;

	//While application is running
	while (!quit) {
		//Handle events on queue
		{
			SDL_Event event;
			while (SDL_PollEvent(&event) != 0) {
				//User requests quit
				if (event.type == SDL_QUIT) {
					quit = true;
					break;
				}
				InputManager::current()->process_event(event);
			}
		}

		int dir[2] = { 0, 0 };

		dir[0] += static_cast<int>(InputManager::current()->get_controller(0).hold(Control::RIGHT));
		dir[0] -= static_cast<int>(InputManager::current()->get_controller(0).hold(Control::LEFT));

		dir[1] += static_cast<int>(InputManager::current()->get_controller(0).hold(Control::DOWN));
		dir[1] -= static_cast<int>(InputManager::current()->get_controller(0).hold(Control::UP));

		player.set_movement(Vector(dir[0] * 4, dir[1] * 4));

		player.update();

		auto& renderer = VideoSystem::current()->get_renderer();
		auto& painter = renderer.get_painter();

		//Render texture to screen
		//temporary

		auto request = new TextureRequest();

		request->type = TEXTURE;
		request->texture = player.m_texture.get();

		request->srcrects.emplace_back(Rectf(0, 0, 16, 28));
		request->dstrects.emplace_back(Rectf(player.m_pos.x, player.m_pos.y, player.m_pos.x + 24, player.m_pos.y + 42));

		renderer.start_draw();

		painter.draw_texture(static_cast<const TextureRequest&>(*request));

		renderer.end_draw();

		VideoSystem::current()->present();

		// SDL_Rect* temp = new SDL_Rect((Rect(Rectf(player.m_pos, Sizef(player.m_texture_size)))).to_sdl());
		
		// SDL_RenderCopy(gRenderer, player.m_texture, nullptr, temp);

		//Update screen
		// SDL_RenderPresent(gRenderer);

		SDL_Delay(20);
	}

	return 0;
}