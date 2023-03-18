#include "collision/collision_object.hpp"

#include "collision/collision_listener.hpp"
#include "object/game_object.hpp"

CollisionObject::CollisionObject(CollisionGroup group, CollisionListener& parent) :
	m_listener(parent),
	m_bounding_box(),
	m_group(group),
	m_movement(),
	m_dest()
{}

void CollisionObject::collision_solid(const CollisionHit& hit) { m_listener.collision_solid(hit); }

HitResponse CollisionObject::collision(CollisionObject& other, const CollisionHit& hit)  { return m_listener.collision(dynamic_cast<GameObject&>(other), hit); }

void CollisionObject::collision_tile(uint32_t tile_attributes) { m_listener.collision_tile(tile_attributes); }

Rectf CollisionObject::get_bounding_box() const { return m_bounding_box; }

void CollisionObject::set_movement(const Vector& movement) { m_movement = movement; }
Vector CollisionObject::get_movement() const { return m_movement; }

Vector CollisionObject::get_pos() const { return m_bounding_box.p1(); }
void CollisionObject::set_pos(const Vector& pos) { m_bounding_box.set_pos(pos); }

void CollisionObject::set_size(float width, float height)
{
	m_bounding_box.set_size(width, height);
	m_dest.set_size(width, height);
}

CollisionGroup CollisionObject::get_group() const { return m_group; }

bool CollisionObject::is_valid() const { return m_listener.listener_is_valid(); }