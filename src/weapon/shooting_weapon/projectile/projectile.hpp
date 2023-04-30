#ifndef HEADER_QBSKR_WEAPON_SHOOTING_WEAPON_PROJECTILE_PROJECTILE_HPP
#define HEADER_QBSKR_WEAPON_SHOOTING_WEAPON_PROJECTILE_PROJECTILE_HPP

#include "object/moving_sprite.hpp"
#include "object/physic.hpp"

class DrawingContext;

/**
 * Base class for Projectile
*/
class Projectile : public MovingSprite {
private:
	Projectile(const Projectile&) = delete;
	Projectile& operator=(const Projectile&) = delete;

protected:
	uint32_t m_hurt_attributes;
	Physic m_physic;
	int m_damage;
	float m_crit_chance;

public:
	Projectile(const std::string& sprite_name);

public:
	virtual void update(float dt_sec) override;
	static std::string class_name();
	virtual std::string get_class_name() const override; 
	// the default implementation just call remove_me()
	virtual void collision_solid(const CollisionHit& hit) override;
	// the default implementation just return ABORT_MOVE
	virtual HitResponse collision(GameObject& other, const CollisionHit& hit) override;
	// the default implementation doesn't do anything
	virtual void collision_tile(uint32_t tile_attributes) override;

	virtual std::unique_ptr<Projectile> clone(const Vector& pos, uint32_t hurt_attributes, float angle) const = 0;

public:
	uint32_t get_hurt_attributes() const;
	void set_hurt_attributes(uint32_t hurt_attributes);
	int get_damage() const;
	float get_crit_chance() const;
};

#endif