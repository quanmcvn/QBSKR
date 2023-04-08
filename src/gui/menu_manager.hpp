#ifndef HEADER_QBSKR_GUI_MENU_MANAGER_HPP
#define HEADER_QBSKR_GUI_MENU_MANAGER_HPP

#include <memory>
#include <vector>

#include "gui/menu_transition.hpp" 
#include "util/currenton.hpp"

class Controller;
class DrawingContext;
class Menu;
/* class MenuTransition; */
union SDL_Event;

class MenuManager final : public Currenton<MenuManager> {
public:
	MenuManager();

private:
	MenuManager(const MenuManager&) = delete;
	MenuManager& operator=(const MenuManager&) = delete;

private:
	std::vector<std::unique_ptr<Menu>> m_menu_stack;
	std::unique_ptr<MenuTransition> m_menu_transition;

public:
	void process_event(const SDL_Event& event);
	void process_input(const Controller& controller);

	void draw(DrawingContext& drawing_context);

	void push_menu(int id, bool do_transition = true);
	void push_menu(std::unique_ptr<Menu> menu, bool do_transition = true);
	void set_menu(int id);
	void set_menu(std::unique_ptr<Menu> menu);
	void pop_menu(bool do_transition = true);
	void clear_menu_stack();

	bool is_active() const;

	Menu* current_menu() const;
	Menu* previous_menu() const;

private:
	void transition(Menu* from, Menu* to);
};

#endif