#include "video/sdl/sdlbased_video_system.hpp"

#include <sstream>

#include "qbskr/gameconfig.hpp"
#include "qbskr/globals.hpp"
#include "util/log.hpp"

SDLBasedVideoSystem::SDLBasedVideoSystem() : 
	m_sdl_window(nullptr, &SDL_DestroyWindow)
{}

SDLBasedVideoSystem::~SDLBasedVideoSystem()
{}

void SDLBasedVideoSystem::create_sdl_window() {
	Size size = g_config->window_size;
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

void SDLBasedVideoSystem::set_icon(const SDL_Surface& icon)
{
	SDL_SetWindowIcon(m_sdl_window.get(), const_cast<SDL_Surface*>(&icon));
}