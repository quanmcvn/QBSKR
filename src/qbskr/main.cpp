#include "qbskr/main.hpp"

#include <SDL_image.h>
#include <string>
#include <iostream>
#include <sstream>

#include "control/input_manager.hpp"
#include "control/keyboard_config.hpp"
#include "control/mouse_button_config.hpp"
#include "math/rect.hpp"
#include "qbskr/constants.hpp"
#include "qbskr/globals.hpp"
#include "qbskr/tile_set_parser.hpp"
#include "qbskr/tile_set.hpp"
#include "qbskr/tile.hpp"
#include "util/log.hpp"
#include "sprite/sprite_data.hpp"
#include "sprite/sprite_ptr.hpp"
#include "sprite/sprite.hpp"
#include "video/canvas.hpp"
#include "video/compositor.hpp"
#include "video/drawing_context.hpp"
#include "video/surface.hpp"
#include "video/sdl/sdl_video_system.hpp"
#include "video/sdl/sdl_renderer.hpp"

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
	m_sprite_manager = std::make_unique<SpriteManager>();

	Player player;

	auto tileset = TileSet::from_file("images/tiles/tiles-tileset.txt");

	const Uint32 ms_per_step = static_cast<Uint32>(1000.0f / LOGICAL_FPS);
	const float seconds_per_step = static_cast<float>(ms_per_step) / 1000.0f;

	// SDL_GetTicks64() is recommended
	Uint64 last_ticks = SDL_GetTicks64();
	// elapsed_ticks for sure can't get to ~49 days
	// or can it? (ref?)
	Uint32 elapsed_ticks = 0;

	bool quit = false;

	bool is_player_walk_left = false;
	bool is_player_walk = false;

	while (!quit) {
		Uint64 ticks = SDL_GetTicks64();
		elapsed_ticks += static_cast<Uint32>(ticks - last_ticks);
		last_ticks = ticks;

		if (elapsed_ticks < ms_per_step) {
			SDL_Delay(ms_per_step - elapsed_ticks);
		}

		int steps = elapsed_ticks / ms_per_step;

		for (int i = 0; i < steps; ++i) {
			g_game_time += seconds_per_step;
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
				is_player_walk_left = false;
			} else if (player_control.hold(Control::LEFT) && !player_control.hold(Control::RIGHT)) {
				is_player_walk_left = true;
			}

			if (dir[0] != 0 || dir[1] != 0) {
				is_player_walk = true;
			} else {
				is_player_walk = false;
			}

			player.set_movement(Vector(dir[0] * 2, dir[1] * 2));

			player.update();

			Compositor compositor(*VideoSystem::current());

			auto& drawing_context = compositor.make_context();
			auto& canvas = drawing_context.get_canvas();

			if (is_player_walk) {
				if (is_player_walk_left) {
					player.m_sprite->set_action("walk-left");
				} else {
					player.m_sprite->set_action("walk-right");
				}
			} else {
				if (is_player_walk_left) {
					player.m_sprite->set_action("idle-left");
				} else {
					player.m_sprite->set_action("idle-right");
				}
			}

			player.m_sprite->draw(canvas, player.m_pos, 100, NO_FLIP);

			for (int i = -5; i <= 5; i += 1) {
				for (int j = -5; j <= 5; j += 1) {
					Vector aug = Vector(i, j) * 16;
					if (abs(i) == 5 || abs(j) == 5) {
						tileset->get(1).draw(canvas, Vector(120, 120) + aug, 99);
					} else {
						tileset->get(2).draw(canvas, Vector(120, 120) + aug, 99);
					}
				}
			}

			compositor.render();
			elapsed_ticks -= ms_per_step;
		}
	}

	return 0;
}