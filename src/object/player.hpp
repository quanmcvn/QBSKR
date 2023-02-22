#ifndef HEADER_QBSKR_OBJECT_PLAYER_HPP
#define HEADER_QBSKR_OBJECT_PLAYER_HPP

#include "math/rectf.hpp"
#include "math/size.hpp"
#include <SDL.h>

class Player {
public:
	Player();
	~Player();

public:
	/** Position of player */
	Vector m_pos;
	/** Texture of player */
	SDL_Texture* m_texture;
	/** Size of texture of player (temporary, will gets its own class soonTM) */
	Size m_texture_size;
	void set_movement(Vector movement);

	void update();

private:
	/** The movement that will happen till next frame */
	Vector m_movement;
	/** Bounding box of player (used for collision detection) */
	Rectf m_bbox;
};

#endif