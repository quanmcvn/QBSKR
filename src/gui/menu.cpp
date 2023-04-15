#include "gui/menu.hpp"

#include <SDL.h>

#include "control/controller.hpp"
#include "gui/item_action.hpp"
#include "gui/item_horizontal_line.hpp"
#include "gui/item_label.hpp"
#include "gui/menu_manager.hpp"
#include "gui/menu_item.hpp"
#include "gui/mouse_cursor.hpp"
#include "math/rectf.hpp"
#include "math/util.hpp"
#include "qbskr/globals.hpp"
#include "video/drawing_context.hpp"
#include "video/video_system.hpp"

Menu::Menu() :
	m_center_pos(Vector(static_cast<float>(SCREEN_WIDTH) / 2.0f, static_cast<float>(SCREEN_HEIGHT) / 2.0f)),
	m_items(),
	m_active_item(-1)
{}

Menu::~Menu()
{}

bool Menu::on_back_action() { return true; }

void Menu::process_event(const SDL_Event& event)
{
	m_items[m_active_item]->process_event(event);
	switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {
				Vector mouse_pos = VideoSystem::current()->get_viewport().to_logical(event.motion.x, event.motion.y);
				if (get_menu_rect().contains(mouse_pos)) {
					process_action(MenuAction::HIT);
				}
			}
			break;

		case SDL_MOUSEMOTION:
			{
				Vector mouse_pos = VideoSystem::current()->get_viewport().to_logical(event.motion.x, event.motion.y);
				if (get_menu_rect().contains(mouse_pos)) {
					int new_active_item = 0;
					float item_y = get_menu_rect().get_top();
					for (size_t i = 0; i < m_items.size(); ++i) {
						float item_height = static_cast<float>(m_items[i]->get_height());
						if (item_y <= mouse_pos.y && mouse_pos.y <= item_y + item_height) {
							new_active_item = i;
							break;
						}
						item_y += item_height;
					}

					// only change to a selectable item
					if (m_items[new_active_item]->is_selectable() && new_active_item != m_active_item) {
						if (m_active_item != -1) {
							process_action(MenuAction::UNSELECT);
						}
						m_active_item = new_active_item;
						process_action(MenuAction::SELECT);
					}

					MouseCursor::current()->set_state(MouseCursorState::LINK);
				} else {
					MouseCursor::current()->set_state(MouseCursorState::NORMAL);
				}
				break;
			}

		default:
			break;
	}
}

void Menu::draw(DrawingContext& drawing_context)
{
	float y_pos = get_menu_rect().get_top();
	for (size_t i = 0; i < m_items.size(); ++i) {
		draw_item(drawing_context, i, y_pos + static_cast<float>(m_items[i]->get_height()) / 2.0f);
		y_pos += static_cast<float>(m_items[i]->get_height());
	}
}

void Menu::draw_item(DrawingContext& drawing_context, int index, int y_pos)
{
	const float x_pos = get_menu_rect().get_left();
	m_items[index]->draw(drawing_context, Vector(x_pos, y_pos), static_cast<int>(get_width()), m_active_item == index);

	if (m_active_item == index) {
		float blink = (math::sin_degree(g_real_time * 180.0f) / 2.0f + 0.5f) * 0.5f + 0.25f;
		const float DENT = 2.5;
		const float BLINK_DENT = 1.0f;
		drawing_context.get_canvas().draw_filled_rect(
			Rectf(
				Vector(get_menu_rect().get_left()  + BLINK_DENT, y_pos - static_cast<float>(m_items[index]->get_height()) / 2),
				Vector(get_menu_rect().get_right() - BLINK_DENT, y_pos + static_cast<float>(m_items[index]->get_height()) / 2)
			),
			Color(1.0f, 1.0f, 1.0f, blink), LAYER_GUI - 10
		);
		drawing_context.get_canvas().draw_filled_rect(
			Rectf(
				Vector(get_menu_rect().get_left()  + DENT, y_pos - static_cast<float>(m_items[index]->get_height()) / 2 + (DENT - BLINK_DENT)),
				Vector(get_menu_rect().get_right() - DENT, y_pos + static_cast<float>(m_items[index]->get_height()) / 2 - (DENT - BLINK_DENT))
			),
			Color(1.0f, 1.0f, 1.0f, 0.5f), LAYER_GUI - 10
		);
	}
}

Vector Menu::get_center_pos() const { return m_center_pos; }
void Menu::set_center_pos(float x, float y) { m_center_pos = Vector(x, y); }

float Menu::get_width() const { return m_menu_width + 24; }
float Menu::get_height() const { return m_menu_height; }

Rectf Menu::get_menu_rect() const
{
	return Rectf(
		m_center_pos.x - get_width() / 2.0f,
		m_center_pos.y - get_height() / 2.0f,
		m_center_pos.x + get_width() / 2.0f,
		m_center_pos.y + get_height() / 2.0f
	);
}

MenuItem& Menu::add_item(std::unique_ptr<MenuItem> new_item)
{
	m_items.push_back(std::move(new_item));
	MenuItem& item = *m_items.back();

	// if a new menu is being built, 
	// the active item shouldn be set to something that is selectable
	if (m_active_item == -1 && item.is_selectable()) {
		m_active_item = static_cast<int>(m_items.size()) - 1;
	}

	calculate_width();
	calculate_height();

	return item;
}

ItemHorizontalLine& Menu::add_horizontal_line()
{
	auto item = std::make_unique<ItemHorizontalLine>();
	auto& item_ref = *item;
	add_item(std::move(item));
	return item_ref;
}

ItemAction& Menu::add_entry(int id, const std::string& text)
{
	auto item = std::make_unique<ItemAction>(text, id);
	auto& item_ref = *item;
	add_item(std::move(item));
	return item_ref;
}

ItemLabel& Menu::add_label(const std::string& text)
{
	auto item = std::make_unique<ItemLabel>(text);
	auto& item_ref = *item;
	add_item(std::move(item));
	return item_ref;
}

void Menu::calculate_width()
{
	// width = max width of every item
	float max_width = 0;
	for (const auto& item : m_items) {
		float w = static_cast<float>(item->get_width());
		max_width = std::max(max_width, w);
	}
	m_menu_width = max_width;
}

void Menu::calculate_height()
{
	// height = sum height of every item
	float height = 0;
	for (const auto& item : m_items) {
		height += static_cast<float>(item->get_height());
	}
	m_menu_height = height;
}

void Menu::process_input(const Controller& controller)
{
	MenuAction menu_action = MenuAction::NONE;

	if (controller.pressed(Control::UP)) {
		menu_action = MenuAction::UP;
	}
	if (controller.pressed(Control::DOWN)) {
		menu_action = MenuAction::DOWN;
	}
	if (controller.pressed(Control::LEFT)) {
		menu_action = MenuAction::LEFT;
	}
	if (controller.pressed(Control::RIGHT)) {
		menu_action = MenuAction::RIGHT;
	}
	if (controller.pressed(Control::MENU_SELECT)) {
		menu_action = MenuAction::HIT;
	}
	if (controller.pressed(Control::ESCAPE)) {
		menu_action = MenuAction::BACK;
	}

	if (m_items.empty()) return;

	process_action(menu_action);
}

void Menu::process_action(const MenuAction& menu_action)
{
	const int last_active_item = m_active_item;

	switch (menu_action) {
		case MenuAction::UP:
			do {
				if (m_active_item > 0) --m_active_item;
				else m_active_item = static_cast<int>(m_items.size()) - 1;
			} while (!m_items[m_active_item]->is_selectable()
				&& (m_active_item != last_active_item));
			break;

		case MenuAction::DOWN:
			do {
				if (m_active_item < static_cast<int>(m_items.size()) - 1) ++m_active_item;
				else m_active_item = 0;
			} while (!m_items[m_active_item]->is_selectable()
				&& (m_active_item != last_active_item));
			break;

		case MenuAction::BACK:
			if (on_back_action()) {
				MenuManager::current()->pop_menu();
			}
			return;

		default:
			break;
	}

	if (last_active_item != m_active_item) {
		if (last_active_item != -1) {
			m_items[last_active_item]->process_action(MenuAction::UNSELECT);
		}
		m_items[m_active_item]->process_action(MenuAction::SELECT);
	}

	m_items[m_active_item]->process_action(menu_action);

	// in case pop_menu() was called
	if (MenuManager::current()->current_menu() != this) return;

	if (menu_action == MenuAction::HIT) item_do_menu_action(*m_items[m_active_item]);
}