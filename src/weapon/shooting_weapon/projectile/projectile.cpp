#include "weapon/shooting_weapon/projectile/projectile.hpp"

Projectile::Projectile(const std::string& sprite_name) :
	MovingSprite(Vector(0.0f, 0.0f), sprite_name, LAYER_PROJECTILES)
{}

Projectile::Projectile(const Sprite* sprite) :
	MovingSprite(Vector(0.0f, 0.0f), sprite, LAYER_PROJECTILES)
{}

void Projectile::update(float dt_sec) { set_movement(m_physic.get_movement(dt_sec)); }

std::string Projectile::class_name() { return "projectile"; }
std::string Projectile::get_class_name() const { return class_name(); } 

void Projectile::collision_solid(const CollisionHit& /* hit */) { remove_me(); }
HitResponse Projectile::collision(GameObject& /* other */, const CollisionHit& /* hit */) { return ABORT_MOVE; }
void Projectile::collision_tile(uint32_t /* tile_attributes */) {}

uint32_t Projectile::get_hurt_attributes() const { return m_hurt_attributes; }
void Projectile::set_hurt_attributes(uint32_t hurt_attributes) { m_hurt_attributes = hurt_attributes; }

int Projectile::get_damage() const { return m_damage; }