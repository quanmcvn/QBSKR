#include "gui/mouse_cursor.hpp"

#include "sprite/sprite.hpp"
#include "video/drawing_context.hpp"
#include "video/video_system.hpp"

MouseCursor::MouseCursor(SpritePtr sprite) :
	m_state(MouseCursorState::NORMAL),
	m_applied_state(MouseCursorState::HIDE),
	m_sprite(std::move(sprite))
{}

void
MouseCursor::draw(DrawingContext& context)
{
	if (m_state != MouseCursorState::HIDE) {
		int x, y;
		Uint32 ispressed = SDL_GetMouseState(&x, &y);

		if (ispressed & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			apply_state(MouseCursorState::CLICK);
		} else {
			apply_state(m_state);
		}

		Vector mouse_pos = VideoSystem::current()->get_viewport().to_logical(x, y);

		m_sprite->draw(context.get_canvas(), mouse_pos, LAYER_GUI + 100);
	}
}

void MouseCursor::set_state(MouseCursorState state) { m_state = state; }

void MouseCursor::apply_state(MouseCursorState state)
{
	if (m_applied_state != state) {
		m_applied_state = state;

		switch(state) {
			case MouseCursorState::NORMAL:
				m_sprite->set_action("normal");
				break;

			case MouseCursorState::CLICK:
				m_sprite->set_action("click");
				break;

			case MouseCursorState::LINK:
				m_sprite->set_action("link");
				break;

			case MouseCursorState::HIDE:
				break;
		}
	}
}
