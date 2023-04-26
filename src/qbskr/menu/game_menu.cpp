#include "qbskr/menu/game_menu.hpp"

#include "gui/menu_item.hpp"
#include "gui/menu_manager.hpp"
#include "qbskr/game_session.hpp"

GameMenu::GameMenu()
{
	add_label("Paused");
	add_horizontal_line();
	add_entry(MNID_CONTINUE, "Continue");
	add_horizontal_line();
	add_entry(MNID_ABORTLEVEL, "Quit");
}

void GameMenu::item_do_menu_action(MenuItem& item)
{
	switch (item.get_id()) {
		case MNID_CONTINUE:
			MenuManager::current()->clear_menu_stack();
			// this toggle pause here have no real effect :D
			GameSession::current()->toggle_pause();
			break;

		case MNID_ABORTLEVEL:
			MenuManager::current()->clear_menu_stack();
			GameSession::current()->abort_level();
			break;
	}
}