#ifndef HEADER_QBSKR_GUI_MOUSE_CURSOR_HPP
#define HEADER_QBSKR_GUI_MOUSE_CURSOR_HPP

#include "util/currenton.hpp"
#include "sprite/sprite_ptr.hpp"

class DrawingContext;

enum class MouseCursorState {
	NORMAL,
	CLICK,
	LINK,
	/**
	 *  tempting to use dekisugi since "HIDEtoshi Dekisugi"
	 *  ...
	 *  sorry...
	 */
	HIDE
};

/**
 *  Custom mouse cursor
*/
class MouseCursor final : public Currenton<MouseCursor> {
private:
	MouseCursor(const MouseCursor&) = delete;
	MouseCursor& operator=(const MouseCursor&) = delete;

private:
	MouseCursorState m_state;
	MouseCursorState m_applied_state;
	SpritePtr m_sprite;

public:
	MouseCursor(SpritePtr sprite);

public:
	void draw(DrawingContext& context);

	void set_state(MouseCursorState state);

private:
	void apply_state(MouseCursorState state);
};

#endif