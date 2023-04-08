#include "qbskr/game_session.hpp"

#include "control/controller.hpp"
#include "gui/menu_manager.hpp"
#include "gui/menu_set.hpp"
#include "qbskr/level_data_set.hpp"
#include "qbskr/level_data.hpp"
#include "qbskr/level.hpp"
#include "video/compositor.hpp"

GameSession::GameSession(const std::string& level_filename) :
	m_level(LevelDataSet::current()->get_level_data(level_filename).make_level())
{
	m_level->activate();
}

void GameSession::update(float dt_sec, const Controller& controller) 
{
	if (controller.pressed(Control::ESCAPE)) {
		toggle_pause();
	}

	// unpause the game if no menu is active
	if (m_game_pause && !MenuManager::current()->is_active()) {
		m_game_pause = false;
	}

	if (!m_game_pause) {
		m_level->update(dt_sec);
	}
}

void GameSession::draw(Compositor& compositor) 
{
	auto& drawing_context = compositor.make_context();

	m_level->draw(drawing_context);
}

void GameSession::toggle_pause()
{
	if (!m_game_pause && !MenuManager::current()->is_active()) {
		MenuManager::current()->set_menu(MenuSet::GAME_MENU);
		m_game_pause = true;
	}

	// unpause will be done update()
}