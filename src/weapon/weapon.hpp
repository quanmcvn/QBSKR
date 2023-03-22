#ifndef HEADER_QBSKR_WEAPON_WEAPON_HPP
#define HEADER_QBSKR_WEAPON_WEAPON_HPP

#include "object/moving_sprite.hpp"

/**
 * Class to represent weapon
 * Only holds a pointer to parent (can't use ref since need init but I need to clone)
*/
class Weapon : public MovingSprite {
private:
	Weapon(const Weapon&) = delete;
	Weapon& operator=(const Weapon&) = delete;

protected:
	// have to be MovingObject* instead of MovingObject&
	MovingObject* m_parent;

public:
	Weapon(MovingObject* parent, const std::string& sprite_name);
	Weapon(MovingObject* parent, const SpritePtr& sprite);
	Weapon(MovingObject* parent, const Sprite* sprite);

public:
	virtual void update(float dt_sec) override;
	virtual void attack(float angle) = 0;
	virtual uint32_t get_hurt_attributes() const = 0;
	virtual void recalculate_hurt_attributes() = 0;
	virtual std::unique_ptr<Weapon> clone(MovingObject* parent) const = 0;

public:
	void change_parent(MovingObject* parent);
};

#endif