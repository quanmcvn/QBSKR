#include "weapon/weapon.hpp"

Weapon::Weapon(MovingObject* parent, const std::string& sprite_name) :
	MovingSprite(parent ? parent->get_pos() : Vector(0.0f, 0.0f), sprite_name, LAYER_WEAPONS, COLLISION_GROUP_DISABLED),
	m_parent(parent)
{}

Weapon::Weapon(MovingObject* parent, const SpritePtr& sprite) :
	MovingSprite(parent ? parent->get_pos() : Vector(0.0f, 0.0f), sprite, LAYER_WEAPONS, COLLISION_GROUP_DISABLED),
	m_parent(parent)
{}

Weapon::Weapon(MovingObject* parent, const Sprite* sprite) :
	MovingSprite(parent ? parent->get_pos() : Vector(0.0f, 0.0f), sprite, LAYER_WEAPONS, COLLISION_GROUP_DISABLED),
	m_parent(parent)
{}

void Weapon::update(float /* dt_sec */)
{}

void Weapon::change_parent(MovingObject* parent)
{
	m_parent = parent;
	recalculate_hurt_attributes();
}