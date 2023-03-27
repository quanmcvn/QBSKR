#include "badguy/badguy.hpp"

#include "qbskr/room.hpp"

BadGuy::BadGuy(const std::string& sprite_filename) :
	MovingSprite(Vector(0.0f, 0.0f), sprite_filename),
	m_physic(),
	m_direction(Direction::RIGHT),
	m_health(0),
	m_die(),
	m_hit_damage()
{}

BadGuy::BadGuy(const Sprite* sprite) :
	MovingSprite(Vector(0.0f, 0.0f), sprite),
	m_physic(),
	m_direction(Direction::RIGHT),
	m_health(0),
	m_die(),
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

int BadGuy::get_health() const { return m_health; };

bool BadGuy::can_see_player() const
{
	return Room::get().can_see_player(get_pos());
}