#include "qbskr/menu/main_menu.hpp"

#include "gui/menu_item.hpp"
#include "gui/menu_manager.hpp"
#include "gui/menu_set.hpp"
#include "qbskr/credit_screen.hpp"
#include "qbskr/game_session.hpp"
#include "qbskr/globals.hpp"
#include "qbskr/level.hpp"
#include "qbskr/screen_manager.hpp"
#include "qbskr/screen_fade.hpp"
#include "video/video_system.hpp"

MainMenu::MainMenu()
{
	set_center_pos(static_cast<float>(SCREEN_WIDTH) / 2.0f, static_cast<float>(SCREEN_HEIGHT) / 2.0f);

	add_label("QBSKR");
	add_horizontal_line();
	add_entry(MNID_STARTGAME, "Start Game");
	add_entry(MNID_CREDITS, "Credits");
	add_entry(MNID_QUITMAINMENU, "Quit Game");
}

void MainMenu::item_do_menu_action(MenuItem& item)
{
	switch (item.get_id()) {
		case MNID_STARTGAME:
			ScreenManager::current()->push_screen(
				std::make_unique<GameSession>("levels/level-0/level-0-level.txt"),
				std::make_unique<ScreenFade>(
					Vector(static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT)) / 2.0f, 
					0.5f
				)
			);
			MenuManager::current()->clear_menu_stack();
			break;

		case MNID_CREDITS:
			ScreenManager::current()->push_screen(
				std::make_unique<CreditScreen>("credits.txt"),
				std::make_unique<ScreenFade>(
					Vector(static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT)) / 2.0f, 
					0.5f
				)
			);
			MenuManager::current()->clear_menu_stack();
			break;

		case MNID_QUITMAINMENU:
			MenuManager::current()->clear_menu_stack();
			ScreenManager::current()->quit();
			break;
	}
}

bool MainMenu::on_back_action() { return false; }