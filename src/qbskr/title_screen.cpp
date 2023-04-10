#include "qbskr/title_screen.hpp"

#include "control/controller.hpp"
#include "gui/menu_manager.hpp"
#include "gui/menu_set.hpp"
#include "video/compositor.hpp"

TitleScreen::TitleScreen() {}

TitleScreen::~TitleScreen() {}

void TitleScreen::setup()
{
	MenuManager::current()->set_menu(MenuSet::MAIN_MENU);
}

void TitleScreen::leave()
{
	MenuManager::current()->clear_menu_stack();
}

void TitleScreen::draw(Compositor& /* compositor */)
{
	// NYI
}

void TitleScreen::update(float /* dt_sec */, const Controller& /* controller */)
{
	// NYI
}