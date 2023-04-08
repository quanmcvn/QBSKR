#ifndef HEADER_QBSKR_GUI_ITEM_HORIZONTAL_LINE_HPP
#define HEADER_QBSKR_GUI_ITEM_HORIZONTAL_LINE_HPP

#include "gui/menu_item.hpp"

class ItemHorizontalLine final : public MenuItem {
public:
	ItemHorizontalLine();

private:
	ItemHorizontalLine(const ItemHorizontalLine&) = delete;
	ItemHorizontalLine& operator=(const ItemHorizontalLine&) = delete;

public:
	virtual void draw(DrawingContext& drawing_context, const Vector& pos, int menu_width, bool active) override;

	virtual bool is_selectable() const override;

	virtual int get_width() const override;
};

#endif