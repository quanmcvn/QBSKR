#ifndef HEADER_QBSKR_GUI_ITEM_GOTO_HPP
#define HEADER_QBSKR_GUI_ITEM_GOTO_HPP

#include "gui/menu_item.hpp"

class ItemGoTo final : public MenuItem {
private:
	ItemGoTo(const ItemGoTo&) = delete;
	ItemGoTo& operator=(const ItemGoTo&) = delete;

private:
	int target_menu;

public:
	ItemGoTo(const std::string& text, int target_menu_, int id = -1);

	virtual void process_action(const MenuAction& action) override;
};

#endif