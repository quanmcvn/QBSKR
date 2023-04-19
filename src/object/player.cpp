#include "object/player.hpp"

#include "audio/sound_manager.hpp"
#include "badguy/badguy.hpp"
#include "control/input_manager.hpp"
#include "sprite/sprite_manager.hpp"
#include "sprite/sprite.hpp"
#include "object/camera.hpp"
#include "object/floating_text.hpp"
#include "qbskr/color_scheme.hpp"
#include "qbskr/game_session.hpp"
#include "qbskr/resources.hpp"
#include "qbskr/room.hpp"
#include "math/interpolate.hpp"
#include "math/util.hpp"
#include "video/drawing_context.hpp"
#include "video/video_system.hpp"
#include "weapon/shooting_weapon/projectile/projectile.hpp"
#include "weapon/hurt.hpp"
#include "weapon/weapon_set.hpp"

namespace {
	const float WALK_SPEED = 100.0f;
	const float DIE_ANIMATION_TIME = 2.0f;
}

Player::Player(int player_id, uint32_t weapon_id) :
	m_id(player_id),
	m_controller(&(InputManager::current()->get_controller(m_id))),
	m_direction(Direction::RIGHT),
	m_physic(),
	m_sprite(SpriteManager::current()->create("images/charactors/knight/knight_sprite.txt")),
	m_weapon(),
	m_max_health(5),
	m_health(m_max_health),
	m_die(),
	m_hit_damage(),
	m_die_animation_timer()
{
	set_pos(Vector(0.0f, 0.0f));
	m_collision_object.set_size(m_sprite->get_current_hitbox_width(), m_sprite->get_current_hitbox_height());
	set_pos(Vector(0.0f, 0.0f) - get_bounding_box().get_middle());
	if (weapon_id > 0) m_weapon = WeaponSet::current()->get_weapon(weapon_id).clone(this);
}

Player::~Player()
{}

Player::Player(const Player& other) :
	m_id(other.m_id),
	m_controller(other.m_controller),
	m_direction(other.m_direction),
	m_physic(other.m_physic),
	m_sprite(other.m_sprite->clone()),
	m_weapon(other.m_weapon->clone(this)),
	m_max_health(other.m_max_health),
	m_health(other.m_health),
	m_die(other.m_die),
	m_hit_damage(),
	m_die_animation_timer()
{
	set_pos(other.get_pos());
	m_collision_object.set_size(m_sprite->get_current_hitbox_width(), m_sprite->get_current_hitbox_height());
}

void Player::update(float dt_sec)
{
	if (m_die) {
		set_group(COLLISION_GROUP_DISABLED);
		return;
	}

	handle_input();

	if (m_hit_damage.has_value()) {
		m_health -= m_hit_damage.value();
		Room::get().add<FloatingText>(get_bounding_box().get_middle(), m_hit_damage.value(), ColorScheme::HUD::damage_color);
		m_hit_damage.reset();
	}

	if (m_health <= 0) {
		m_die = true;
		m_health = 0;
	}

	m_collision_object.set_movement(m_physic.get_movement(dt_sec));
}

void Player::draw(DrawingContext& drawing_context)
{
	if (m_die) {
		if (!m_die_animation_timer.started()) {
			m_die_animation_timer.start(DIE_ANIMATION_TIME, false);
		}
		// dim player
		m_sprite->set_color(Color(.5f, .5f, .5f, 1.0f));
		if (m_direction == Direction::RIGHT) {
			m_sprite->set_angle(-90.0f);
		} else {
			m_sprite->set_angle(90.0f);
		}
		if (m_die_animation_timer.check()) {
			GameSession::current()->abort_level();
		}
	} else {
		if (m_weapon) m_weapon->draw(drawing_context);
	}

	std::string action_postfix;
	action_postfix = (m_direction == Direction::RIGHT) ? "-right" : "-left";
	
	if (m_die) {
		m_sprite->set_action("die" + action_postfix);
	} else {
		if (math::length(m_physic.get_velocity()) < 1.0f) {
			m_sprite->set_action("idle" + action_postfix);
		} else {
			m_sprite->set_action("walk" + action_postfix);
		}
	}

	m_sprite->draw(drawing_context.get_canvas(), get_pos(), get_layer());

	// draw HUD for player at the screen
	// this also means no multiplayer (on a single run instance) (or still can but HUD is bad)
	// design choice (?)

	drawing_context.push_transform();
	// remove translation caused by camera
	// HUD for player is fixed on screen
	drawing_context.set_translation(Vector(0.0f, 0.0f));
	// note that scale is also applied to HUD

	auto& canvas = drawing_context.get_canvas();

	// since 60 is highly composite, choose this will make it look nice (in theory)
	const float HEALTH_BAR_WIDTH = 60;
	// I found 6 : 1 is a quite good ratio
	const float HEALTH_BAR_HEIGHT = HEALTH_BAR_WIDTH / 6;
	
	canvas.draw_filled_rect(Rectf(
			Vector(5, 5),
			Sizef(70, 20)
		), 
		Color::from_rgb(0xA9, 0x88, 0x66), LAYER_HUD
	);
	{
		Vector pos = Vector(10, 10);
		Sizef size_health_bar(HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT);
		Sizef size_real(
			interpolate::linear_interpolation(
				static_cast<float>(m_health) / static_cast<float>(m_max_health),
				0, HEALTH_BAR_WIDTH
			),
			HEALTH_BAR_HEIGHT
		);
		canvas.draw_filled_rect(Rectf(pos, size_health_bar), Color::from_rgb(0x63, 0x56, 0x34), LAYER_HUD);
		canvas.draw_filled_rect(Rectf(pos, size_real), Color::RED, LAYER_HUD);
		canvas.draw_text(
			Resources::fixed_font, 
			std::to_string(m_health) + "/" + std::to_string(m_max_health),
			// the reason for this weird x, y shenanigan is because I want the text to be the middle
			// but text.x is coded different from text.y :/
			Vector(
				Rectf(pos, size_health_bar).get_middle().x,
				pos.y
			),
			ALIGN_CENTER, LAYER_HUD + 1, Color(1.0f, 1.0f, 1.0f)
		);
	}

	drawing_context.pop_transform();
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

HitResponse Player::collision(GameObject& other, const CollisionHit& /* hit */)
{
	if (dynamic_cast<BadGuy*>(&other)) {
		return ABORT_MOVE;
	}

	if (auto projectile = dynamic_cast<Projectile*>(&other)) {
		if (projectile->get_hurt_attributes() & HURT_PLAYER) {
			m_hit_damage = projectile->get_damage();
		}
	}

	return CONTINUE;
}

void Player::collision_tile(uint32_t tile_attributes)
{
	if (tile_attributes & Tile::GOAL) {
		if (m_controller->pressed(Control::ATTACK)) {
			GameSession::current()->finish_level();
		}
	}
}

int Player::get_layer() const { return LAYER_OBJECTS + 1; }

int Player::get_id() const { return m_id; }
void Player::set_id(int id) { m_id = id; m_controller = &(InputManager::current()->get_controller(m_id)); }

void Player::handle_input()
{
	handle_movement_input();

	if (m_weapon) handle_weapon();
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

void Player::handle_weapon()
{
	Vector mouse_pos = 
	     VideoSystem::current()->get_viewport().to_logical(m_controller->get_mouse_pos().x, m_controller->get_mouse_pos().y)
	   + Room::get().get_camera().get_translation();
	Vector line_player_mouse_pos = mouse_pos - get_bounding_box().get_middle();

	float angle = math::radian_to_degree(std::atan2(line_player_mouse_pos.y, line_player_mouse_pos.x));
	m_weapon->set_angle(angle);
	
	if (std::abs(angle) >= 90.0f) {
		m_weapon->set_flip(VERTICAL_FLIP);
		m_direction = Direction::LEFT;
	} else {
		m_weapon->set_flip(NO_FLIP);
		m_direction = Direction::RIGHT;
	}

	if (m_controller->hold(Control::ATTACK)) {
		SoundManager::current()->play_sound("sounds/player/shoot.wav");
		m_weapon->attack();
	}
}

std::unique_ptr<Player> Player::clone() const
{
	auto player = std::make_unique<Player>(*this);
	return player;
}