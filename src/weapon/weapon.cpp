#include "weapon/weapon.hpp"

Weapon::Weapon(const std::string& sprite_name) :
	MovingSprite(Vector(0.0f, 0.0f), sprite_name, LAYER_WEAPONS, COLLISION_GROUP_DISABLED),
	m_parent(),
	m_hurt_attributes()
{}

Weapon::Weapon(const Sprite* sprite) :
	MovingSprite(Vector(0.0f, 0.0f), sprite, LAYER_WEAPONS, COLLISION_GROUP_DISABLED),
	m_parent(),
	m_hurt_attributes()
{}

void Weapon::update(float /* dt_sec */)
{}

void Weapon::draw(DrawingContext& drawing_context)
{
	// trick to have weapon's pos = parent's pos
	// since update is done
	if (m_parent) {
		set_pos(m_parent->get_pos());
	}
	MovingSprite::draw(drawing_context);
}

void Weapon::change_parent(MovingObject* parent)
{
	m_parent = parent;
	recalculate_hurt_attributes();
}

uint32_t Weapon::get_hurt_attributes() const { return m_hurt_attributes; }
void Weapon::set_hurt_attributes(uint32_t hurt_attributes) { m_hurt_attributes = hurt_attributes; }