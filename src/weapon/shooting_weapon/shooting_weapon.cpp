#include "weapon/shooting_weapon/shooting_weapon.hpp"

#include "qbskr/room.hpp"
#include "weapon/shooting_weapon/projectile/projectile.hpp"
#include "weapon/shooting_weapon/projectile/projectile_set.hpp"

ShootingWeapon::ShootingWeapon(const std::string& sprite_name) :
	Weapon(sprite_name)
{}

ShootingWeapon::ShootingWeapon(const Sprite* sprite) :
	Weapon(sprite)
{}

void ShootingWeapon::attack()
{
	if (check_timer()) shoot_projectile(get_angle());
}

void ShootingWeapon::shoot_projectile(float angle) const
{
	// wtf is this
	// peak bad design
	const Projectile& projectile = ProjectileSet::current()->get(get_projectile_id());
	Room::get().add_object(projectile.clone(get_projectile_spawn_pos(), get_hurt_attributes(), angle));
}