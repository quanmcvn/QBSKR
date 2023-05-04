#include "qbskr/savegame.hpp"

#include "object/player.hpp"
#include "qbskr/player_status.hpp"

Savegame::Savegame() :
	m_player_status()
{
	m_player_status = std::make_unique<PlayerStatus>();
}

void Savegame::reset_player_status()
{
	m_player_status->reset();
}

void Savegame::update_player_status(const Player& player)
{
	m_player_status->set_player(player);
}

PlayerStatus& Savegame::get_status()
{
	return *m_player_status;
}