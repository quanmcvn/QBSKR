#ifndef HEADER_QBSKR_QBSKR_GAME_MANAGER_HPP
#define HEADER_QBSKR_QBSKR_GAME_MANAGER_HPP

#include <memory>

#include "util/currenton.hpp"

class Savegame;

class GameManager final : public Currenton<GameManager> {
public:
	GameManager();
	~GameManager() override;

private:
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;

private:
	std::unique_ptr<Savegame> m_savegame;

public:
	void start_level(const std::string& level_filename, bool wait = false);
};

#endif