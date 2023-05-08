#ifndef HEADER_QBSKR_GUI_ITEM_STRING_SELECT_HPP
#define HEADER_QBSKR_GUI_ITEM_STRING_SELECT_HPP

#include "gui/menu_item.hpp"

class ItemStringSelect final : public MenuItem {
private:
	ItemStringSelect(const ItemStringSelect&) = delete;
	ItemStringSelect& operator=(const ItemStringSelect&) = delete;

private:
	// list of item
	std::vector<std::string> m_items;
	// currently selected item
	int& m_selected;
	float m_width;

public:
	ItemStringSelect(const std::string& text, std::vector<std::string> items, int& selected, int id = -1);

public:
	virtual void draw(DrawingContext& drawing_context, const Vector& pos, int menu_width, bool active) override;

	virtual int get_width() const override;

	virtual void process_action(const MenuAction& action) override;

private:
	float calculate_width() const;
};

#endif