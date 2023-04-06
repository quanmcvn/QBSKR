#include "object/tile_set.hpp"

#include "object/tile_set_parser.hpp"
#include "util/log.hpp"

TileSet::TileSet() :
	m_tiles(1)
{
	TileSetParser parser(*this, "images/tiles/tiles-tileset.txt");
	parser.parse();
}

TileSet::~TileSet()
{
	m_tiles.clear();
}

void TileSet::add_tile(uint32_t id, std::unique_ptr<Tile> tile)
{
	if (id >= static_cast<uint32_t>(m_tiles.size())) {
		m_tiles.resize(id + 1);
	}

	if (m_tiles[id]) {
		log_warning << "Tile id " << id << " redefined" << std::endl;
	} else {
		m_tiles[id] = std::move(tile);
	}
}

const Tile& TileSet::get(const uint32_t id) const
{
	if (id >= static_cast<uint32_t>(m_tiles.size())) {
		log_warning << "Invalid tile id: " << id << std::endl;
		return *m_tiles[0];
	} else {
		Tile* tile = m_tiles[id].get();
		if (tile) {
			return *tile;
		} else {
			log_warning << "Invalid tile id: " << id << std::endl;
			return *m_tiles[0];
		}
	}
}
