#include "badguy/generic_badguy.hpp"

#include <algorithm>
#include <stdexcept>

#include "audio/sound_manager.hpp"
#include "math/interpolate.hpp"
#include "math/random.hpp"
#include "math/util.hpp"
#include "object/player.hpp"
#include "qbskr/room.hpp"
#include "util/crappy_reader_data.hpp"
#include "qbskr/color_scheme.hpp"
#include "qbskr/resources.hpp"
#include "weapon/shooting_weapon/projectile/projectile.hpp"
#include "weapon/hurt.hpp"
#include "weapon/weapon.hpp"
#include "weapon/weapon_set.hpp"

namespace {
	// arbitrary choice
	const float WALK_SPEED = 32.0f;
	// arbitrary choice
	const float WANDER_CHANCE = 0.02f;
	// arbitrary choice
	const float DIE_ANIMATION_TIME = 0.4f;
	// arbitrary choice
	const float KNOCKBACK_ANIMATION_TIME = 0.1f;
	// arbitrary choice
	const float HIT_TEXT_TIME = 1.0f;
	// arbitrary choice
	const float KNOCKBACK_SPEED = 100.0f;
}

GenericBadGuy::~GenericBadGuy()
{
	m_weapon.reset();
}

GenericBadGuy::GenericBadGuy(const std::string& sprite_filename) :
	BadGuy(sprite_filename),
	m_weapon()
{}

std::unique_ptr<GenericBadGuy> GenericBadGuy::from_reader(const CrappyReaderData* crd)
{
	int health = 10;
	crd->get("health", health);

	uint32_t weapon_id;
	if (!crd->get("weapon-id", weapon_id)) {
		throw std::runtime_error("Missing weapon-id in badguy-specific");
	}

	Vector weapon_pos_offset;
	if (CrappyReaderData* crd_weapon_pos_offset = crd->get_child("weapon-pos-offset")) {
		crd_weapon_pos_offset->get("x", weapon_pos_offset.x);
		crd_weapon_pos_offset->get("y", weapon_pos_offset.y);
	}

	float m_attack_per_sec = .25f;
	crd->get("attack-per-sec", m_attack_per_sec);

	float m_attack_chance = .5f;
	crd->get("attack-chance", m_attack_chance);

	std::string sprite_filename;
	if (!crd->get("sprite-filename", sprite_filename)) {
		throw std::runtime_error("Missing sprite-filename in badguy-specific");
	}

	auto badguy = std::make_unique<GenericBadGuy>(crd->m_parent_path + sprite_filename);
	badguy->m_weapon = WeaponSet::current()->get_weapon(weapon_id).clone(badguy.get());
	badguy->m_weapon->set_pos_offset(weapon_pos_offset);
	badguy->m_health = health;
	badguy->m_attack_timer.start(1.0 / m_attack_per_sec, true);
	badguy->m_attack_chance = m_attack_chance;

	return badguy;
}

void GenericBadGuy::update(float dt_sec)
{
	try_change_state();
	
	if (m_die) {
		set_group(COLLISION_GROUP_DISABLED);
		return;
	}

	if (m_hit_damage.has_value()) {
		m_last_hit_damage = m_hit_damage.value();
		m_health -= m_hit_damage.value();
		m_hit_damage.reset();
		SoundManager::current()->play_sound("sounds/badguy/generic_badguy_hurt.wav");
	}

	if (!m_knockback_animation_timer.started()) {
		wander();
	}

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

	if (m_health <= 0) m_die = true;

	m_collision_object.set_movement(m_physic.get_movement(dt_sec));
}

void GenericBadGuy::draw(DrawingContext& drawing_context)
{
	if (m_die) {
		if (!m_die_animation_timer.started()) {
			m_die_animation_timer.start(DIE_ANIMATION_TIME, false);
		}
		set_layer(LAYER_TILES + 10);
		// dim badguy
		m_sprite->set_color(Color(.5f, .5f, .5f, 1.0f));
		// use interpolate to calculate the angle to rotate sprite
		float time_die_animation = std::clamp(m_die_animation_timer.get_timegone() / m_die_animation_timer.get_period(), 0.0f, 1.0f);
		if (m_direction == Direction::RIGHT) {
			m_sprite->set_angle(interpolate::quadratic_ease_out(time_die_animation, 0.0f, 90.0f));
		} else {
			m_sprite->set_angle(interpolate::quadratic_ease_out(time_die_animation, 0.0f, -90.0f));
		}
	} else {
		if (m_state == STATE_INACTIVE) {
			m_weapon->set_angle(math::angle(m_physic.get_velocity()));
		}

		float weapon_angle = m_weapon->get_angle();

		if (std::abs(weapon_angle) >= 90.0f) {
			m_weapon->set_flip(VERTICAL_FLIP);
			m_direction = Direction::LEFT;
		} else {
			m_weapon->set_flip(NO_FLIP);
			m_direction = Direction::RIGHT;
		}
		
		m_weapon->draw(drawing_context);
	}

	std::string action_postfix;
	action_postfix = (m_direction == Direction::RIGHT) ? "-right" : "-left";

	if (m_hit_damage.has_value()) {
		m_knockback_animation_timer.start(KNOCKBACK_ANIMATION_TIME, false);
		m_hit_text_timer.start(HIT_TEXT_TIME, false);
	}

	if (m_die) {
		m_sprite->set_action("die" + action_postfix);
	} else {
		if (!m_knockback_animation_timer.ended()) {
			m_sprite->set_action("knockback" + action_postfix);
		} else {
			m_sprite->set_action("idle" + action_postfix);
		}
		if (!m_hit_text_timer.ended()) {
			drawing_context.get_canvas().draw_text(
				Resources::fixed_font, 
				std::to_string(m_last_hit_damage), 
				get_pos(), 
				ALIGN_LEFT, 
				LAYER_HUD, 
				ColorScheme::HUD::damage_color);
		}
	}

	MovingSprite::draw(drawing_context);
}

void GenericBadGuy::collision_solid(const CollisionHit& hit)
{
	if (hit.left || hit.right) {
		m_physic.inverse_velocity_x();
	}

	if (hit.top || hit.bottom) {
		m_physic.inverse_velocity_y();
	}
}

HitResponse GenericBadGuy::collision(GameObject& other, const CollisionHit& /* hit */)
{
	if (auto projectile = dynamic_cast<Projectile*>(&other)) {
		if (projectile->get_hurt_attributes() & HURT_BADGUY) {
			m_hit_damage = projectile->get_damage();
		}
	}
	return CONTINUE;
}

void GenericBadGuy::active_update(float /* dt_sec */)
{
	Vector player_pos = Room::get().get_nearest_player(get_pos())->get_pos();
	Vector line_player_pos = player_pos - get_pos();
	float angle = math::angle(line_player_pos);

	m_weapon->set_angle(angle);

	if (std::abs(angle) >= 90.0f) {
		m_weapon->set_flip(VERTICAL_FLIP);
		m_direction = Direction::LEFT;
	} else {
		m_weapon->set_flip(NO_FLIP);
		m_direction = Direction::RIGHT;
	}

	// generic badguy only attack once in a while
	if (m_attack_timer.check()) {
		// if lucky, it attack
		// else, another timer await
		if (g_game_random.test_lucky(m_attack_chance)) {
			m_weapon->attack();
		} else {
			m_attack_timer.start_true(m_attack_timer.get_period(), true);
		}
	}
}

void GenericBadGuy::inactive_update(float /* dt_sec */)
{}

void GenericBadGuy::try_change_state()
{
	if (can_see_player()) {
		m_state = STATE_ACTIVE;
	} else {
		m_state = STATE_INACTIVE;
	}
}

std::unique_ptr<BadGuy> GenericBadGuy::clone(const Vector& pos) const
{
	auto badguy = std::make_unique<GenericBadGuy>(m_sprite_name);
	badguy->set_pos(pos);
	badguy->m_weapon = m_weapon->clone(badguy.get());
	badguy->m_weapon->set_pos_offset(m_weapon->get_pos_offset());
	badguy->m_health = m_health;
	badguy->m_attack_timer.start(m_attack_timer.get_period(), true);
	badguy->m_attack_chance = m_attack_chance;

	return badguy;
}

void GenericBadGuy::wander()
{
	if (g_game_random.test_lucky(WANDER_CHANCE)) {
		// random speed uniformly in quad I
		// add random an angle from -45 deg to 45 deg
		// no sharp angle change
		// Vector speed(g_game_random.randf(-WALK_SPEED, WALK_SPEED), g_game_random.randf(-WALK_SPEED, WALK_SPEED));
		Vector speed(WALK_SPEED, WALK_SPEED);
		
		// float current_angle = math::angle(m_physic.get_velocity());
		// float added_angle = g_game_random.randf(-45.0f, 45.0f);
		speed = math::rotate(speed, g_game_random.randf(-180.0f, 180.0f));
		m_physic.set_velocity(speed);
	}
}