#ifndef HEADER_QBSKR_VIDEO_SDL_SDLBASED_VIDEO_SYSTEM_HPP
#define HEADER_QBSKR_VIDEO_SDL_SDLBASED_VIDEO_SYSTEM_HPP

#include <SDL.h>

#include "video/video_system.hpp"

/**
 * To separate the SDL_Window and SDL_Renderer
*/
class SDLBasedVideoSystem : public VideoSystem {
public:
	SDLBasedVideoSystem();
	~SDLBasedVideoSystem() override;

private:
	SDLBasedVideoSystem(const SDLBasedVideoSystem&) = delete;
	SDLBasedVideoSystem& operator=(const SDLBasedVideoSystem&) = delete;

protected:
	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_sdl_window;

protected:
	// currently hardcoded flag = 0 for SDL_Window
	// also hardcoded size of window
	void create_sdl_window();
};

#endif