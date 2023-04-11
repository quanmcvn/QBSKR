#ifndef HEADER_QBSKR_GUI_ITEM_ACTION_HPP
#define HEADER_QBSKR_GUI_ITEM_ACTION_HPP

#include <functional>

#include "gui/menu_item.hpp"

/**
 *  Selectable item
*/
class ItemAction final : public MenuItem {
private:
	ItemAction(const ItemAction&) = delete;
	ItemAction& operator=(const ItemAction&) = delete;

public:
	ItemAction(const std::string& text, int id = -1);

public:
	virtual void process_action(const MenuAction& menu_action) override;
};

#endif