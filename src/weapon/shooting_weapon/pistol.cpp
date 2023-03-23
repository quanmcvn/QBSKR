#include "weapon/shooting_weapon/pistol.hpp"

#include <stdexcept>

#include "math/util.hpp"
#include "object/player.hpp"
#include "util/crappy_reader_data.hpp"
#include "weapon/hurt.hpp"
#include "weapon/shooting_weapon/projectile_set.hpp"
#include "weapon/shooting_weapon/projectile.hpp"

Pistol::Pistol(MovingObject* parent, const std::string& sprite_name) :
	ShootingWeapon(parent, sprite_name),
	m_projectile_id(0),
	m_projectile_speed(0),
	m_projectile_spawn_pos(Vector(0, 0))
{}

Pistol::Pistol(MovingObject* parent, const Sprite* sprite) :
	ShootingWeapon(parent, sprite),
	m_projectile_id(0),
	m_projectile_speed(0),
	m_projectile_spawn_pos(Vector(0, 0))
{}

std::unique_ptr<Pistol> Pistol::from_reader(const CrappyReaderData* crd, const std::string& parent_path)
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

	auto pistol = std::make_unique<Pistol>(nullptr, parent_path + sprite_filename);
	pistol->m_projectile_id = projectile_id;
	pistol->m_projectile_speed = projectile_speed;
	pistol->m_timer.start_true(1.0f / projectiles_per_sec);
	pistol->m_projectile_spawn_pos = projectile_spawn_pos;

	return pistol;
}

std::string Pistol::class_name() { return "pistol"; }
std::string Pistol::get_class_name() const { return class_name(); }

bool Pistol::check_timer() { return m_timer.check(); }

Vector Pistol::get_projectile_spawn_pos() const
{
	Vector to_rotate = m_projectile_spawn_pos;
	return get_bounding_box().get_middle() - ProjectileSet::current()->get(get_projectile_id()).get_bounding_box().get_middle() + math::rotate(to_rotate, get_angle());
}

uint32_t Pistol::get_hurt_attributes() const { return m_hurt_attributes; }

uint32_t Pistol::get_projectile_id() const { return m_projectile_id; }
float Pistol::get_projectile_speed() const { return m_projectile_speed; }

void Pistol::recalculate_hurt_attributes()
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

std::unique_ptr<Weapon> Pistol::clone(MovingObject* parent) const
{
	auto pistol = std::make_unique<Pistol>(parent, m_sprite.get());
	pistol->m_projectile_id = m_projectile_id;
	pistol->m_projectile_speed = m_projectile_speed;
	pistol->m_timer.start_true(m_timer.get_period());
	pistol->m_projectile_spawn_pos = m_projectile_spawn_pos;
	pistol->recalculate_hurt_attributes();

	return pistol;
}