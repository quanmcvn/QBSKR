#include "weapon/shooting_weapon/shooting_weapon.hpp"

#include "qbskr/room.hpp"
#include "weapon/shooting_weapon/projectile/projectile.hpp"
#include "weapon/shooting_weapon/projectile/projectile_set.hpp"

ShootingWeapon::ShootingWeapon(const std::string& sprite_name) :
	Weapon(sprite_name)
{}

void ShootingWeapon::attack(int times)
{
	if (check_timer()) {
		for (int i = 0; i < times; ++ i) shoot_projectile(get_shoot_angle());
		play_shoot_sound();
	}
}

void ShootingWeapon::shoot_projectile(float angle) const
{
	// wtf is this
	// peak bad design
	const Projectile& projectile = ProjectileSet::current()->get_projectile(get_projectile_id());
	// only spawn if
	// free of tile and in bound
	Rectf projectile_spawn_bounding_box = Rectf(get_projectile_spawn_pos(), projectile.get_bounding_box().get_size());
	if (Room::get().is_free_of_tiles(projectile_spawn_bounding_box) && Room::get().inside(projectile_spawn_bounding_box)) {
		Room::get().add_object(projectile.clone(get_projectile_spawn_pos(), get_hurt_attributes(), angle));
	}
}

float ShootingWeapon::get_shoot_angle() const
{
	return get_angle();
}