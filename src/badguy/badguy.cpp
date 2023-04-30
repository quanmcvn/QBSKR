#include "badguy/badguy.hpp"

#include "math/random.hpp"
#include "qbskr/room.hpp"
#include "weapon/shooting_weapon/projectile/projectile.hpp"
#include "weapon/hurt.hpp"

BadGuy::BadGuy(const std::string& sprite_filename) :
	MovingSprite(Vector(0.0f, 0.0f), sprite_filename),
	m_physic(),
	m_direction(Direction::RIGHT),
	m_health(0),
	m_die(false),
	m_hit_damage()
{}

void BadGuy::update(float dt_sec)
{
	try_change_state();

	switch (m_state) {
		case STATE_INACTIVE:
			inactive_update(dt_sec);
			break;

		case STATE_ACTIVE:
			active_update(dt_sec);
			break;

		default:
			break;
	}

	// only set m_die = true, it's to the derived to decide what to do with it
	if (m_health <= 0) m_die = true;
}

std::string BadGuy::class_name() { return "bad-guy"; }
std::string BadGuy::get_class_name() const { return class_name(); }

HitResponse BadGuy::collision(GameObject& other, const CollisionHit& /* hit */)
{
	if (auto projectile = dynamic_cast<Projectile*>(&other)) {
		if (projectile->get_hurt_attributes() & HURT_BADGUY) {
			if (g_game_random.test_lucky_percent(projectile->get_crit_chance())) {
				m_hit_damage = projectile->get_damage() * 2;
			} else {
				m_hit_damage = projectile->get_damage();
			}
		}
	}

	// put here to make compiler happy
	return ABORT_MOVE;
}

int BadGuy::get_health() const { return m_health; };
bool BadGuy::can_see_player() const { return Room::get().can_see_player(get_pos()); }
bool BadGuy::is_dead() const { return m_die; }