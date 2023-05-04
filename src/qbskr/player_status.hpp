#ifndef HEADER_QBSKR_QBSKR_PLAYER_STATUS_HPP
#define HEADER_QBSKR_QBSKR_PLAYER_STATUS_HPP

#include <memory>

class Player;

/**
 * This class keeps player status between gamesessions
 * by holding the player it self
*/
class PlayerStatus final {
public:
	PlayerStatus();

private:
	PlayerStatus(const PlayerStatus&) = delete;
	PlayerStatus& operator=(const PlayerStatus&) = delete;

private:
	std::unique_ptr<Player> m_player;

public:
	void reset();
	void set_player(const Player& player);
	std::unique_ptr<Player> get_player() const;
};

#endif