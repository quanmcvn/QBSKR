#include "weapon/shooting_weapon/generic_shooting_weapon.hpp"

#include <stdexcept>

#include "audio/sound_manager.hpp"
#include "util/crappy_reader_data.hpp"
#include "weapon/shooting_weapon/projectile/projectile_set.hpp"
#include "weapon/shooting_weapon/projectile/projectile.hpp"

GenericShootingWeapon::GenericShootingWeapon(const std::string& sprite_name) :
	ShootingWeapon(sprite_name),
	m_projectile_id(0),
	m_timer(),
	m_projectile_spawn_pos(Vector(0, 0)),
	m_sound_file()
{}

std::unique_ptr<GenericShootingWeapon> GenericShootingWeapon::from_reader(const CrappyReaderData* crd)
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

	std::string sound_file;
	crd->get("shoot-sound-file", sound_file);

	auto weapon = std::make_unique<GenericShootingWeapon>(crd->m_parent_path + sprite_filename);
	weapon->m_projectile_id = projectile_id;
	weapon->m_timer.start_true(1.0f / projectiles_per_sec, true);
	weapon->m_projectile_spawn_pos = projectile_spawn_pos;
	weapon->m_sound_file = sound_file;

	return weapon;
}

std::string GenericShootingWeapon::class_name() { return "generic-shooting-weapon"; }
std::string GenericShootingWeapon::get_class_name() const { return class_name(); }

bool GenericShootingWeapon::check_timer() { return m_timer.check(); }

Vector GenericShootingWeapon::get_projectile_spawn_pos() const
{
	Vector to_rotate = m_projectile_spawn_pos;
	return get_bounding_box().get_middle() - ProjectileSet::current()->get_projectile(get_projectile_id()).get_bounding_box().get_middle() + math::rotate(to_rotate, get_angle());
}

uint32_t GenericShootingWeapon::get_projectile_id() const { return m_projectile_id; }

void GenericShootingWeapon::play_shoot_sound() const { if (!m_sound_file.empty()) SoundManager::current()->play_sound(m_sound_file); }

std::unique_ptr<Weapon> GenericShootingWeapon::clone(MovingObject* parent, const Vector& pos) const
{
	auto weapon = std::make_unique<GenericShootingWeapon>(m_sprite_name);
	weapon->change_parent(parent);
	weapon->set_pos(pos);
	weapon->m_pos_offset = m_pos_offset;
	weapon->m_projectile_id = m_projectile_id;
	weapon->m_timer.start_true(m_timer.get_period(), true);
	weapon->m_projectile_spawn_pos = m_projectile_spawn_pos;
	weapon->m_sound_file = m_sound_file;

	return weapon;
}