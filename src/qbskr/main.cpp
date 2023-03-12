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
#include "video/canvas.hpp"
#include "video/compositor.hpp"
#include "video/drawing_context.hpp"
#include "video/sdl/sdl_video_system.hpp"
#include "video/sdl/sdl_renderer.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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

	Player player;
	player.m_surface = Surface::from_file("images/creatures/knight/idle0.png");
	player.m_surface_flip_horizontal = player.m_surface->clone_flip(HORIZONTAL_FLIP);

	bool quit = false;

	bool is_player_flip = false;

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

		auto& player_control = InputManager::current()->get_controller(0);

		dir[0] += static_cast<int>(player_control.hold(Control::RIGHT));
		dir[0] -= static_cast<int>(player_control.hold(Control::LEFT));

		dir[1] += static_cast<int>(player_control.hold(Control::DOWN));
		dir[1] -= static_cast<int>(player_control.hold(Control::UP));

		if (!player_control.hold(Control::LEFT) && player_control.hold(Control::RIGHT)) {
			is_player_flip = false;
		}

		if (player_control.hold(Control::LEFT) && !player_control.hold(Control::RIGHT)) {
			is_player_flip = true;
		}

		player.set_movement(Vector(dir[0] * 3, dir[1] * 3));

		player.update();

		Compositor compositor(*VideoSystem::current());

		auto& drawing_context = compositor.make_context();
		auto& canvas = drawing_context.get_canvas();

		if (is_player_flip) {
			canvas.draw_surface(player.m_surface_flip_horizontal, player.m_pos, 100);
		} else {
			canvas.draw_surface(player.m_surface, player.m_pos, 100);
		}

		compositor.render();

		SDL_Delay(20);
	}

	return 0;
}