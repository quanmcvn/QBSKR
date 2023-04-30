#ifndef HEADER_QBSKR_WEAPON_SHOOTING_WEAPON_GENERIC_SHOOTING_WEAPON_HPP
#define HEADER_QBSKR_WEAPON_SHOOTING_WEAPON_GENERIC_SHOOTING_WEAPON_HPP

#include "util/timer.hpp"
#include "weapon/shooting_weapon/shooting_weapon.hpp"

class CrappyReaderData;

/**
 * Class for generic shooting weapon 
 * (has no interesting mechanic, only shoot projectile)
*/
class GenericShootingWeapon final : public ShootingWeapon {
private:
	GenericShootingWeapon(const GenericShootingWeapon&) = delete;
	GenericShootingWeapon& operator=(const GenericShootingWeapon&) = delete;

private:
	uint32_t m_projectile_id;
	Timer m_timer;
	Vector m_projectile_spawn_pos;
	std::string m_sound_file;
	float m_inaccuracy;

public:
	GenericShootingWeapon(const std::string& sprite_name);

public:
	static std::unique_ptr<GenericShootingWeapon> from_reader(const CrappyReaderData* crd);

public:
	static std::string class_name();
	virtual std::string get_class_name() const;
	virtual bool check_timer() override;
	virtual Vector get_projectile_spawn_pos() const override;
	virtual uint32_t get_projectile_id() const override;
	virtual void play_shoot_sound() const override;
	virtual float get_shoot_angle() const override;
	virtual std::unique_ptr<Weapon> clone(MovingObject* parent, const Vector& pos) const override;
};

#endif