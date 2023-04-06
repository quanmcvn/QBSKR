#ifndef HEADER_QBSKR_OBJECT_TILE_SET_HPP
#define HEADER_QBSKR_OBJECT_TILE_SET_HPP

#include <vector>
#include <memory>
#include <string>

#include "object/tile.hpp"
#include "util/currenton.hpp"

class CrappyReader;

class TileSet final : public Currenton<TileSet> {
public:
	TileSet();
	~TileSet() override;

private:
	std::vector<std::unique_ptr<Tile>> m_tiles;

public:
	void add_tile(uint32_t id, std::unique_ptr<Tile> tile);
	const Tile& get(const uint32_t id) const;
};

#endif