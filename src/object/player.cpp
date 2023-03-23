#include "object/player.hpp"

#include "control/input_manager.hpp"
#include "sprite/sprite_manager.hpp"
#include "sprite/sprite.hpp"
#include "qbskr/room.hpp"
#include "math/util.hpp"
#include "video/video_system.hpp"
#include "weapon/weapon_set.hpp"

namespace {
	const float WALK_SPEED = 100.0f;

	const float PLAYER_WIDTH = 10.0f;
	const float PLAYER_HEIGHT = 10.0f;
}

Player::Player(int player_id) :
	m_id(player_id),
	m_controller(&(InputManager::current()->get_controller(m_id))),
	m_direction(Direction::RIGHT),
	m_physic(),
	m_sprite(SpriteManager::current()->create("images/charactors/knight/knight_sprite.txt")),
	m_weapon()
{
	set_pos(Vector(17.0f, 17.0f));
	m_collision_object.set_size(PLAYER_WIDTH, PLAYER_HEIGHT);
}

Player::Player(int player_id, uint32_t weapon_id) :
	m_id(player_id),
	m_controller(&(InputManager::current()->get_controller(m_id))),
	m_direction(Direction::RIGHT),
	m_physic(),
	m_sprite(SpriteManager::current()->create("images/charactors/knight/knight_sprite.txt")),
	m_weapon()
{
	set_pos(Vector(17.0f, 17.0f));
	m_collision_object.set_size(PLAYER_WIDTH, PLAYER_HEIGHT);
	// should be OK here even though I did a cast down hierachy
	m_weapon = static_cast<Weapon*>(&Room::get().add_object(WeaponSet::current()->get_weapon(weapon_id).clone(this)));
}

Player::~Player()
{}

void Player::update(float dt_sec)
{
	handle_input();

	m_collision_object.set_movement(m_physic.get_movement(dt_sec));
}

void Player::draw(DrawingContext& drawing_context)
{
	std::string action_postfix;
	action_postfix = (m_direction == Direction::RIGHT) ? "-right" : "-left";
	
	if (math::length(m_physic.get_velocity()) < 1.0f) {
		m_sprite->set_action("idle" + action_postfix);
	} else {
		m_sprite->set_action("walk" + action_postfix);
	}

	m_sprite->draw(drawing_context.get_canvas(), get_pos(), get_layer());
}

void Player::collision_solid(const CollisionHit& hit)
{
	if (hit.left || hit.right) {
		m_physic.set_velocity_x(0);
	}

	if (hit.top || hit.bottom) {
		m_physic.set_velocity_y(0);
	}
}

HitResponse Player::collision(GameObject& /* other */, const CollisionHit& /* hit */)
{
	// NYI
	return ABORT_MOVE;
}

void Player::collision_tile(uint32_t /* tile_attributes */)
{
	// NYI
}

int Player::get_layer() const { return LAYER_OBJECTS + 1; }

int Player::get_id() const { return m_id; }
void Player::set_id(int id) { m_id = id; m_controller = &(InputManager::current()->get_controller(m_id)); }

void Player::handle_input()
{
	handle_movement_input();

	if (m_weapon) {
		Vector mouse_pos = VideoSystem::current()->get_viewport().to_logical(m_controller->get_mouse_pos().x, m_controller->get_mouse_pos().y);
		Vector line_player_mouse_pos = mouse_pos - get_bounding_box().get_middle();

		float angle = math::radian_to_degree(std::atan2(line_player_mouse_pos.y, line_player_mouse_pos.x));
		m_weapon->set_angle(angle);
		
		if (std::abs(angle) >= 90.0f) {
			m_weapon->set_flip(VERTICAL_FLIP);
		} else {
			m_weapon->set_flip(NO_FLIP);
		}

		if (m_controller->hold(Control::ATTACK)) {
			m_weapon->attack();
		}
	}
}

void Player::handle_movement_input()
{
	float dir_x_sign = 0;
	if (m_controller->hold(Control::LEFT) && !m_controller->hold(Control::RIGHT)) {
		dir_x_sign = -1;
		m_direction = Direction::LEFT;
	} else if (!m_controller->hold(Control::LEFT) && m_controller->hold(Control::RIGHT)) {
		dir_x_sign = 1;
		m_direction = Direction::RIGHT;
	}

	float dir_y_sign = 0;
	if (m_controller->hold(Control::UP) && !m_controller->hold(Control::DOWN)) {
		dir_y_sign = -1;
	} else if (!m_controller->hold(Control::UP) && m_controller->hold(Control::DOWN)) {
		dir_y_sign = 1;
	}

	m_physic.set_velocity_x(dir_x_sign * WALK_SPEED);
	m_physic.set_velocity_y(dir_y_sign * WALK_SPEED);
}