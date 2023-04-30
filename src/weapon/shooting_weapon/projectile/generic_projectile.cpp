#include "weapon/shooting_weapon/projectile/generic_projectile.hpp"

#include "badguy/badguy.hpp"
#include "math/util.hpp"
#include "object/player.hpp"
#include "util/crappy_reader.hpp"
#include "weapon/hurt.hpp"

GenericProjectile::GenericProjectile(const std::string& sprite_filename) :
	Projectile(sprite_filename),
	m_bounce_wall_count()
{}

std::unique_ptr<GenericProjectile> GenericProjectile::from_reader(const CrappyReaderData* crd, const std::string& parent_path)
{
	float speed = 100.0f;
	crd->get("speed", speed);
	// cap minimum speed
	speed = std::max(speed, 80.0f);

	int bounce_wall_count = 0;
	crd->get("bounce-wall-count", bounce_wall_count);
	bounce_wall_count = std::max(bounce_wall_count, 0);

	int damage = 1;
	crd->get("damage", damage);
	damage = std::max(damage, 1);

	float crit_chance = 0.0f;
	crd->get("crit-chance", crit_chance);

	std::string sprite_filename;
	if (!crd->get("sprite-filename", sprite_filename)) {
		throw std::runtime_error("Missing sprite-filename in projectile-specific");
	}

	auto generic_projectile = std::make_unique<GenericProjectile>(parent_path + sprite_filename);
	generic_projectile->m_physic.set_velocity(Vector(speed, 0.0f));
	generic_projectile->m_bounce_wall_count = bounce_wall_count;
	generic_projectile->m_damage = damage;
	generic_projectile->m_crit_chance = crit_chance;

	return generic_projectile;
}

std::string GenericProjectile::class_name() { return "generic-projectile"; }
std::string GenericProjectile::get_class_name() const { return class_name(); }

void GenericProjectile::collision_solid(const CollisionHit& hit)
{
	if (m_bounce_wall_count > 0) {
		-- m_bounce_wall_count;

		if (hit.left || hit.right) {
			m_physic.inverse_velocity_x();
		}

		if (hit.top || hit.bottom) {
			m_physic.inverse_velocity_y();
		}
	} else {
		remove_me();
	}
}

HitResponse GenericProjectile::collision(GameObject& other, const CollisionHit& /* hit */)
{
	if (dynamic_cast<Player*>(&other)) {
		if (!(m_hurt_attributes & HURT_PLAYER)) {
			return ABORT_MOVE;
		}
		// projectile will hurt player, but that will be resolved by player's collision() call
		remove_me();
		return FORCE_MOVE;
	}

	if (dynamic_cast<BadGuy*>(&other)) {
		if (!(m_hurt_attributes & HURT_BADGUY)) {
			return ABORT_MOVE;
		}
		// projectile will hurt badguy, but that will be resolved by badguy's collision() call
		remove_me();
		return FORCE_MOVE;
	}

	if (dynamic_cast<GenericProjectile*>(&other)) {
		// generic projectile when collide with each other doesn't do anything
		return ABORT_MOVE;
	}

	return CONTINUE;
}

std::unique_ptr<Projectile> GenericProjectile::clone(const Vector& pos, uint32_t hurt_attributes, float angle) const
{
	auto projectile = std::make_unique<GenericProjectile>(m_sprite_name);
	projectile->set_pos(pos);
	projectile->set_hurt_attributes(hurt_attributes);
	projectile->set_angle(angle);
	projectile->m_bounce_wall_count = m_bounce_wall_count;
	// get velocity oriented Ox
	Vector velocity = Vector(math::length(m_physic.get_velocity()), 0.0f);
	projectile->m_physic.set_velocity(math::rotate(velocity, angle));
	projectile->m_damage = m_damage;
	projectile->m_crit_chance = m_crit_chance;

	return projectile;
}