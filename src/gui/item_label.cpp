#include "gui/item_label.hpp"

#include "qbskr/color_scheme.hpp"
#include "qbskr/resources.hpp"
#include "video/drawing_context.hpp"
#include "video/font.hpp"

ItemLabel::ItemLabel(const std::string& text_) :
	MenuItem(text_)
{}

void ItemLabel::draw(DrawingContext& drawing_context, const Vector& pos, int menu_width, bool /* active */)
{
	drawing_context.get_canvas().draw_text(
		Resources::big_font, get_text(),
		Vector(
			pos.x + static_cast<float>(menu_width) / 2.0f,
			pos.y - Resources::big_font->get_height() / 2.0f
		),
		ALIGN_CENTER, LAYER_GUI, get_color()
	);
}


bool ItemLabel::is_selectable() const { return false; }

int ItemLabel::get_width() const
{
	return static_cast<int>(Resources::big_font->get_text_width(get_text()));
} 

Color ItemLabel::get_color() const
{
	return ColorScheme::Menu::label_color;
}