#include "video/surface.hpp"

#include "video/texture_manager.hpp"

Surface::~Surface()
{}

SurfacePtr Surface::from_texture(const TexturePtr& texture)
{
	return std::make_shared<Surface>(texture);
}

SurfacePtr Surface::from_file(const std::string& filename, const std::optional<Rect>& rect) 
{
	if (rect) {
		TexturePtr texture = TextureManager::current()->get(filename, *rect);
		return std::make_shared<Surface>(texture, filename);
	} else {
		TexturePtr texture = TextureManager::current()->get(filename);
		return std::make_shared<Surface>(texture, filename);
	}
}

Surface::Surface(const TexturePtr& texture, const std::string filename) :
	m_texture(texture),
	m_region(0, 0, texture->get_image_width(), texture->get_image_height()),
	m_filename(filename)
{}

Surface::Surface(const TexturePtr& texture, const Rect& region, const std::string filename) :
	m_texture(texture),
	m_region(region),
	m_filename(filename)
{}

TexturePtr Surface::get_texture() const { return m_texture; }
Rect Surface::get_region() const {return m_region; }
int Surface::get_width() const { return m_region.get_width(); }
int Surface::get_height() const { return m_region.get_height(); }