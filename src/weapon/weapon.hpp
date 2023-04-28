#ifndef HEADER_QBSKR_WEAPON_WEAPON_HPP
#define HEADER_QBSKR_WEAPON_WEAPON_HPP

#include "object/moving_sprite.hpp"

/**
 * Class to represent weapon
 * Holds a pointer to parent (can't use ref since ref need init but I need to clone)
*/
class Weapon : public MovingSprite {
private:
	Weapon(const Weapon&) = delete;
	Weapon& operator=(const Weapon&) = delete;

protected:
	// have to be MovingObject* instead of MovingObject&
	MovingObject* m_parent;
	// see src/weapon/hurt.hpp
	// every weapon has a bitmask 'hurt_attributes' to determine what it will hurt
	uint32_t m_hurt_attributes;
	// position offset with respect to m_parent
	// this->pos = m_parent->get_pos() + m_pos_offset
	// will be set by parent
	Vector m_pos_offset;

public:
	Weapon(const std::string& sprite_name);

public:
	virtual void update(float dt_sec) override;
	virtual void draw(DrawingContext& drawing_context) override;
	virtual void attack(int times = 1) = 0;
	// the reason for virtual is design choice
	// maybe a joke weapon that it doesn't hurt anything (water gun?)
	// or a weapon that hurt both player and badguy
	virtual void recalculate_hurt_attributes();
	virtual std::unique_ptr<Weapon> clone(MovingObject* parent, const Vector& pos = Vector(0.0f, 0.0f)) const = 0;

public:
	void change_parent(MovingObject* parent);

	uint32_t get_hurt_attributes() const;
	void set_hurt_attributes(uint32_t hurt_attributes);

	Vector get_pos_offset() const;
	void set_pos_offset(const Vector& pos_offset);
};

#endif