#ifndef HEADER_QBSKR_OBJECT_PLAYER_HPP
#define HEADER_QBSKR_OBJECT_PLAYER_HPP

#include <SDL.h>

#include "math/rectf.hpp"
#include "math/size.hpp"
#include "video/surface.hpp"
#include "video/surface_ptr.hpp"

class Player {
public:
	Player();
	~Player();

public:
	// Position of player
	Vector m_pos;

public:
	// temporary, will be changed soonTM
	SurfacePtr m_surface;
	// The movement that will happen till next frame
	Vector m_movement;
	// // Bounding box of player (used for collision detection)
	// Rectf m_bbox;
public:
	void set_movement(Vector movement);

	void update();

	Texture get_texture();
};

#endif