#include "sprite/sprite.hpp"

#include "qbskr/globals.hpp"
#include "sprite/sprite_ptr.hpp"
#include "util/log.hpp"
#include "video/drawing_context.hpp"
#include "video/surface.hpp"

Sprite::~Sprite()
{}

Sprite::Sprite(const Sprite& other) :
	m_data(other.m_data),
	m_frame_progress(other.m_frame_progress),
	m_frame_index(other.m_frame_index),
	m_last_ticks(other.m_last_ticks),
	m_angle(0.0f),
	m_alpha(1.0f),
	m_color(1.0f, 1.0f, 1.0f, 1.0f),
	m_action(other.m_action)
{}

Sprite::Sprite(SpriteData& newdata) :
	m_data(newdata),
	m_frame_progress(0),
	m_frame_index(0),
	m_last_ticks(g_game_time),
	m_angle(),
	m_alpha(1.0f),
	m_color(),
	m_action(m_data.actions.begin()->second.get())
{}

SpritePtr Sprite::clone() const
{
	return std::make_unique<Sprite>(*this);
}

void Sprite::draw(Canvas& canvas, const Vector& pos, int layer, Flip flip)
{
	assert(m_action != nullptr);
	update();

	DrawingContext& drawing_context = canvas.get_drawing_context();
	drawing_context.push_transform();

	drawing_context.set_flip(drawing_context.get_flip() ^ flip);
	drawing_context.set_alpha(drawing_context.get_alpha() * m_alpha);

	canvas.draw_surface(
		m_action->surfaces[m_frame_index],
		pos - Vector(m_action->x_offset, m_action->y_offset),
		m_angle, m_color, layer
	);

	drawing_context.pop_transform();
}

void Sprite::draw_scaled(Canvas& canvas, const Rectf& dstrect, int layer, Flip flip)
{
	assert(m_action != nullptr);
	update();

	DrawingContext& drawing_context = canvas.get_drawing_context();
	drawing_context.push_transform();

	drawing_context.set_flip(drawing_context.get_flip() ^ flip);
	drawing_context.set_alpha(drawing_context.get_alpha() * m_alpha);

	canvas.draw_surface_scaled(m_action->surfaces[m_frame_index], dstrect, m_color, layer);

	drawing_context.pop_transform();
}

void Sprite::update()
{
	float frame_inc = m_action->fps * (g_game_time - m_last_ticks);
	m_last_ticks = g_game_time;

	m_frame_progress += frame_inc;

	// cast to int is better than floor (I think)
	m_frame_index += static_cast<int>(m_frame_progress);
	m_frame_progress -= static_cast<int>(m_frame_progress);

	m_frame_index %= get_frames();
}

void Sprite::set_action(const std::string& name)
{
	if (m_action->name == name) return;

	const SpriteData::Action* new_action = m_data.get_action(name);
	if (!new_action) {
		log_debug << "Action '" << name << "' not found" << std::endl;
		return;
	}

	if (m_action->family_name != new_action->family_name) {
		m_frame_index = 0;
		m_frame_progress = 0;
	}

	m_action = new_action;
}

int Sprite::get_frames() const { return static_cast<int>(m_action->surfaces.size()); }
int Sprite::get_current_frame() const { return m_frame_index; }
float Sprite::get_current_frame_progess() const { return m_frame_progress; }

float Sprite::get_current_hitbox_width() const { return m_action->hitbox_w; }
float Sprite::get_current_hitbox_height() const { return m_action->hitbox_h; }

Vector Sprite::get_current_offset() const { return Vector(m_action->x_offset, m_action->y_offset); }

float Sprite::get_angle() const { return m_angle; }
void Sprite::set_angle(float angle) { m_angle = angle; }

float Sprite::get_alpha() const { return m_alpha; }
void Sprite::set_alpha(float alpha) { m_alpha = alpha; }

Color Sprite::get_color() const { return m_color; }
void Sprite::set_color(const Color& color) { m_color = color; }