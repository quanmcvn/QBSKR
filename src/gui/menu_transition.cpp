#include "gui/menu_transition.hpp"

#include "math/interpolate.hpp"
#include "qbskr/color_scheme.hpp"
#include "qbskr/globals.hpp"
#include "video/drawing_context.hpp"

namespace {
	// multiplier
	// x6 is equivalemt to ~0.16666s to complete
	const float TRANSITION_MULTIPLIER = 6.0f;
}

MenuTransition::MenuTransition() :
	m_from_rect(),
	m_to_rect(),
	m_effect_progress(1.0f),
	m_effect_start_time(),
	m_is_active(false)
{}

void MenuTransition::start(const Rectf& from_rect, const Rectf& to_rect)
{
	m_from_rect = from_rect;
	m_to_rect = to_rect;

	m_effect_start_time = g_real_time;
	m_effect_progress = 0.0f;

	m_is_active = true;
}

void MenuTransition::set(const Rectf& rect) { m_from_rect = m_to_rect = rect; }

void MenuTransition::update()
{
	if (m_is_active) {
		m_effect_progress = (g_real_time - m_effect_start_time) * TRANSITION_MULTIPLIER;

		if (m_effect_progress > 1.0f) {
			m_effect_progress = 1.0f;
			m_is_active = false;
		}
	}
}

void MenuTransition::draw(DrawingContext& drawing_context)
{
	Rectf rect = get_current_rect();

	drawing_context.get_canvas().draw_filled_rect(rect, ColorScheme::Menu::front_color, LAYER_GUI - 10);
}

Rectf MenuTransition::get_current_rect() const
{
	if (!m_is_active) return m_to_rect;

	float progress = m_effect_progress;
	return Rectf(
		interpolate::linear_interpolation(progress, m_from_rect.get_left(), m_to_rect.get_left()),
		interpolate::linear_interpolation(progress, m_from_rect.get_top(), m_to_rect.get_top()),
		interpolate::linear_interpolation(progress, m_from_rect.get_right(), m_to_rect.get_right()),
		interpolate::linear_interpolation(progress, m_from_rect.get_bottom(), m_to_rect.get_bottom())
	);
}

bool MenuTransition::is_active() const { return m_is_active; }