#include "gui/item_horizontal_line.hpp"

#include "qbskr/color_scheme.hpp"
#include "video/drawing_context.hpp"

ItemHorizontalLine::ItemHorizontalLine() :
	MenuItem("")
{}

void ItemHorizontalLine::draw(DrawingContext& drawing_context, const Vector& pos, int menu_width, bool /* active */)
{
	drawing_context.get_canvas().draw_filled_rect(
		Rectf(
			Vector(pos.x, pos.y - 6.0f),
			Sizef(static_cast<float>(menu_width), 4.0f)
		),
		ColorScheme::Menu::horizontal_line_color, LAYER_GUI
	);
}

bool ItemHorizontalLine::is_selectable() const { return false; }

int ItemHorizontalLine::get_width() const { return 0; }