#include "video/ttf_surface.hpp"

#include <SDL_ttf.h>

#include "util/log.hpp"
#include "video/sdl/sdl_surface_ptr.hpp"
#include "video/surface.hpp"
#include "video/ttf_font.hpp"
#include "video/video_system.hpp"

TTFSurface::TTFSurface(const SurfacePtr& surface, const Vector& offset) : 
	m_surface(surface),
	m_offset(offset)
{}

TTFSurfacePtr TTFSurface::create(const TTFFont& font, const std::string& text)
{
	SDLSurfacePtr text_surface(TTF_RenderUTF8_Blended(font.get_ttf_font(), text.c_str(), SDL_Color{0xff, 0xff, 0xff, 0xff}));

	if (!text_surface) {
		log_warning << "Couldn't render text '" << text << "' :" << SDL_GetError() << std::endl;
		return std::make_shared<TTFSurface>(SurfacePtr(), Vector(0.0f, 0.0f));
	}

	SurfacePtr result = Surface::from_texture(VideoSystem::current()->new_texture(*text_surface));
	return std::make_shared<TTFSurface>(result, Vector(0.0f, 0.0f));
}

SurfacePtr TTFSurface::get_surface() const { return m_surface; }
Vector TTFSurface::get_offset() const { return m_offset; }

int TTFSurface::get_width() const { return m_surface->get_width(); }
int TTFSurface::get_height() const { return m_surface->get_height(); }