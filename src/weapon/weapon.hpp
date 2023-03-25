#ifndef HEADER_QBSKR_WEAPON_WEAPON_HPP
#define HEADER_QBSKR_WEAPON_WEAPON_HPP

#include "object/moving_sprite.hpp"

/**
 * Class to represent weapon
 * Holds a pointer to parent (can't use ref since need init but I need to clone)
*/
class Weapon : public MovingSprite {
private:
	Weapon(const Weapon&) = delete;
	Weapon& operator=(const Weapon&) = delete;

protected:
	// have to be MovingObject* instead of MovingObject&
	MovingObject* m_parent;
	uint32_t m_hurt_attributes;

public:
	Weapon(const std::string& sprite_name);
	Weapon(const Sprite* sprite);

public:
	virtual void update(float dt_sec) override;
	virtual void draw(DrawingContext& drawing_context) override;
	virtual void attack() = 0;
	virtual void recalculate_hurt_attributes() = 0;
	virtual std::unique_ptr<Weapon> clone(MovingObject* parent) const = 0;

public:
	void change_parent(MovingObject* parent);
	uint32_t get_hurt_attributes() const;
	void set_hurt_attributes(uint32_t hurt_attributes);
};

#endif