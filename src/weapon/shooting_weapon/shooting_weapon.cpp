#include "weapon/shooting_weapon/shooting_weapon.hpp"

#include "audio/sound_manager.hpp"
#include "qbskr/room.hpp"
#include "weapon/shooting_weapon/projectile/projectile.hpp"
#include "weapon/shooting_weapon/projectile/projectile_set.hpp"

ShootingWeapon::ShootingWeapon(const std::string& sprite_name) :
	Weapon(sprite_name),
	m_sound_file(),
	m_timer(),
	m_inaccuracy(),
	m_projectiles_per_attack()
{}

void ShootingWeapon::attack()
{
	if (m_timer.check()) {
		for (int i = 0; i < m_projectiles_per_attack; ++ i) shoot_projectile(get_shoot_angle());
		if (!m_sound_file.empty()) SoundManager::current()->play_sound(m_sound_file);
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