#include "video/sdl/sdl_texture.hpp"

#include <sstream>
#include <SDL.h>

#include "video/sdl/sdl_renderer.hpp"
#include "video/video_system.hpp"

SDLTexture::SDLTexture(SDL_Texture* texture, int width, int height) :
	m_texture(texture),
	m_width(width),
	m_height(height)
{}

SDLTexture::SDLTexture(const SDL_Surface& image) :
	m_texture(),
	m_width(),
	m_height()
{
	m_texture = SDL_CreateTextureFromSurface(static_cast<SDLRenderer&>(VideoSystem::current()->get_renderer()).get_sdl_renderer(),
	                                         const_cast<SDL_Surface*>(&image));
	if (!m_texture) {
		std::ostringstream msg;
		msg << "Couldn't create texture: " << SDL_GetError();
		throw std::runtime_error(msg.str());
	}

	m_width = image.w;
	m_height = image.h;
}

SDLTexture::~SDLTexture() 
{
	SDL_DestroyTexture(m_texture);
}