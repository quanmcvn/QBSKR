#include "weapon/shooting_weapon/projectile/generic_projectile.hpp"

#include "math/util.hpp"
#include "util/crappy_reader.hpp"

GenericProjectile::GenericProjectile(const std::string& sprite_filename) :
	Projectile(sprite_filename),
	m_bounce_wall_count()
{}

GenericProjectile::GenericProjectile(const Sprite* sprite) :
	Projectile(sprite),
	m_bounce_wall_count()
{}

std::unique_ptr<GenericProjectile> GenericProjectile::from_reader(const CrappyReaderData* crd, const std::string& parent_path)
{
	float speed = 100.0f;
	crd->get("speed", speed);

	int bounce_wall_count = 0;
	crd->get("bounce-wall-count", bounce_wall_count);
	if (bounce_wall_count < 0) bounce_wall_count = 0;

	std::string sprite_filename;
	if (!crd->get("sprite-filename", sprite_filename)) {
		throw std::runtime_error("Missing sprite-filename in projectile-specific");
	}

	auto generic_projectile = std::make_unique<GenericProjectile>(parent_path + sprite_filename);
	generic_projectile->m_physic.set_velocity(Vector(speed, 0.0f));
	generic_projectile->m_bounce_wall_count = bounce_wall_count;

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

std::unique_ptr<Projectile> GenericProjectile::clone(const Vector& pos, uint32_t hurt_attributes, float angle) const
{
	auto projectile = std::make_unique<GenericProjectile>(m_sprite.get());
	projectile->set_pos(pos);
	projectile->set_hurt_attributes(hurt_attributes);
	projectile->set_angle(angle);
	projectile->m_bounce_wall_count = m_bounce_wall_count;
	Vector speed = Vector(math::length(m_physic.get_velocity()), 0.0f);
	projectile->m_physic.set_velocity(math::rotate(speed, angle));

	return projectile;
}