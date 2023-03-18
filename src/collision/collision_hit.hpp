#ifndef HEADER_QBSKR_COLLISION_COLLISION_HIT_HPP
#define HEADER_QBSKR_COLLISION_COLLISION_HIT_HPP

#include "math/vector.hpp"


/**
 * Used as a returned value from collision function, to decide how the
 * collision should be handled
**/
enum HitResponse {
	// call collision() but no collision handling
	ABORT_MOVE = 0,

	// move object out of collision (normal physics)
	CONTINUE,

	// treat object as having inf mass
	// push every CONTINUE objects out of the way
	FORCE_MOVE
};

/**
 * Class holds data of a collision
**/
class CollisionHit final {
public:
	bool left;
	bool right;
	bool top;
	bool bottom;

	CollisionHit() :
		left(false),
		right(false),
		top(false),
		bottom(false)
	{}
};

#endif