#include "gui/item_action.hpp"

ItemAction::ItemAction(const std::string& text, int id) :
	MenuItem(text, id)
{}

void ItemAction::process_action(const MenuAction& /* menu_action */)
{}