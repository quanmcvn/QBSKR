#ifndef HEADER_QBSKR_QBSKR_TITLE_SCREEN_HPP
#define HEADER_QBSKR_QBSKR_TITLE_SCREEN_HPP

#include "qbskr/screen.hpp"

/**
 *  The title screen
 *  Contains nothing (?)
*/
class TitleScreen final : public Screen {
public:
	TitleScreen();
	~TitleScreen() override;

private:
	TitleScreen(const TitleScreen&) = delete;
	TitleScreen& operator=(const TitleScreen&) = delete;

public:
	virtual void setup() override;
	virtual void leave() override;

	virtual void draw(Compositor& compositor) override;
	virtual void update(float dt_sec, const Controller& controller) override;
};

#endif