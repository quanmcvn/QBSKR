#ifndef HEADER_QBSKR_GUI_ITEM_BACK_HPP
#define HEADER_QBSKR_GUI_ITEM_BACK_HPP

#include "gui/menu_item.hpp"

class ItemBack final : public MenuItem {
private:
	ItemBack(const ItemBack&) = delete;
	ItemBack& operator=(const ItemBack&) = delete;

public:
	ItemBack(const std::string& text, int id = -1);

public:
	virtual void process_action(const MenuAction& action) override;
};

#endif