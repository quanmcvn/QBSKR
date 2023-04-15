#include "badguy/balbusour.hpp"

#include "audio/sound_manager.hpp"
#include "math/interpolate.hpp"
#include "math/random.hpp"
#include "object/floating_text.hpp"
#include "qbskr/color_scheme.hpp"
#include "qbskr/room.hpp"
#include "util/crappy_reader_data.hpp"
#include "video/drawing_context.hpp"
#include "weapon/shooting_weapon/projectile/projectile_set.hpp"
#include "weapon/shooting_weapon/projectile/projectile.hpp"
#include "weapon/hurt.hpp"

Balbusour::Balbusour() :
	BadGuy("images/badguys/balbusour/balbusour-sprite.txt"),
	m_full_health(),
	m_attack1_cooldown(),
	m_attack2_cooldown(),
	m_attack2_attack_time(),
	m_attack2_attack_freq()
{
	m_attack1_cooldown.start(1.5, true);
	m_attack2_cooldown.start(2, false);
}

std::unique_ptr<Balbusour> Balbusour::from_reader(const CrappyReaderData* crd)
{
	int health = 120;
	crd->get("health", health);

	auto badguy = std::make_unique<Balbusour>();
	badguy->m_health = health;
	badguy->m_full_health = health;

	return badguy;
}

void Balbusour::draw(DrawingContext& drawing_context)
{
	if (m_die) {
		set_layer(LAYER_TILES + 10);
		// dim badguy
		m_sprite->set_color(Color(.5f, .5f, .5f, 1.0f));
	}
	
	if (!m_die) {
		// draw health bar
		const float DENT = 10;
		const float HEALTH_DENT = 2;
		const float VERTICAL_LIFT = 5;
		const float HEIGHT = 10;
		// draw the background of health bar
		drawing_context.get_canvas().draw_filled_rect(
			Rectf(
				Vector(get_bounding_box().get_left() - DENT, get_bounding_box().get_top() - VERTICAL_LIFT - HEIGHT),
				Vector(get_bounding_box().get_right() + DENT, get_bounding_box().get_top() - VERTICAL_LIFT)
			),
			Color::BLACK,
			LAYER_HUD
		);

		// draw the health bar with (m_health / m_full_health) * 100% full (with a catch)
		drawing_context.get_canvas().draw_filled_rect(
			Rectf(
				Vector(get_bounding_box().get_left() - (DENT - HEALTH_DENT), get_bounding_box().get_top() - VERTICAL_LIFT - HEIGHT + HEALTH_DENT),
				Vector(
					interpolate::linear_interpolation(
						static_cast<float>(m_health) / static_cast<float>(m_full_health),
						// design decision: +1 to make health bar at least 1 pixel
						// when health <= 0, the health bar isn't draw so it's not a problem
						get_bounding_box().get_left() - (DENT - HEALTH_DENT) + 1, get_bounding_box().get_right() + (DENT - HEALTH_DENT)
					), 
					get_bounding_box().get_top() - VERTICAL_LIFT - HEALTH_DENT)
			),
			Color::RED,
			LAYER_HUD
		);
	}

	MovingSprite::draw(drawing_context);
}

HitResponse Balbusour::collision(GameObject& other, const CollisionHit& hit)
{
	BadGuy::collision(other, hit);
	// boss generally isn't knockback-able (english?)
	return FORCE_MOVE;
}

void Balbusour::active_update(float /* dt_sec */)
{
	if (m_die) {
		set_group(COLLISION_GROUP_DISABLED);
		return;
	}

	if (m_hit_damage.has_value()) {
		m_health -= m_hit_damage.value();
		Vector pos_text = Vector(
			g_game_random.randf(get_bounding_box().get_left(), get_bounding_box().get_right()),
			g_game_random.randf(get_bounding_box().get_top(), get_bounding_box().get_bottom())
		);
		Room::get().add<FloatingText>(pos_text, m_hit_damage.value(), ColorScheme::HUD::damage_color);
		m_hit_damage.reset();
		SoundManager::current()->play_sound("sounds/badguy/generic_badguy_hurt.wav");
	}

	do_attack1();
	do_attack2();
}

void Balbusour::inactive_update(float /* dt_sec */) { /* nothing */}

void Balbusour::try_change_state()
{
	// boss is always active
	m_state = STATE_ACTIVE;
}

std::unique_ptr<BadGuy> Balbusour::clone(const Vector& pos) const
{
	auto badguy = std::make_unique<Balbusour>();
	badguy->set_pos(pos);
	badguy->m_health = m_health;
	badguy->m_full_health = m_full_health;

	return badguy;
}

void Balbusour::do_attack1()
{
	if (m_attack1_cooldown.check()) {
		for (float angle = 0; angle <= 360; angle += 15) {
			Room::get().add_object(ProjectileSet::current()->get_projectile(2).clone(get_bounding_box().get_middle(), HURT_PLAYER, angle));
		}
	}
}

void Balbusour::do_attack2()
{
	// this attack is bullet spinning around Balbusour
	// hope this is easy to understand

	if (m_attack2_attack_time.ended()) {
		if (!m_attack2_cooldown.started()) {
			m_attack2_cooldown.start(3, false);
		}
		if (m_attack2_cooldown.check()) {
			// start attack
			m_attack2_attack_time.start(3.6, false);
			m_attack2_attack_freq.start_true(0.075, true);
		}
	} else {
		// if can attack (real)
		if (m_attack2_attack_freq.check()) {
			float angle = interpolate::linear_interpolation(m_attack2_attack_time.get_timegone_normalized(), 0, 360);
			float angle2 = angle + 90;
			float angle3 = angle + 180;
			float angle4 = angle + 270;
			const int BULLET_TYPE = 3;
			Room::get().add_object(ProjectileSet::current()->get_projectile(BULLET_TYPE).clone(get_bounding_box().get_middle(), HURT_PLAYER, angle));
			Room::get().add_object(ProjectileSet::current()->get_projectile(BULLET_TYPE).clone(get_bounding_box().get_middle(), HURT_PLAYER, angle2));
			Room::get().add_object(ProjectileSet::current()->get_projectile(BULLET_TYPE).clone(get_bounding_box().get_middle(), HURT_PLAYER, angle3));
			Room::get().add_object(ProjectileSet::current()->get_projectile(BULLET_TYPE).clone(get_bounding_box().get_middle(), HURT_PLAYER, angle4));
		}
	}
}