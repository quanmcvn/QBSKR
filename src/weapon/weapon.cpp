#include "weapon/weapon.hpp"

#include "badguy/badguy.hpp"
#include "object/player.hpp"
#include "weapon/hurt.hpp"

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
		set_pos(m_parent->get_pos() + m_pos_offset);
	}
	MovingSprite::draw(drawing_context);
}

void Weapon::recalculate_hurt_attributes()
{
	if (m_parent == nullptr) {
		m_hurt_attributes = 0;
	} else {
		if (dynamic_cast<Player*>(m_parent)) {
			m_hurt_attributes = HURT_BADGUY;
		} else if (dynamic_cast<BadGuy*>(m_parent)) {
			m_hurt_attributes = HURT_PLAYER;
		}
	}
}

void Weapon::change_parent(MovingObject* parent)
{
	m_parent = parent;
	recalculate_hurt_attributes();
}

uint32_t Weapon::get_hurt_attributes() const { return m_hurt_attributes; }
void Weapon::set_hurt_attributes(uint32_t hurt_attributes) { m_hurt_attributes = hurt_attributes; }

Vector Weapon::get_pos_offset() const { return m_pos_offset; }
void Weapon::set_pos_offset(const Vector& pos_offset) { m_pos_offset = pos_offset; }