#ifndef HEADER_QBSKR_QBSKR_MENU_GAME_MENU_HPP
#define HEADER_QBSKR_QBSKR_MENU_GAME_MENU_HPP

#include "gui/menu.hpp"

enum GameMenuIDs {
	MNID_CONTINUE,
	MNID_ABORTLEVEL
};

class GameMenu final : public Menu {
public:
	GameMenu();

public:
	void item_do_menu_action(MenuItem& item) override;

private:
	GameMenu(const GameMenu&) = delete;
	GameMenu& operator=(const GameMenu&) = delete;
};

#endif