#ifndef HEADER_QBSKR_COLLISION_COLLISION_OBJECT_HPP
#define HEADER_QBSKR_COLLISION_COLLISION_OBJECT_HPP

#include "collision/collision_group.hpp"
#include "collision/collision_hit.hpp"
#include "math/rectf.hpp"

class CollisionListener;
class GameObject;

class CollisionObject final {
	friend class CollisionSystem;

private:
	CollisionObject(const CollisionObject&) = delete;
	CollisionObject& operator=(const CollisionObject&) = delete;

private:
	CollisionListener& m_listener;

public:
	// the bounding box of the object used for collision detection
	Rectf m_bounding_box;

	// the collision group
	CollisionGroup m_group;

private:
	// the movement that will happen next frame
	Vector m_movement;

	// anticipated destination of the object during collision detection 
	Rectf m_dest;

public:
	CollisionObject(CollisionGroup group, CollisionListener& parent);

	// called when collided with something solid
	void collision_solid(const CollisionHit& hit);

	// called when collided with any other object
	HitResponse collision(CollisionObject& other, const CollisionHit& hit);

	// called when collided with tiles with special attributes
	void collision_tile(uint32_t tile_attributes);

	Rectf get_bounding_box() const;

	void set_movement(const Vector& movement);
	Vector get_movement() const;
	
	Vector get_pos() const;
	void set_pos(const Vector& pos);

	void set_size(float width, float height);

	CollisionGroup get_group() const;

	CollisionListener* get_listener();

	bool is_valid() const;
};

#endif