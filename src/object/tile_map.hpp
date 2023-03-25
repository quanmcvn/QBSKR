#ifndef HEADER_QBSKR_OBJECT_TILE_MAP_HPP
#define HEADER_QBSKR_OBJECT_TILE_MAP_HPP

#include "math/rect.hpp"
#include "math/vector.hpp"
#include "object/game_object.hpp"

class Tile;
class TileSet;
class CrappyReaderData;

/**
 * This class draws the level tiles
 * The reason TileMap inherit GameObject is that it can be added in GameObjectManager
*/

class TileMap final : public GameObject {
public:
	TileMap();
	~TileMap() override;

private:
	TileMap(const TileMap&) = delete;
	TileMap& operator=(const TileMap&) = delete;

private:
	const TileSet* m_tileset;
	using Tiles = std::vector<uint32_t>;
	Tiles m_tiles;

	// is any of tile in this tilemap is solid
	bool m_is_solid;

	int m_width;
	int m_height;
	int m_layer;
	
	Vector m_offset;

public:
	TileMap(TileSet* tileset, CrappyReaderData* crd);

	virtual void update(float dt_sec) override;
	virtual void draw(DrawingContext& context) override;

	bool is_solid() const;

	Vector get_offset() const;
	void set_offset(const Vector& new_offset);

	// NOT respects m_offset
	uint32_t get_tile_id(int x, int y) const;
	// respects m_offset
	uint32_t get_tile_id_at(const Vector& pos) const;
	// NOT respects m_offset
	const Tile& get_tile(int x, int y) const;
	// respects m_offset
	const Tile& get_tile_at(const Vector& pos) const;

	// NOT respects m_offset
	void change(int x, int y, uint32_t new_tile);
	// respects m_offset
	void change_at(const Vector& pos, uint32_t new_tile);

	// respects m_offset
	bool is_outside_bounds(const Vector& pos) const;
	
	// get a rect of indices that overlap the given rectf
	// respects m_offset
	Rect get_tiles_overlap(const Rectf& rectf) const;

	// respects m_offset
	Vector get_tile_posisition(int x, int y) const;

	// respects m_offset
	Rectf get_tile_bounding_box(int x, int y) const;

	// respects m_offset
	Rectf get_bounding_box() const;

private:
	void parse_tiles(CrappyReaderData* crd);
};

#endif