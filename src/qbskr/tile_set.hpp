#ifndef HEADER_QBSKR_QBSKR_TILE_SET_HPP
#define HEADER_QBSKR_QBSKR_TILE_SET_HPP

#include <vector>
#include <memory>
#include <string>

#include "qbskr/tile.hpp"

class CrappyReader;

class TileSet final {
public:
	static std::unique_ptr<TileSet> from_file(const std::string& filename);

public:
	TileSet();
	~TileSet();

	void add_tile(uint32_t id, std::unique_ptr<Tile> tile);
	const Tile& get(const uint32_t id) const;

private:
	std::vector<std::unique_ptr<Tile>> m_tiles;
};

#endif