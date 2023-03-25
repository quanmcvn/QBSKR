#include "badguy/badguy.hpp"

#include "qbskr/room.hpp"

BadGuy::BadGuy(const std::string& sprite_filename) :
	MovingSprite(Vector(0.0f, 0.0f), sprite_filename)
{}

BadGuy::BadGuy(const Sprite* sprite) :
	MovingSprite(Vector(0.0f, 0.0f), sprite)
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
}

void BadGuy::draw(DrawingContext& drawing_context)
{
	std::string action_postfix;
	action_postfix = (m_direction == Direction::RIGHT) ? "-right" : "-left";

	m_sprite->set_action("idle" + action_postfix);

	MovingSprite::draw(drawing_context);
}

std::string BadGuy::class_name() { return "bad-guy"; }
std::string BadGuy::get_class_name() const { return class_name(); }

bool BadGuy::can_see_player() const
{
	return Room::get().can_see_player(get_pos());
}