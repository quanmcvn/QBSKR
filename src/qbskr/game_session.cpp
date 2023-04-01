#include "qbskr/game_session.hpp"

#include "qbskr/level_data_set.hpp"
#include "qbskr/level_data.hpp"
#include "qbskr/level.hpp"
#include "video/compositor.hpp"

GameSession::GameSession(const std::string& level_filename) :
	m_level(LevelDataSet::current()->get_level_data(level_filename).make_level())
{
	m_level->activate();
}

void GameSession::update(float dt_sec, const Controller& /* controller */) 
{
	if (!m_game_pause) {
		m_level->update(dt_sec);
	}
}

void GameSession::draw(Compositor& compositor) 
{
	auto& drawing_context = compositor.make_context();

	m_level->draw(drawing_context);
}
