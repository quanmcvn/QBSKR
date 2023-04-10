#include "gui/menu_manager.hpp"

#include "gui/menu_transition.hpp"
#include "gui/menu.hpp"
#include "gui/menu_set.hpp"
#include "gui/mouse_cursor.hpp"
#include "util/log.hpp"
#include "video/drawing_context.hpp"

MenuManager::MenuManager() :
	m_menu_stack(),
	m_menu_transition(std::make_unique<MenuTransition>())
{}

void MenuManager::process_event(const SDL_Event& event)
{
	if (!m_menu_transition->is_active()) {
		if (current_menu()) {
			current_menu()->process_event(event);
		}
	}
}

void MenuManager::process_input(const Controller& controller)
{
	if (!m_menu_transition->is_active()) {
		if (current_menu()) {
			current_menu()->process_input(controller);
		}
	}
}

void MenuManager::draw(DrawingContext& drawing_context)
{
	if (m_menu_transition->is_active()) {
		m_menu_transition->update();
		m_menu_transition->draw(drawing_context);
	} else {
		if (current_menu()) {
			// bruteforce set rect in case the menu changed size
			m_menu_transition->set(current_menu()->get_menu_rect());
			m_menu_transition->draw(drawing_context);

			current_menu()->draw(drawing_context);
		}
	}

	MouseCursor::current()->draw(drawing_context);
}

void MenuManager::push_menu(int id, bool do_transition)
{
	push_menu(MenuSet::current()->create(static_cast<MenuSet::MenuId>(id)), do_transition);
}

void MenuManager::push_menu(std::unique_ptr<Menu> menu, bool do_transition)
{
	assert(menu);
	if (do_transition) {
		transition(current_menu(), menu.get());
	}
	m_menu_stack.push_back(std::move(menu));
}

void MenuManager::set_menu(int id)
{
	set_menu(MenuSet::current()->create(static_cast<MenuSet::MenuId>(id)));
}

void MenuManager::set_menu(std::unique_ptr<Menu> menu)
{
	transition(current_menu(), menu.get());
	m_menu_stack.clear();
	if (menu) m_menu_stack.push_back(std::move(menu));
}

void MenuManager::pop_menu(bool do_transition)
{
	if (m_menu_stack.empty()) {
		log_warning << "trying to pop empty menu_stack" << std::endl;
	} else {
		if (do_transition) {
			transition(current_menu(), previous_menu());
		}
		m_menu_stack.pop_back();
		MouseCursor::current()->set_state(MouseCursorState::NORMAL);
	}
}

void MenuManager::clear_menu_stack()
{
	transition(current_menu(), nullptr);
	m_menu_stack.clear();
	MouseCursor::current()->set_state(MouseCursorState::NORMAL);
}

bool MenuManager::is_active() const { return !m_menu_stack.empty(); }

Menu* MenuManager::current_menu() const
{
	if (m_menu_stack.empty()) return nullptr;
	else return m_menu_stack.back().get();
}

Menu* MenuManager::previous_menu() const
{
	if (m_menu_stack.size() < 2) return nullptr;
	else return m_menu_stack[static_cast<int>(m_menu_stack.size()) - 2].get();
}

void MenuManager::transition(Menu* from, Menu* to)
{
	if (!from && !to) return;

	Rectf from_rect;
	if (from) from_rect = from->get_menu_rect();
	else from_rect = Rectf(to->get_center_pos(), Sizef(0, 0));

	Rectf to_rect;
	if (to) to_rect = to->get_menu_rect();
	else to_rect = Rectf(from->get_center_pos(), Sizef(0, 0));

	m_menu_transition->start(from_rect, to_rect);
}