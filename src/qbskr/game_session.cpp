#include "qbskr/game_session.hpp"

#include "control/controller.hpp"
#include "gui/menu_manager.hpp"
#include "gui/menu_set.hpp"
#include "qbskr/globals.hpp"
#include "qbskr/level_data_set.hpp"
#include "qbskr/level_data.hpp"
#include "qbskr/level.hpp"
#include "qbskr/screen_fade.hpp"
#include "qbskr/screen_manager.hpp"
#include "video/compositor.hpp"
#include "video/video_system.hpp"

GameSession::GameSession(const std::string& level_filename) :
	m_level(LevelDataSet::current()->get_level_data(level_filename).make_level()),
	m_game_pause(false),
	m_speed_before_pause(ScreenManager::current()->get_speed())
{
	m_level->activate();
}

void GameSession::update(float dt_sec, const Controller& controller) 
{
	if (controller.pressed(Control::ESCAPE)) {
		on_escape_press();
	}

	// unpause the game if no menu is active
	if (m_game_pause && !MenuManager::current()->is_active()) {
		ScreenManager::current()->set_speed(m_speed_before_pause);
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

void GameSession::on_escape_press()
{
	if (ScreenManager::current()->has_pending_fade()) {
		// dont let player open menu when switching screen
		return;
	}

	toggle_pause();
}

void GameSession::toggle_pause()
{
	if (!m_game_pause && !MenuManager::current()->is_active()) {
		m_speed_before_pause = ScreenManager::current()->get_speed();
		ScreenManager::current()->set_speed(0.0f);
		MenuManager::current()->set_menu(MenuSet::GAME_MENU);
		m_game_pause = true;
	}

	// unpause will be done update()
}

void GameSession::abort_level()
{
	MenuManager::current()->clear_menu_stack();
	ScreenManager::current()->pop_screen(std::make_unique<ScreenFade>(Vector(static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT)) / 2.0f, 0.5f));
}