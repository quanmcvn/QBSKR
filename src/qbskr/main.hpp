#ifndef HEADER_QBSKR_QBSKR_MAIN_HPP
#define HEADER_QBSKR_QBSKR_MAIN_HPP

#include "control/input_manager.hpp"
#include "object/player.hpp"
#include "qbskr/gameconfig.hpp"
#include "video/texture_manager.hpp"
#include "video/video_system.hpp"

// //Starts up SDL and creates window
// bool init();

// //Loads media
// bool loadMedia();

// //Frees media and shuts down SDL
// void close();

// //Loads individual image as texture
// SDL_Texture* loadTexture(std::string path);

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

public:
	int run(int argc, char** argv);
};

#endif
