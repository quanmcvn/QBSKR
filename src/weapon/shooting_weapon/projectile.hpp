#ifndef HEADER_QBSKR_WEAPON_SHOOTING_WEAPON_PROJECTILE_HPP
#define HEADER_QBSKR_WEAPON_SHOOTING_WEAPON_PROJECTILE_HPP

#include "object/moving_sprite.hpp"
#include "object/physic.hpp"

class DrawingContext;

/**
 * Base class for Projectile
 * Can be inherted to expand but now I'm lazy
*/
class Projectile : public MovingSprite {
public:
	Projectile();

private:
	Projectile(const Projectile&);
	Projectile& operator=(const Projectile&) = delete;

private:
	uint32_t m_hurt_attributes;
	Physic m_physic;

public:
	Projectile(const std::string& sprite_name, uint32_t hurt_attributes);
	Projectile(const Vector& pos, const std::string& sprite_name, uint32_t hurt_attributes, float speed, float angle);
	Projectile(const Vector& pos, const SpritePtr& sprite, uint32_t hurt_attributes, float speed, float angle);
	Projectile(const Vector& pos, const Sprite* sprite, uint32_t hurt_attributes, float speed, float angle);

public:
	std::unique_ptr<Projectile> clone(const Vector& pos, uint32_t hurt_attributes, float speed, float angle) const;

	virtual void update(float dt_sec) override;
	static std::string class_name();
	virtual std::string get_class_name() const override; 
	// the default implementation just call remove_me()
	virtual void collision_solid(const CollisionHit& hit) override;
	// the default implementation just return ABORT_MOVE
	virtual HitResponse collision(GameObject& other, const CollisionHit& hit) override;
	// the default implementation doesn't do anything
	virtual void collision_tile(uint32_t tile_attributes) override;
};

#endif