#include "gui/menu_set.hpp"

#include "qbskr/menu/game_menu.hpp"
#include "qbskr/menu/main_menu.hpp"
#include "util/log.hpp"

MenuSet::MenuSet()
{}

std::unique_ptr<Menu> MenuSet::create(MenuId menu_id)
{
	switch (menu_id) {
		case MAIN_MENU:
			return std::make_unique<MainMenu>();
		
		case GAME_MENU:
			return std::make_unique<GameMenu>();

		case NO_MENU:
			// use std::unique_ptr instead of std::make_unique since Menu is an incomplete type
			return std::unique_ptr<Menu>();

		default:
			log_warning << "unknown MenuId provided" << std::endl;
			assert(false);
			// to make compiler happy
			return std::unique_ptr<Menu>();
	}
}