#ifndef HEADER_QBSKR_QBSKR_SAVEGAME_HPP
#define HEADER_QBSKR_QBSKR_SAVEGAME_HPP

#include <memory>

class Player;
class PlayerStatus;

class Savegame final {
public:
	Savegame();

private:
	Savegame(const Savegame&) = delete;
	Savegame& operator=(const Savegame&) = delete;

private:
	std::unique_ptr<PlayerStatus> m_player_status;

public:
	void reset_player_status();
	void update_player_status(const Player& player);
	PlayerStatus& get_status();
};

#endif