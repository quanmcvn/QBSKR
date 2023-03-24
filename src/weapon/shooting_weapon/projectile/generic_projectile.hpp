#ifndef HEADER_QBSKR_WEAPON_SHOOTING_WEAPON_PROJECTILE_GENERIC_PROJECTILE_HPP
#define HEADER_QBSKR_WEAPON_SHOOTING_WEAPON_PROJECTILE_GENERIC_PROJECTILE_HPP

#include "object/physic.hpp"
#include "weapon/shooting_weapon/projectile/projectile.hpp"

class CrappyReaderData;

/**
 * Class for generic projectile 
 * (has no interesting mechanic, only fly in a straight path and bounce off wall)
*/
class GenericProjectile final : public Projectile {
private:
	GenericProjectile(const GenericProjectile&) = delete;
	GenericProjectile& operator=(const GenericProjectile&) = delete;

private:
	int m_bounce_wall_count;

public:
	GenericProjectile(const std::string& sprite_name);
	GenericProjectile(const Sprite* sprite);

public:
	static std::unique_ptr<GenericProjectile> from_reader(const CrappyReaderData* crd, const std::string& parent_path);

public:
	static std::string class_name();
	virtual std::string get_class_name() const;
	virtual void collision_solid(const CollisionHit& hit) override;
	virtual std::unique_ptr<Projectile> clone(const Vector& pos, uint32_t hurt_attributes, float angle) const override;
};

#endif