#ifndef HEADER_QBSKR_QBSKR_MENU_MAIN_MENU_HPP
#define HEADER_QBSKR_QBSKR_MENU_MAIN_MENU_HPP

#include "gui/menu.hpp"

/**
 *  Title screen menu
*/
class MainMenu final : public Menu {
public:
	MainMenu();

public:
	void item_do_menu_action(MenuItem& item) override;
	bool on_back_action() override;
};

#endif