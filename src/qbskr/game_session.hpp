#ifndef HEADER_QBSKR_QBSKR_GAME_SESSION_HPP
#define HEADER_QBSKR_QBSKR_GAME_SESSION_HPP

#include <memory>

#include "qbskr/screen.hpp"
#include "util/currenton.hpp"

class Level;

class GameSession final : public Screen, public Currenton<GameSession> {
private:
	GameSession(const GameSession&) = delete;
	GameSession& operator=(const GameSession&) = delete;

private:
	std::unique_ptr<Level> m_level;
	bool m_game_pause;
	float m_speed_before_pause;

public:
	GameSession(const std::string& level_filename);

public:
	virtual void update(float dt_sec, const Controller& controller) override;
	virtual void draw(Compositor& compositor) override;

	void toggle_pause();
	void abort_level();
};

#endif