#ifndef HEADER_QBSKR_QBSKR_SCREEN_HPP
#define HEADER_QBSKR_QBSKR_SCREEN_HPP

class Compositor;
class Controller;

class Screen {
public:
	virtual ~Screen();

	virtual void update(float dt_sec, const Controller& controller) = 0;
	virtual void draw(Compositor& compositor) = 0;
};

#endif