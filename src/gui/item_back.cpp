#include "gui/item_back.hpp"

#include "gui/menu_manager.hpp"

ItemBack::ItemBack(const std::string& text, int id) :
	MenuItem(text, id)
{}

void ItemBack::process_action(const MenuAction& action) {
	if (action == MenuAction::HIT) {
		if (MenuManager::current()->current_menu()->on_back_action()) {
			MenuManager::current()->pop_menu();
		}
	}
}