#ifndef HEADER_QBSKR_QBSKR_WAITING_SCREEN_HPP
#define HEADER_QBSKR_QBSKR_WAITING_SCREEN_HPP

#include <functional>

#include "sprite/sprite_ptr.hpp"
#include "sprite/sprite.hpp"
#include "qbskr/screen.hpp"
#include "util/timer.hpp"

class WaitingScreen final : public Screen {
private:
	WaitingScreen(const WaitingScreen&) = delete;
	WaitingScreen& operator=(const WaitingScreen&) = delete;

private:
	std::function<void()> m_callback;
	// a little bit of trolling
	// make player feels like it's doing something :D
	Timer m_wait_timer;
	SpritePtr m_sprite;

public:
	WaitingScreen(std::function<void()> callback);

public:
	virtual void setup() override;

	virtual void update(float dt_sec, const Controller& controller) override;
	virtual void draw(Compositor& compositor) override;
};

#endif