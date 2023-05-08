#include "gui/item_string_select.hpp"

#include "gui/menu_manager.hpp"
#include "qbskr/color_scheme.hpp"
#include "qbskr/resources.hpp"
#include "video/drawing_context.hpp"
#include "video/font.hpp"

ItemStringSelect::ItemStringSelect(const std::string& text, std::vector<std::string> items, int& selected, int id) :
	MenuItem(text, id),
	m_items(items),
	m_selected(selected),
	m_width(calculate_width())
{}

void ItemStringSelect::draw(DrawingContext& drawing_context, const Vector& pos, int menu_width, bool active)
{
	// draw left side
	drawing_context.get_canvas().draw_text(
		Resources::normal_font, get_text(),
		Vector(
			pos.x + 16.0f,
			pos.y - Resources::normal_font->get_height() / 2.0f
		),
		ALIGN_LEFT, LAYER_GUI, 
		active ? ColorScheme::Menu::active_color : get_color()
	);

	// draw right side
	drawing_context.get_canvas().draw_text(
		Resources::normal_font, m_items[m_selected],
		Vector(
			pos.x + static_cast<float>(menu_width) - 8.0f,
			pos.y - Resources::normal_font->get_height() / 2.0f),
			ALIGN_RIGHT, LAYER_GUI, 
			active ? ColorScheme::Menu::active_color : get_color()
	);
}


int ItemStringSelect::get_width() const { return static_cast<int>(m_width); }

void ItemStringSelect::process_action(const MenuAction& action)
{
	switch (action) {
		case MenuAction::LEFT:
			if ( m_selected > 0) {
				m_selected--;
			} else {
				m_selected = static_cast<int>(m_items.size()) - 1;
			}
			MenuManager::current()->current_menu()->item_do_menu_action(*this);
			break;
		case MenuAction::RIGHT:
		case MenuAction::HIT:
			if ( m_selected + 1 < static_cast<int>(m_items.size())) {
				m_selected++;
			} else {
				m_selected = 0;
			}
			MenuManager::current()->current_menu()->item_do_menu_action(*this);
		break;
		default:
		break;
	}
}

float ItemStringSelect::calculate_width() const
{
	float max_item_width = 0;
	for (auto const& item : m_items) {
		max_item_width = std::max(Resources::normal_font->get_text_width(item), max_item_width);
	}
	return Resources::normal_font->get_text_width(get_text()) + max_item_width + 32.0f;
}