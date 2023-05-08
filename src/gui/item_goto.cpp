#include "gui/item_goto.hpp"

#include "gui/menu_manager.hpp"

ItemGoTo::ItemGoTo(const std::string& text, int target_menu_, int id) :
	MenuItem(text, id),
	target_menu(target_menu_)
{}

void ItemGoTo::process_action(const MenuAction& action)
{
	if (action == MenuAction::HIT) {
		assert(target_menu != 0);
		MenuManager::current()->push_menu(target_menu);
	}
}