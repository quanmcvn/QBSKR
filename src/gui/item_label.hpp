#ifndef HEADER_QBSKR_GUI_ITEM_LABEL_HPP
#define HEADER_QBSKR_GUI_ITEM_LABEL_HPP

#include "gui/menu_item.hpp"

class ItemLabel final : public MenuItem {
private:
	ItemLabel(const ItemLabel&) = delete;
	ItemLabel& operator=(const ItemLabel&) = delete;

public:
	ItemLabel(const std::string& text_);

public:
	virtual void draw(DrawingContext& drawing_context, const Vector& pos, int menu_width, bool active) override;

	virtual bool is_selectable() const override;

	virtual int get_width() const override;

	virtual Color get_color() const override;

};

#endif