#ifndef HEADER_QBSKR_WEAPON_SHOOTING_WEAPON_SHOOTING_WEAPON_HPP
#define HEADER_QBSKR_WEAPON_SHOOTING_WEAPON_SHOOTING_WEAPON_HPP

#include "weapon/weapon.hpp"

/**
 * Abastract base class for weapon that shoot projectile
*/
class ShootingWeapon : public Weapon {
private:
	ShootingWeapon(const ShootingWeapon&) = delete;
	ShootingWeapon& operator=(const ShootingWeapon&) = delete;

public:
	ShootingWeapon(MovingObject* parent, const std::string& sprite_name);
	ShootingWeapon(MovingObject* parent, const Sprite* sprite);

public:
	virtual void attack() override;
	virtual void shoot_projectile(float speed, float angle) const;
	virtual bool check_timer() = 0;
	// since get_pos() returns the top left of collision bounding box
	// spawn the projectile there is weird
	virtual Vector get_projectile_spawn_pos() const = 0;
	virtual uint32_t get_hurt_attributes() const override = 0;
	virtual uint32_t get_projectile_id() const = 0;
	virtual float get_projectile_speed() const = 0;
	virtual void recalculate_hurt_attributes() override = 0;
	virtual std::unique_ptr<Weapon> clone(MovingObject* parent) const override = 0;
};

#endif