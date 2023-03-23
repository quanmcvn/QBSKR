#include "weapon/shooting_weapon/shooting_weapon.hpp"

#include "qbskr/room.hpp"
#include "weapon/shooting_weapon/projectile.hpp"
#include "weapon/shooting_weapon/projectile_set.hpp"

ShootingWeapon::ShootingWeapon(MovingObject* parent, const std::string& sprite_name) :
	Weapon(parent, sprite_name)
{}

ShootingWeapon::ShootingWeapon(MovingObject* parent, const Sprite* sprite) :
	Weapon(parent, sprite)
{}

void ShootingWeapon::attack()
{
	if (check_timer()) shoot_projectile(get_projectile_speed(), get_angle());
}

void ShootingWeapon::shoot_projectile(float speed, float angle) const
{
	// wtf is this
	// peak bad design
	const Projectile& projectile = ProjectileSet::current()->get(get_projectile_id());
	Room::get().add_object(projectile.clone(get_projectile_spawn_pos(), get_hurt_attributes(), speed, angle));
}