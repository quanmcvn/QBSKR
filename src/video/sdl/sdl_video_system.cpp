#include "video/sdl/sdl_video_system.hpp"

#include <sstream>

#include "util/log.hpp"
#include "video/renderer.hpp"
#include "video/sdl/sdl_renderer.hpp"
#include "video/sdl/sdl_texture.hpp"
#include "video/texture_manager.hpp"

SDLVideoSystem::SDLVideoSystem() : 
	m_sdl_renderer(nullptr, &SDL_DestroyRenderer),
	m_renderer(),
	m_texture_manager()
{
	create_window();

	m_renderer = std::make_unique<SDLRenderer>(m_sdl_renderer.get());
	m_texture_manager = std::make_unique<TextureManager>();
}

SDLVideoSystem::~SDLVideoSystem() 
{}

void SDLVideoSystem::create_window() 
{
	log_info << "Create SDLVideoSystem" << std::endl;

	create_sdl_window();

	m_sdl_renderer.reset(SDL_CreateRenderer(m_sdl_window.get(), -1, 0));
	if (!m_sdl_renderer) {
		std::stringstream msg;
		msg << "Couldn' t create SDL_Renderer: " << SDL_GetError();
		throw std::runtime_error(msg.str());
	}
}

Renderer& SDLVideoSystem::get_renderer() const 
{
	return *m_renderer;
}

TexturePtr SDLVideoSystem::new_texture(const SDL_Surface& image)
{
	return TexturePtr(new SDLTexture(image));
}

void SDLVideoSystem::present() {
	m_renderer->present();
}