#ifndef HEADER_QBSKR_WEAPON_SHOOTING_WEAPON_PISTOL_HPP
#define HEADER_QBSKR_WEAPON_SHOOTING_WEAPON_PISTOL_HPP

#include "util/timer.hpp"
#include "weapon/shooting_weapon/shooting_weapon.hpp"

class CrappyReaderData;

class Pistol final : public ShootingWeapon {
private:
	Pistol(const Pistol&) = delete;
	Pistol& operator=(const Pistol&) = delete;

private:
	uint32_t m_hurt_attributes;
	uint32_t m_projectile_id;
	float m_projectile_speed;
	Timer m_timer;

public:
	Pistol(MovingObject* parent, const std::string& sprite_name,
	       uint32_t projectile_id, float projectile_speed, float projectiles_per_sec);

	Pistol(MovingObject* parent, const SpritePtr& sprite,
	       uint32_t projectile_id, float projectile_speed, float projectiles_per_sec);

	Pistol(MovingObject* parent, const Sprite* sprite,
	       uint32_t projectile_id, float projectile_speed, float projectiles_per_sec);

public:
	static std::unique_ptr<Pistol> from_reader(const CrappyReaderData* crd, const std::string& parent_path);

public:
	static std::string class_name();
	virtual std::string get_class_name() const;
	virtual bool check_timer() override;
	virtual uint32_t get_hurt_attributes() const override;
	virtual uint32_t get_projectile_id() const override;
	virtual float get_projectile_speed() const override;
	virtual void recalculate_hurt_attributes() override;
	virtual std::unique_ptr<Weapon> clone(MovingObject* parent) const override;
};

#endif