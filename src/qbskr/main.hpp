#ifndef HEADER_QBSKR_QBSKR_MAIN_HPP
#define HEADER_QBSKR_QBSKR_MAIN_HPP

#include "audio/sound_manager.hpp"
#include "badguy/badguy_set.hpp"
#include "control/input_manager.hpp"
#include "object/player.hpp"
#include "object/tile_set.hpp"
#include "qbskr/level_data_set.hpp"
#include "qbskr/resources.hpp"
#include "qbskr/screen_manager.hpp"
#include "sprite/sprite_manager.hpp"
#include "video/texture_manager.hpp"
#include "video/ttf_surface_manager.hpp"
#include "video/video_system.hpp"
#include "weapon/weapon_set.hpp"
#include "weapon/shooting_weapon/projectile/projectile_set.hpp"

class SDLSubSystem final {
public:
	SDLSubSystem();
	~SDLSubSystem();
};

class Main final {
public:
	Main();
	~Main();

private:
	Main(const Main&) = delete;
	Main& operator=(const Main&) = delete;

private:
	std::unique_ptr<SDLSubSystem> m_sdl_subsystem;
	std::unique_ptr<InputManager> m_input_manager;
	std::unique_ptr<VideoSystem> m_video_system;
	std::unique_ptr<SpriteManager> m_sprite_manager;
	std::unique_ptr<TTFSurfaceManager> m_ttf_surface_manager;
	std::unique_ptr<SoundManager> m_sound_manager;
	std::unique_ptr<WeaponSet> m_weapon_set;
	std::unique_ptr<ProjectileSet> m_projectile_set;
	std::unique_ptr<BadGuySet> m_badguy_set;
	std::unique_ptr<TileSet> m_tile_set;
	std::unique_ptr<LevelDataSet> m_level_data_set;
	std::unique_ptr<Resources> m_resources;
	std::unique_ptr<ScreenManager> m_screen_manager;

public:
	int run(int argc, char** argv);
};

#endif
