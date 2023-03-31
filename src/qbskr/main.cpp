#include "qbskr/main.hpp"

#include <SDL_image.h>
#include <string>
#include <iostream>
#include <sstream>

#include "object/camera.hpp"
#include "object/tile.hpp"
#include "qbskr/constants.hpp"
#include "qbskr/globals.hpp"
#include "qbskr/level_data.hpp"
#include "qbskr/level.hpp"
#include "qbskr/room_data_set.hpp"
#include "qbskr/room.hpp"
#include "util/log.hpp"
#include "video/canvas.hpp"
#include "video/compositor.hpp"
#include "video/drawing_context.hpp"
#include "video/sdl/sdl_video_system.hpp"

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
	m_video_system(),
	m_sprite_manager(),
	m_weapon_set(),
	m_projectile_set(),
	m_badguy_set(),
	m_tile_set()
{}

Main::~Main()
{
	m_tile_set.release();
	m_badguy_set.release();
	m_projectile_set.release();
	m_weapon_set.release();
	m_sprite_manager.release();
	m_video_system.release();
	m_input_manager.release();
	m_sdl_subsystem.release();
}

int Main::run(int /* argc */, char** /* argv */)
{
	m_sdl_subsystem = std::make_unique<SDLSubSystem>();
	m_input_manager = std::make_unique<InputManager>(g_config->keyboard_config, g_config->mouse_button_config);
	m_video_system = VideoSystem::create(VideoSystem::VIDEO_SDL);
	m_sprite_manager = std::make_unique<SpriteManager>();
	m_weapon_set = std::make_unique<WeaponSet>();
	m_projectile_set = std::make_unique<ProjectileSet>();
	m_badguy_set = std::make_unique<BadGuySet>();
	m_tile_set = TileSet::from_file("images/tiles/tiles-tileset.txt");

	auto level_data = std::make_unique<LevelData>("levels/level-0/level-0-level.txt");
	auto level = level_data->make_level();
	level->activate();

	Room::get().add<Player>(0, 1);
	Room::get().add<Camera>();

	const Uint32 ms_per_step = static_cast<Uint32>(1000.0f / LOGICAL_FPS);
	const float seconds_per_step = static_cast<float>(ms_per_step) / 1000.0f;

	// SDL_GetTicks64() is recommended
	Uint64 last_ticks = SDL_GetTicks64();
	// elapsed_ticks for sure can't get to ~49 days
	// or can it? (ref?)
	Uint32 elapsed_ticks = 0;

	bool quit = false;

	while (!quit) {
		Uint64 ticks = SDL_GetTicks64();
		elapsed_ticks += static_cast<Uint32>(ticks - last_ticks);
		last_ticks = ticks;

		if (elapsed_ticks < ms_per_step) {
			SDL_Delay(ms_per_step - elapsed_ticks);
		}

		int steps = elapsed_ticks / ms_per_step;

		// cap max step per frame
		// 4 step per frame = 16 fps
		steps = std::min(steps, 4);

		for (int i = 0; i < steps; ++i) {
			float d_time = seconds_per_step;
			g_game_time += d_time;
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
			level->update(d_time);
			elapsed_ticks -= ms_per_step;
		}

		if (steps > 0) {
			Compositor compositor(*VideoSystem::current());
			auto& drawing_context = compositor.make_context();
			Camera& camera = Room::get().get_camera();
			
			drawing_context.push_transform();
			
			drawing_context.set_translation(camera.get_translation());
			level->draw(drawing_context);
			
			drawing_context.pop_transform();
			compositor.render();
		}
	}

	return 0;
}