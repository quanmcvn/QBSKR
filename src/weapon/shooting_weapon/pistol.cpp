#include "weapon/shooting_weapon/pistol.hpp"

#include <stdexcept>

#include "object/player.hpp"
#include "util/crappy_reader_data.hpp"
#include "weapon/hurt.hpp"

Pistol::Pistol(MovingObject* parent, const std::string& sprite_name,
	           uint32_t projectile_id, float projectile_speed, float sec_per_projectiles) :
	ShootingWeapon(parent, sprite_name),
	m_projectile_id(projectile_id),
	m_projectile_speed(projectile_speed)
{
	m_timer.start_true(sec_per_projectiles);
	recalculate_hurt_attributes();
}

Pistol::Pistol(MovingObject* parent, const SpritePtr& sprite,
	           uint32_t projectile_id, float projectile_speed, float sec_per_projectiles) :
	ShootingWeapon(parent, sprite),
	m_projectile_id(projectile_id),
	m_projectile_speed(projectile_speed)
{
	m_timer.start_true(sec_per_projectiles);
	recalculate_hurt_attributes();
}

Pistol::Pistol(MovingObject* parent, const Sprite* sprite,
	           uint32_t projectile_id, float projectile_speed, float sec_per_projectiles) :
	ShootingWeapon(parent, sprite),
	m_projectile_id(projectile_id),
	m_projectile_speed(projectile_speed)
{
	m_timer.start_true(sec_per_projectiles);
	recalculate_hurt_attributes();
}

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

	std::string sprite_filename;
	if (!crd->get("sprite-filename", sprite_filename)) {
		throw std::runtime_error("Missing sprite-filename in weapon-specific");
	}

	return std::make_unique<Pistol>(nullptr, parent_path + sprite_filename, projectile_id, projectile_speed, 1.0f / projectiles_per_sec);
}

std::string Pistol::class_name() { return "pistol"; }
std::string Pistol::get_class_name() const { return class_name(); }

bool Pistol::check_timer() { return m_timer.check(); }
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
	return std::make_unique<Pistol>(parent, m_sprite.get(), m_projectile_id, m_projectile_speed, m_timer.get_period());
}