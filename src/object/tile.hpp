#ifndef HEADER_QBSKR_OBJECT_TILE_HPP
#define HEADER_QBSKR_OBJECT_TILE_HPP

#include <vector>
#include <string>
#include <stdint.h>

#include "math/vector.hpp"
#include "video/surface_ptr.hpp"

class Canvas;

class Tile final {
public:
	// bitset for tile attribute
	enum {
		SOLID                       = 0b0000'0000'0000'0001,

		// reserve for some more attribute

		// to check in collision
		FIRST_INTERESTING_ATTRIBUTE = 0b0000'0001'0000'0000,
		GOAL                        = 0b0000'0001'0000'0000
	};

public:
	Tile();

private:
	Tile(const Tile&) = delete;
	Tile& operator=(const Tile&) = delete;

private:
	std::vector<SurfacePtr> m_images;
	uint32_t m_attributes;

	float m_fps;

	std::string m_name;

public:
	Tile(const std::vector<SurfacePtr>& images, uint32_t attributes, float fps, const std::string& name = "");

	void draw(Canvas& canvas, const Vector& pos, int layer) const;

	uint32_t get_attributes() const;

	bool is_solid() const;
};

#endif