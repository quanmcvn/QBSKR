#ifndef HEADER_QBSKR_GUI_MENU_ITEM_HPP
#define HEADER_QBSKR_GUI_MENU_ITEM_HPP

#include <string>

#include "gui/menu.hpp"

class MenuItem {
public:
	virtual ~MenuItem();

private:
	MenuItem(const MenuItem&) = delete;
	MenuItem& operator=(const MenuItem&) = delete;

private:
	int m_id;
	std::string m_text;

public:
	MenuItem(const std::string& text, int id = -1);

public:
	int get_id() const;

	void set_text(const std::string& text);
	const std::string& get_text() const;

	virtual void draw(DrawingContext& drawing_context, const Vector& pos, int menu_width, bool active);

	// return true when the menu item has action therefore can be selected
	// default to true
	virtual bool is_selectable() const;

	// return width of menu item
	// default to width of text + magic number
	virtual int get_width() const;

	// return height of menu item
	// default to a magic number (assume the text is 1 line)
	virtual int get_height() const;

	// process the menu action
	// default to do nothing
	virtual void process_action(const MenuAction& action);

	// process the given event
	// default to do nothing
	virtual void process_event(const SDL_Event& event);

	virtual Color get_color() const;
};

#endif