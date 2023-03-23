#include "weapon/shooting_weapon/projectile.hpp"

#include "math/util.hpp"

Projectile::Projectile() :
	MovingSprite(),
	m_hurt_attributes(),
	m_physic()
{}

Projectile::Projectile(const std::string& sprite_name, uint32_t hurt_attributes) :
	MovingSprite(Vector(0.0f, 0.0f), sprite_name, LAYER_PROJECTILES),
	m_hurt_attributes(hurt_attributes),
	m_physic()
{}

Projectile::Projectile(const Vector& pos, const std::string& sprite_name, uint32_t hurt_attributes, float speed, float angle) :
	MovingSprite(pos, sprite_name),
	m_hurt_attributes(hurt_attributes)
{
	set_angle(angle);
	m_physic.set_velocity(Vector(math::cos_degree(angle), math::sin_degree(angle)) * speed);
}

Projectile::Projectile(const Vector& pos, const Sprite* sprite, uint32_t hurt_attributes, float speed, float angle) :
	MovingSprite(pos, sprite, LAYER_PROJECTILES),
	m_hurt_attributes(hurt_attributes)
{
	set_angle(angle);
	m_physic.set_velocity(Vector(math::cos_degree(angle), math::sin_degree(angle)) * speed);
}

std::unique_ptr<Projectile> Projectile::clone(const Vector& pos, uint32_t hurt_attributes, float speed, float angle) const
{
	return std::make_unique<Projectile>(pos, m_sprite.get(), hurt_attributes, speed, angle);
}

void Projectile::update(float dt_sec) { set_movement(m_physic.get_movement(dt_sec)); }

std::string Projectile::class_name() { return "projectile"; }
std::string Projectile::get_class_name() const { return class_name(); } 

void Projectile::collision_solid(const CollisionHit& /* hit */) { remove_me(); }
HitResponse Projectile::collision(GameObject& /* other */, const CollisionHit& /* hit */) { return ABORT_MOVE; }
void Projectile::collision_tile(uint32_t /* tile_attributes */) {}