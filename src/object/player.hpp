#ifndef HEADER_QBSKR_OBJECT_PLAYER_HPP
#define HEADER_QBSKR_OBJECT_PLAYER_HPP

#include "math/vector.hpp"
#include "sprite/sprite_ptr.hpp"

class Player {
public:
	Player();
	~Player();

public:
	// Position of player
	Vector m_pos;

public:
	SpritePtr m_sprite;
	// The movement that will happen till next frame
	Vector m_movement;
	// // Bounding box of player (used for collision detection)
	// Rectf m_bbox;
public:
	void set_movement(Vector movement);

	void update();

};

#endif