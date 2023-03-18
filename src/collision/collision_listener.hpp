#ifndef HEADER_QBSKR_COLLISION_COLLISION_LISTENER_HPP
#define HEADER_QBSKR_COLLISION_COLLISION_LISTENER_HPP

#include <stdint.h>

#include "collision/collision_hit.hpp"

class GameObject;

/**
 * This class is an interface
*/
class CollisionListener {
public:
	virtual ~CollisionListener() {}

	// called when collided with something solid
	virtual void collision_solid(const CollisionHit& hit) = 0;

	// called when collided with any other object
	virtual HitResponse collision(GameObject& other, const CollisionHit& hit) = 0;

	// called when collided with tiles with special attributes
	virtual void collision_tile(uint32_t tile_attributes) = 0;

	virtual bool listener_is_valid() const = 0;
};

#endif