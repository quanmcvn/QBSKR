#include "gui/menu_item.hpp"

#include "qbskr/color_scheme.hpp"
#include "qbskr/resources.hpp"
#include "video/drawing_context.hpp"

MenuItem::~MenuItem()
{}

MenuItem::MenuItem(const std::string& text, int id) : 
	m_id(id),
	m_text(text)
{}

int MenuItem::get_id() const { return m_id; }

void MenuItem::set_text(const std::string& text) { m_text = text; }
const std::string& MenuItem::get_text() const { return m_text; }

void MenuItem::draw(DrawingContext& drawing_context, const Vector& pos, int menu_width, bool active)
{
	drawing_context.get_canvas().draw_text(
		Resources::normal_font, m_text,
		Vector(
			pos.x + static_cast<float>(menu_width) / 2.0f,
			pos.y - static_cast<float>(Resources::normal_font->get_height()) / 2.0f
		),
		ALIGN_CENTER, LAYER_GUI,
		active ? ColorScheme::Menu::active_color : get_color()
	);
}

bool MenuItem::is_selectable() const { return true; }

int MenuItem::get_width() const
{
	return static_cast<int>(Resources::normal_font->get_text_width(m_text)) + 8;
}

int MenuItem::get_height() const { return 12; }

void MenuItem::process_action(const MenuAction& /* action */) {}

void MenuItem::process_event(const SDL_Event& /* event */) {}

Color MenuItem::get_color() const { return ColorScheme::Menu::default_color; }