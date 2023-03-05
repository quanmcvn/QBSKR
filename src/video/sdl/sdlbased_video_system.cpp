#include "video/sdl/sdlbased_video_system.hpp"

#include <sstream>

#include "util/log.hpp"

SDLBasedVideoSystem::SDLBasedVideoSystem() : 
	m_sdl_window(nullptr, &SDL_DestroyWindow)
{}

SDLBasedVideoSystem::~SDLBasedVideoSystem()
{}

void SDLBasedVideoSystem::create_sdl_window() {
	Size size = Size(640, 480);
	m_sdl_window.reset(SDL_CreateWindow("QBSKR",
	                                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	                                    size.width, size.height,
	                                    0));
	if (!m_sdl_window) {
		std::ostringstream msg;
		msg << "Couldn't set video " << size.width << "x" << size.height << ": " << SDL_GetError();
		throw std::runtime_error(msg.str());
	}
}