#include "qbskr/game_manager.hpp"

#include "object/player.hpp"
#include "qbskr/game_session.hpp"
#include "qbskr/globals.hpp"
#include "qbskr/player_status.hpp"
#include "qbskr/savegame.hpp"
#include "qbskr/screen_manager.hpp"
#include "qbskr/screen_fade.hpp"
#include "qbskr/waiting_screen.hpp"
#include "video/video_system.hpp"

GameManager::GameManager() :
	m_savegame()
{
	m_savegame = std::make_unique<Savegame>();
}

GameManager::~GameManager()
{
	m_savegame.reset();
}

void GameManager::start_level(const std::string& level_filename, bool wait)
{
	if (!wait) {
		ScreenManager::current()->push_screen(
			std::make_unique<GameSession>(level_filename, *m_savegame),
			std::make_unique<ScreenFade>(Vector(static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT)) / 2.0f, 0.5f)
		);
	} else {
		std::string level_filename_ = level_filename;
		ScreenManager::current()->push_screen(
			std::make_unique<WaitingScreen>([level_filename_, this] () {
				ScreenManager::current()->push_screen(
					std::make_unique<GameSession>(level_filename_, *m_savegame),
					std::make_unique<ScreenFade>(Vector(static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT)) / 2.0f, 0.5f)
				);
			}),
			std::make_unique<ScreenFade>(Vector(static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT)) / 2.0f, 0.5f)
		);
	}
}