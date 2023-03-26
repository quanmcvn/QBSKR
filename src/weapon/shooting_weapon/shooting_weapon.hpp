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
	ShootingWeapon(const std::string& sprite_name);
	ShootingWeapon(const Sprite* sprite);

public:
	virtual void attack(int times = 1) override;
	virtual void shoot_projectile(float angle) const;
	virtual bool check_timer() = 0;
	// since get_pos() returns the top left of collision bounding box
	// spawn the projectile there is weird
	virtual Vector get_projectile_spawn_pos() const = 0;
	virtual uint32_t get_projectile_id() const = 0;
};

#endif