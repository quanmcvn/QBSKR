#include "object/tile.hpp"

#include "qbskr/globals.hpp"
#include "video/canvas.hpp"

Tile::Tile() :
	m_images(),
	m_attributes(),
	m_fps(0),
	m_name()
{}

Tile::Tile(const std::vector<SurfacePtr>& images, uint32_t attributes, float fps, const std::string& name) :
	m_images(images),
	m_attributes(attributes),
	m_fps(fps),
	m_name(name)
{}

void Tile::draw(Canvas& canvas, const Vector& pos, int layer) const
{
	if (m_images.size() > 1) {
		size_t frame_index = static_cast<size_t>(g_game_time * m_fps) % m_images.size();
		canvas.draw_surface(m_images[frame_index], pos, layer);
	} else if (m_images.size() == 1) {
		canvas.draw_surface(m_images[0], pos, layer);
	}
}

uint32_t Tile::get_attributes() const { return m_attributes; }

bool Tile::is_solid() const { return (m_attributes & SOLID); }