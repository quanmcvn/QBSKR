#ifndef HEADER_QBSKR_GUI_MENU_HPP
#define HEADER_QBSKR_GUI_MENU_HPP

#include <memory>
#include <vector>

#include "gui/menu_action.hpp"
#include "math/rectf.hpp"
#include "video/color.hpp"

class Controller;
class DrawingContext;
class ItemAction;
class ItemHorizontalLine;
class ItemLabel;
class MenuItem;
union SDL_Event;

/**
 *  This is the menu, (somewhat) abstract base class
*/
class Menu {
public:
	Menu();
	virtual ~Menu();

private:
	Menu(const Menu&) = delete;
	Menu& operator=(const Menu&) = delete;

private:
	// position of center of the menu (NOT top left!!!)
	Vector m_center_pos;
	std::vector<std::unique_ptr<MenuItem>> m_items;

	float m_menu_width;
	float m_menu_height;

protected:
	int m_active_item;

public:
	// core of this design
	// action will be done based on item's id, through overriding this method
	virtual void item_do_menu_action(MenuItem& item) = 0;
	// called before the menu is exited
	// return true if it should do the back action
	// default to true
	virtual bool on_back_action();

	virtual void process_event(const SDL_Event& event);

	void draw(DrawingContext& drawing_context);

private:
	void draw_item(DrawingContext& drawing_context, int index, int y_pos);

public:
	Vector get_center_pos() const;
	void set_center_pos(float x, float y);

	float get_width() const;
	float get_height() const;

	Rectf get_menu_rect() const;

protected:
	MenuItem& add_item(std::unique_ptr<MenuItem> menu_item);

// a whole lot of Menu::add_xxx(...);
// could've used template (like GameObjectManager)
// but eh
// also if use template then the included file also have to include a lot of things
// which i kinda don't like
public:
	ItemHorizontalLine& add_horizontal_line();
	ItemAction& add_entry(int id, const std::string& text);
	ItemLabel& add_label(const std::string& text);

protected:
	// recalculate the width
	void calculate_width();
	// recalculate the height
	void calculate_height();

public:
	void process_input(const Controller& controller);

private:
	void process_action(const MenuAction& menu_action);
};

#endif