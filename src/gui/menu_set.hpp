#ifndef HEADER_QBSKR_GUI_MENU_SET_HPP
#define HEADER_QBSKR_GUI_MENU_SET_HPP

#include <memory>

#include "util/currenton.hpp"

class Menu;

class MenuSet final : public Currenton<MenuSet> {
public:
	MenuSet();

private:
	MenuSet(const MenuSet&) = delete;
	MenuSet& operator=(const MenuSet&) = delete;

public:
	enum MenuId {
		NO_MENU,
		MAIN_MENU,
		GAME_MENU
	};

public:
	std::unique_ptr<Menu> create(MenuId menu_id);
};

#endif