#include "qbskr/main.hpp"

#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include <sstream>
#include <chrono>

#include "math/random.hpp"
#include "object/camera.hpp"
#include "object/tile.hpp"
#include "qbskr/constants.hpp"
#include "qbskr/gameconfig.hpp"
#include "qbskr/globals.hpp"
#include "qbskr/game_session.hpp"
#include "qbskr/screen_manager.hpp"
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
	Uint32 flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
	if (SDL_Init(flags) < 0) {
		std::ostringstream msg;
		msg << "Couldn't init SDL: " << SDL_GetError();
		throw std::runtime_error(msg.str());
	}

	int img_flags = IMG_INIT_PNG;
	if (!(IMG_Init(img_flags) & img_flags)) {
		std::ostringstream msg;
		msg << "Couldn't init SDL_Image: " << SDL_GetError();
		throw std::runtime_error(msg.str());
	}

	if (TTF_Init() < 0) {
		std::ostringstream msg;
		msg << "Couldn't initialize SDL TTF: " << SDL_GetError();
		throw std::runtime_error(msg.str());
	}

	// apparently I can do this, extra safe
	atexit(TTF_Quit);
	atexit(Mix_Quit);
	atexit(IMG_Quit);
	atexit(SDL_Quit);
}

SDLSubSystem::~SDLSubSystem()
{
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

Main::Main() :
	m_sdl_subsystem(),
	m_input_manager(),
	m_video_system(),
	m_sprite_manager(),
	m_ttf_surface_manager(),
	m_sound_manager(),
	m_weapon_set(),
	m_projectile_set(),
	m_badguy_set(),
	m_tile_set(),
	m_level_data_set(),
	m_resources(),
	m_screen_manager()
{}

Main::~Main()
{
	m_screen_manager.reset();
	m_resources.reset();
	m_level_data_set.reset();
	m_tile_set.reset();
	m_badguy_set.reset();
	m_projectile_set.reset();
	m_weapon_set.reset();
	m_sound_manager.reset();
	m_ttf_surface_manager.reset();
	m_sprite_manager.reset();
	m_video_system.reset();
	m_input_manager.reset();
	m_sdl_subsystem.reset();
}

int Main::run(int /* argc */, char** /* argv */)
{
	int result = 0;
	try {
		m_sdl_subsystem = std::make_unique<SDLSubSystem>();
		m_input_manager = std::make_unique<InputManager>(g_config->keyboard_config, g_config->mouse_button_config);
		m_video_system = VideoSystem::create(VideoSystem::VIDEO_SDL);
		m_sprite_manager = std::make_unique<SpriteManager>();
		m_ttf_surface_manager = std::make_unique<TTFSurfaceManager>();
		m_sound_manager = std::make_unique<SoundManager>();
		m_weapon_set = std::make_unique<WeaponSet>();
		m_projectile_set = std::make_unique<ProjectileSet>();
		m_badguy_set = std::make_unique<BadGuySet>();
		m_tile_set = std::make_unique<TileSet>();
		m_level_data_set = std::make_unique<LevelDataSet>();
		m_resources = std::make_unique<Resources>();
		m_screen_manager = std::make_unique<ScreenManager>(*m_video_system, *m_input_manager);

		g_game_random.seed(std::chrono::system_clock::now().time_since_epoch().count());

		ScreenManager::current()->push_screen(std::make_unique<GameSession>("levels/level-0/level-0-level.txt"));
		ScreenManager::current()->run();
	} catch (std::exception& e) {
		log_fatal << "Unexpected exception: " << e.what() << std::endl;
		result = 1;
	}
	return result;
}