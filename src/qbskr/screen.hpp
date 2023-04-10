#ifndef HEADER_QBSKR_QBSKR_SCREEN_HPP
#define HEADER_QBSKR_QBSKR_SCREEN_HPP

class Compositor;
class Controller;

class Screen {
public:
	virtual ~Screen();

public:
	virtual void setup();
	virtual void leave();

	virtual void update(float dt_sec, const Controller& controller) = 0;
	virtual void draw(Compositor& compositor) = 0;
};

#endif