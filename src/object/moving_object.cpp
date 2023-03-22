#include "object/moving_object.hpp"

MovingObject::MovingObject() :
	m_collision_object(COLLISION_GROUP_MOVING, *this)
{}

MovingObject::~MovingObject()
{}

void MovingObject::collision_solid(const CollisionHit& /* hit */) {}
HitResponse MovingObject::collision(GameObject& /* other */, const CollisionHit& /* hit */) { return ABORT_MOVE; }
void MovingObject::collision_tile(uint32_t /* tile_attributes */) {}
bool MovingObject::listener_is_valid() const { return is_valid(); }

Rectf MovingObject::get_bounding_box() const { return m_collision_object.get_bounding_box(); }

void MovingObject::set_movement(const Vector& movement) { m_collision_object.set_movement(movement); }
Vector MovingObject::get_movement() const { return m_collision_object.get_movement(); }

Vector MovingObject::get_pos() const { return m_collision_object.get_pos(); }
void MovingObject::set_pos(const Vector& pos) { m_collision_object.set_pos(pos); }

CollisionGroup MovingObject::get_group() const { return m_collision_object.get_group(); }

CollisionObject* MovingObject::get_collision_object() { return &m_collision_object; }
const CollisionObject* MovingObject::get_collision_object() const { return &m_collision_object; }

std::string MovingObject::class_name() { return "moving-object"; }
std::string MovingObject::get_class_name() const { return class_name(); }

void MovingObject::set_group(CollisionGroup collision_group) { m_collision_object.m_group = collision_group; }