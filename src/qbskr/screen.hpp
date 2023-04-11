#ifndef HEADER_QBSKR_QBSKR_SCREEN_HPP
#define HEADER_QBSKR_QBSKR_SCREEN_HPP

class Compositor;
class Controller;

/**
 *  Virtual base class for thing that takes an entire screen
 *  (Title screen, game session, ...)
*/
class Screen {
public:
	virtual ~Screen();

public:
	// call after screen is constructed (by ScreenManager)
	virtual void setup();
	// call before screen is deconstructed (by ScreenManager)
	virtual void leave();

	virtual void update(float dt_sec, const Controller& controller) = 0;
	virtual void draw(Compositor& compositor) = 0;
};

#endif