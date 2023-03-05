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

Player player;

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

	player.m_texture = TextureManager::current()->get("../data/images/creatures/knight/idle0.png");

	bool quit = false;

	while (!quit) {
		{
			SDL_Event event;
			while (SDL_PollEvent(&event) != 0) {
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

		auto request = new TextureRequest();

		request->type = TEXTURE;
		request->texture = player.m_texture.get();

		request->srcrects.emplace_back(Rectf(0, 0, 16, 28));
		request->dstrects.emplace_back(Rectf(player.m_pos.x, player.m_pos.y, player.m_pos.x + 24, player.m_pos.y + 42));

		renderer.start_draw();

		painter.draw_texture(static_cast<const TextureRequest&>(*request));

		renderer.end_draw();

		VideoSystem::current()->present();

		SDL_Delay(20);
	}

	return 0;
}