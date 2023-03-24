#include "weapon/shooting_weapon/generic_shooting_weapon.hpp"

#include <stdexcept>

#include "object/player.hpp"
#include "util/crappy_reader_data.hpp"
#include "weapon/hurt.hpp"
#include "weapon/shooting_weapon/projectile/projectile_set.hpp"
#include "weapon/shooting_weapon/projectile/projectile.hpp"

GenericShootingWeapon::GenericShootingWeapon(const std::string& sprite_name) :
	ShootingWeapon(sprite_name),
	m_projectile_id(0),
	m_projectile_spawn_pos(Vector(0, 0))
{}

GenericShootingWeapon::GenericShootingWeapon(const Sprite* sprite) :
	ShootingWeapon(sprite),
	m_projectile_id(0),
	m_projectile_spawn_pos(Vector(0, 0))
{}

std::unique_ptr<GenericShootingWeapon> GenericShootingWeapon::from_reader(const CrappyReaderData* crd, const std::string& parent_path)
{
	uint32_t projectile_id;
	if (!crd->get("projectile-id", projectile_id)) {
		throw std::runtime_error("Missing projectile-id in weapon-specific");
	}

	float projectile_speed = 1.0f;
	crd->get("projectile-speed", projectile_speed);

	float projectiles_per_sec = 1.0f;
	crd->get("projectiles-per-sec", projectiles_per_sec);

	Vector projectile_spawn_pos = Vector(0, 0);
	if (const CrappyReaderData* crd_projectile_spawn_pos = crd->get_child("projectile-spawn-pos")) {
		crd_projectile_spawn_pos->get("x", projectile_spawn_pos.x);
		crd_projectile_spawn_pos->get("y", projectile_spawn_pos.y);
	}

	std::string sprite_filename;
	if (!crd->get("sprite-filename", sprite_filename)) {
		throw std::runtime_error("Missing sprite-filename in weapon-specific");
	}

	auto weapon = std::make_unique<GenericShootingWeapon>(parent_path + sprite_filename);
	weapon->m_projectile_id = projectile_id;
	weapon->m_timer.start_true(1.0f / projectiles_per_sec);
	weapon->m_projectile_spawn_pos = projectile_spawn_pos;

	return weapon;
}

std::string GenericShootingWeapon::class_name() { return "generic-shooting-weapon"; }
std::string GenericShootingWeapon::get_class_name() const { return class_name(); }

bool GenericShootingWeapon::check_timer() { return m_timer.check(); }

Vector GenericShootingWeapon::get_projectile_spawn_pos() const
{
	Vector to_rotate = m_projectile_spawn_pos;
	return get_bounding_box().get_middle() - ProjectileSet::current()->get(get_projectile_id()).get_bounding_box().get_middle() + math::rotate(to_rotate, get_angle());
}

uint32_t GenericShootingWeapon::get_projectile_id() const { return m_projectile_id; }

void GenericShootingWeapon::recalculate_hurt_attributes()
{
	if (m_parent == nullptr) {
		m_hurt_attributes = 0;
	} else {
		if (dynamic_cast<const Player*>(m_parent)) {
			m_hurt_attributes = HURT_BADGUY;
		} else {
			// NYI
		}
	}
}

std::unique_ptr<Weapon> GenericShootingWeapon::clone(MovingObject* parent) const
{
	auto weapon = std::make_unique<GenericShootingWeapon>(m_sprite.get());
	weapon->change_parent(parent);
	weapon->m_projectile_id = m_projectile_id;
	weapon->m_timer.start_true(m_timer.get_period());
	weapon->m_projectile_spawn_pos = m_projectile_spawn_pos;
	weapon->recalculate_hurt_attributes();

	return weapon;
}