#include "qbskr/screen_fade.hpp"

#include "qbskr/globals.hpp"
#include "video/drawing_context.hpp"
#include "video/video_system.hpp"

ScreenFade::ScreenFade(const Vector& dest, float fade_time) :
	m_dest(dest),
	m_fade_time(fade_time),
	m_sum_time(0),
	m_begin_size(std::max(SCREEN_WIDTH, SCREEN_HEIGHT))
{}

void ScreenFade::update(float dt_sec)
{
	m_sum_time += dt_sec;
	if (m_sum_time > m_fade_time) m_sum_time = m_fade_time;
}

void ScreenFade::draw(DrawingContext& drawing_context)
{
	float progress = m_sum_time / m_fade_time;
	float diameter = 2 * m_begin_size * (1.0f - progress);
	drawing_context.get_canvas().draw_inverse_ellipse(
		m_dest, Sizef(1.1f * diameter, diameter),
		Color(0.0f, 0.0f, 0.0f), LAYER_GUI + 1
	);
}

bool ScreenFade::done() const { return m_sum_time >= m_fade_time; }