#ifndef HEADER_QBSKR_COLLISION_COLLISION_SYSTEM_HPP
#define HEADER_QBSKR_COLLISION_COLLISION_SYSTEM_HPP

#include <vector>
#include <memory>
#include <stdint.h>

#include "collision/collision.hpp"
#include "object/tile.hpp"

class CollisionObject;
class DrawingContext;
class Room;

class CollisionSystem final {
private:
	CollisionSystem(const CollisionSystem&) = delete;
	CollisionSystem& operator=(const CollisionSystem&) = delete;

private:
	Room& m_room;
	std::vector<CollisionObject*>  m_objects;

public:
	CollisionSystem(Room& room);

	void add(CollisionObject* object);
	void remove(CollisionObject* object);

	// checks for all possible collisions and calls the collision_handlers
	void update();
	void draw_debug(DrawingContext& drawing_context);

private:
	void collision_static_constraints(collision::Constraints* constraints, const Vector& movement, const Rectf& dest, CollisionObject& object) const;
	void collision_tilemap_constraints(collision::Constraints* constraints, const Vector& movement, const Rectf& dest, CollisionObject& object) const;
	// does collision detection against all other static and tile map
	// also respond to it, but only for first hit (other hit get ignored)
	// call this repeatedly to resolve those
	void collision_static_tilemap_constraints(collision::Constraints* constraints, const Vector& movement, const Rectf& dest, CollisionObject& object) const;
	void collision_static_tilemap_object(CollisionObject& object) const;

	void collision_object(CollisionObject& object1, CollisionObject& object2) const;

};

#endif