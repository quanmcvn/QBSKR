#include "qbskr/player_status.hpp"

#include "object/player.hpp"

PlayerStatus::PlayerStatus() :
	m_player()
{
	reset();
}

void PlayerStatus::reset()
{
	m_player = std::make_unique<Player>(0, 1);
}

void PlayerStatus::set_player(const Player& player)
{
	m_player.reset();
	m_player = player.clone();
	m_player->set_pos(Vector(0.0f, 0.0f));
	m_player->set_pos(Vector(0.0f, 0.0f) - m_player->get_bounding_box().get_middle());
}

std::unique_ptr<Player> PlayerStatus::get_player() const
{
	return m_player->clone();
}