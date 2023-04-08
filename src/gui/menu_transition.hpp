#ifndef HEADER_QBSKR_GUI_MENU_TRANSITION_HPP
#define HEADER_QBSKR_GUI_MENU_TRANSITION_HPP

#include "math/rectf.hpp"

class DrawingContext;

/**
 *  Although it is called 'menu' transistion, it can be re-used
 *  IF, use other functions than the draw()
*/
class MenuTransition final {
public:
	MenuTransition();

private:
	Rectf m_from_rect;
	Rectf m_to_rect;
	float m_effect_progress;
	float m_effect_start_time;
	bool m_is_active;

public:
	void start(const Rectf& from_rect, const Rectf& to_rect);
	void set(const Rectf& rect);
	void update();
	void draw(DrawingContext& drawing_context);
	Rectf get_current_rect() const;
	bool is_active() const;
};

#endif