#include "object/tile_map.hpp"

#include <stdexcept>
#include <math.h>

#include "math/rectf.hpp"
#include "qbskr/constants.hpp"
#include "object/tile.hpp"
#include "object/tile_set.hpp"
#include "video/drawing_context.hpp"
#include "util/crappy_reader_data.hpp"

TileMap::TileMap() :
	m_tileset(),
	m_tiles(),
	m_is_solid(),
	m_width(),
	m_height(),
	m_layer(),
	m_offset()
{}

TileMap::~TileMap()
{}

TileMap::TileMap(TileSet* tileset, CrappyReaderData* crd) :
	m_tileset(tileset),
	m_tiles(),
	m_is_solid(),
	m_width(),
	m_height(),
	m_layer(),
	m_offset()
{
	crd->get("solid", m_is_solid);

	crd->get("width", m_width);
	crd->get("height", m_height);

	crd->get("layer", m_layer);
	
	if (m_width <= 0 || m_height <= 0) {
		throw std::runtime_error("Invalid width/height");
	}
	if (CrappyReaderData::Child crd_tiles = crd->get_child("tiles")) {
		parse_tiles(crd_tiles);
	} else {
		throw std::runtime_error("No tiles in tilemap");
	}
}

void TileMap::update(float)
{}

void TileMap::draw(DrawingContext& drawing_context)
{
	drawing_context.push_transform();

	Canvas& canvas = drawing_context.get_canvas();

	Rect t_draw_rect = get_tiles_overlap(drawing_context.get_cliprect());
	Vector start = get_tile_posisition(t_draw_rect.left, t_draw_rect.top);

	Vector pos(0.0f, 0.0f);
	int tx = 0, ty = 0;

	for (pos.x = start.x, tx = t_draw_rect.left; tx < t_draw_rect.right; pos.x += BLOCK_SIZE, ++tx) {
		for (pos.y = start.y, ty = t_draw_rect.top; ty < t_draw_rect.bottom; pos.y += BLOCK_SIZE, ++ty) {
			int index = ty * m_width + tx;
			assert(index >= 0);
			assert(index < m_width * m_height);

			if (m_tiles[index] == 0) continue;
			const Tile& tile = m_tileset->get(m_tiles[index]);

			tile.draw(canvas, pos, m_layer);
		}
	}

	drawing_context.pop_transform();
}

bool TileMap::is_solid() const { return m_is_solid; }

Vector TileMap::get_offset() const { return m_offset; }
void TileMap::set_offset(const Vector& new_offset) { m_offset = new_offset; }

uint32_t TileMap::get_tile_id(int x, int y) const
{
	if (x < 0) x = 0;
	if (x >= m_width) x = m_width - 1;
	if (y < 0) y = 0;
	if (y >= m_height) y = m_height - 1;

	return m_tiles[y * m_width + x];
}

uint32_t TileMap::get_tile_id_at(const Vector& pos) const
{
	Vector xy = (pos - m_offset) / BLOCK_SIZE;
	return get_tile_id(static_cast<int>(xy.x), static_cast<int>(xy.y));
}

const Tile& TileMap::get_tile(int x, int y) const
{
	uint32_t id = get_tile_id(x, y);
	return m_tileset->get(id);
}

const Tile& TileMap::get_tile_at(const Vector& pos) const
{
	uint32_t id = get_tile_id_at(pos);
	return m_tileset->get(id);
}

void TileMap::change(int x, int y, uint32_t new_tile)
{
	assert(0 <= x && x < m_width && 0 <= y && y < m_height);
	m_tiles[y * m_width + x] = new_tile;
}

void TileMap::change_at(const Vector& pos, uint32_t new_tile)
{
	Vector xy = (pos - m_offset) / BLOCK_SIZE;
	change(static_cast<int>(xy.x), static_cast<int>(xy.y), new_tile);
}

bool TileMap::is_outside_bounds(const Vector& pos) const
{
  Vector pos_ = (pos - m_offset) / BLOCK_SIZE;
  float width = static_cast<float>(m_width);
  float height = static_cast<float>(m_height);
  return pos_.x < 0 || pos_.x >= width || pos_.y < 0 || pos_.y >= height;
}

Rect TileMap::get_tiles_overlap(const Rectf& rectf) const
{
	Rectf rectf2 = rectf.moved(-m_offset);

	int t_left    = std::max(0, static_cast<int>(floorf(rectf2.get_left() / BLOCK_SIZE)));
	int t_top     = std::max(0, static_cast<int>(floorf(rectf2.get_top() / BLOCK_SIZE)));
	int t_right   = std::min(m_width , static_cast<int>(ceilf(rectf2.get_right() / BLOCK_SIZE)));
	int t_bottom  = std::min(m_height, static_cast<int>(ceilf(rectf2.get_bottom() / BLOCK_SIZE)));
	return Rect(t_left, t_top, t_right, t_bottom);
}

Vector TileMap::get_tile_posisition(int x, int y) const
{ 
	return m_offset + Vector(static_cast<float>(x), static_cast<float>(y)) * BLOCK_SIZE;
}

Rectf TileMap::get_tile_bounding_box(int x, int y) const
{
	return Rectf(get_tile_posisition(x, y), get_tile_posisition(x + 1, y + 1));
}

Rectf TileMap::get_bounding_box() const
{
	return Rectf(get_tile_posisition(0, 0), get_tile_posisition(m_width, m_height));
}

void TileMap::parse_tiles(CrappyReaderData* crd)
{
	m_tiles.clear();
	m_tiles.resize(m_width * m_height);

	const std::vector<CrappyReaderData::Child> crd_rows = crd->get_child_all("row");
	if (static_cast<int>(crd_rows.size()) != m_height) {
		throw std::runtime_error("Not have the same number of rows as height defined");
	}
	
	for (const auto& crd_row : crd_rows) {
		if (static_cast<int>(crd_row->m_childs.size()) != m_width) {
			throw std::runtime_error("Row not have the same entries as width defined");
		}
	}

	for (size_t i = 0; i < crd_rows.size(); ++ i) {
		std::vector<uint32_t> row_data;
		crd_rows[i]->get_child_values(row_data);
		std::move(row_data.begin(), row_data.end(), m_tiles.begin() + i * m_width);
	}
}