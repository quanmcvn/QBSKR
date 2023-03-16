#include "sprite/sprite.hpp"

#include "qbskr/globals.hpp"
#include "util/log.hpp"
#include "video/drawing_context.hpp"
#include "video/surface.hpp"

Sprite::~Sprite()
{}

Sprite::Sprite(SpriteData& newdata) :
	m_data(newdata),
	m_frame_progress(0),
	m_frame_index(0),
	m_last_ticks(g_game_time),
	m_action(m_data.actions.begin()->second.get())
{}

void Sprite::draw(Canvas& canvas, const Vector& pos, int layer, Flip flip)
{
	assert(m_action != nullptr);
	update();

	DrawingContext& drawing_context = canvas.get_drawing_context();
	drawing_context.push_transform();

	drawing_context.set_flip(drawing_context.get_flip() ^ flip);

	canvas.draw_surface(m_action->surfaces[m_frame_index],
	                    pos - Vector(m_action->x_offset, m_action->y_offset),
	                    layer);

	drawing_context.pop_transform();
}

void Sprite::update()
{
	float frame_inc = m_action->fps * (g_game_time - m_last_ticks);
	m_last_ticks = g_game_time;

	m_frame_progress += frame_inc;

	while (m_frame_progress >= 1.0f) {
		m_frame_progress -= 1.0f;
		++ m_frame_index;
	}

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

	m_frame_index = 0;
	m_frame_progress = 0;

	m_action = new_action;
}

int Sprite::get_frames() const { return static_cast<int>(m_action->surfaces.size()); }
int Sprite::get_current_frame() const { return m_frame_index; }
float Sprite::get_current_frame_progess() const { return m_frame_progress; }