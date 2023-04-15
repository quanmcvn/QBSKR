#ifndef HEADER_QBSKR_QBSKR_SCREEN_FADE_HPP
#define HEADER_QBSKR_QBSKR_SCREEN_FADE_HPP

#include "math/vector.hpp"

class DrawingContext;

/**
 *  Class to be displayed with another screen
 *  Shrinks a rectangle screen towards a specific position
*/
class ScreenFade final {
private:
	ScreenFade(const ScreenFade&) = delete;
	ScreenFade& operator=(const ScreenFade&) = delete;

private:
	Vector m_dest;
	float m_fade_time;
	float m_sum_time;
	float m_begin_size;

public:
	ScreenFade(const Vector& dest, float fade_time);

public:
	void update(float dt_sec);
	void draw(DrawingContext& drawing_context);
	bool done() const;
};

#endif