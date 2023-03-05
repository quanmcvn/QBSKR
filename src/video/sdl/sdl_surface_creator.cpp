#include "video/sdl/sdl_surface_creator.hpp"

#include <sstream>
#include <stdexcept>
#include <SDL_image.h>

#include "util/log.hpp"

SDLSurfacePtr SDLSurfaceCreator::from_file(const std::string& filename)
{
	log_debug << "loading image: " << filename << std::endl;
	SDLSurfacePtr surface(IMG_Load(filename.c_str()));
	if (!surface) {
		std::ostringstream msg;
		msg << "Couldn't load image: '" << filename << "' :" << SDL_GetError();
		throw std::runtime_error(msg.str());
	} else {
		return surface;
	}
}