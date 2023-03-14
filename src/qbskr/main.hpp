#ifndef HEADER_QBSKR_QBSKR_MAIN_HPP
#define HEADER_QBSKR_QBSKR_MAIN_HPP

#include "control/input_manager.hpp"
#include "object/player.hpp"
#include "qbskr/gameconfig.hpp"
#include "sprite/sprite_manager.hpp"
#include "video/texture_manager.hpp"
#include "video/video_system.hpp"

class SDLSubSystem final {
public:
	SDLSubSystem();
	~SDLSubSystem();
};

class Main final {
public:
	Main();

private:
	Main(const Main&) = delete;
	Main& operator=(const Main&) = delete;

private:
	std::unique_ptr<SDLSubSystem> m_sdl_subsystem;
	std::unique_ptr<InputManager> m_input_manager;
	std::unique_ptr<VideoSystem> m_video_system;
	std::unique_ptr<SpriteManager> m_sprite_manager;

public:
	int run(int argc, char** argv);
};

#endif
