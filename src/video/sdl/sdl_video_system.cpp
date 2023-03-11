#include "video/sdl/sdl_video_system.hpp"

#include <sstream>

#include "qbskr/gameconfig.hpp"
#include "qbskr/globals.hpp"
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

	m_renderer = std::make_unique<SDLRenderer>(*this, m_sdl_renderer.get());
	m_texture_manager = std::make_unique<TextureManager>();

	apply_config();
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

const Viewport& SDLVideoSystem::get_viewport() const 
{
	return m_viewport; 
}

void SDLVideoSystem::apply_config()
{
	Rect viewport = Rect(0, 0, g_config->window_size);
	Vector scale = Vector(g_config->magnification, g_config->magnification);
	m_viewport = Viewport(viewport, scale);
}

void SDLVideoSystem::present() {
	m_renderer->present();
}