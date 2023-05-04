#ifndef HEADER_QBSKR_WEAPON_SHOOTING_WEAPON_SHOOTING_WEAPON_HPP
#define HEADER_QBSKR_WEAPON_SHOOTING_WEAPON_SHOOTING_WEAPON_HPP

#include "util/timer.hpp"
#include "weapon/weapon.hpp"

/**
 * Abstract base class for weapon that shoot projectile
*/
class ShootingWeapon : public Weapon {
private:
	ShootingWeapon(const ShootingWeapon&) = delete;
	ShootingWeapon& operator=(const ShootingWeapon&) = delete;

protected:
	std::string m_sound_file;
	Timer m_timer;
	float m_inaccuracy;
	int m_projectiles_per_attack;

public:
	ShootingWeapon(const std::string& sprite_name);

public:
	virtual void attack() override;
	virtual void shoot_projectile(float angle) const;
	// since get_pos() returns the top left of collision bounding box
	// spawn the projectile there is weird
	virtual Vector get_projectile_spawn_pos() const = 0;
	virtual uint32_t get_projectile_id() const = 0;
	virtual float get_shoot_angle() const;
};

#endif