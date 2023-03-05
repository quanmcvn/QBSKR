#ifndef HEADER_QBSKR_VIDEO_SDL_SDL_SURFACE_CREATOR_HPP
#define HEADER_QBSKR_VIDEO_SDL_SDL_SURFACE_CREATOR_HPP

#include <string>
#include <SDL.h>

#include "video/sdl/sdl_surface_ptr.hpp"

/**
 * Class to create SDL_Surface* in the form of SDLSurfacePtr
 * It is not Manager because it isn's responsible for managing SDL_Surface* created
*/
class SDLSurfaceCreator final {
public:
	static SDLSurfacePtr from_file(const std::string& filename);
};

#endif