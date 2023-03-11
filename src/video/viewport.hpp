#ifndef HEADER_QBSKR_VIDEO_VIEWPORT_HPP
#define HEADER_QBSKR_VIDEO_VIEWPORT_HPP

#include "math/rect.hpp"
#include "math/vector.hpp"

class Viewport final {
public:
	Viewport();

private:
	Rect m_rect;
	Vector m_scale;

public:
	Viewport(const Rect& rect, const Vector& scale);

public:
	// the size of viewport
	Rect get_rect() const;
	// the scale of viewport
	Vector get_scale() const;
	// the width of logical screen
	int get_screen_width() const;
	// the height of logical screen
	int get_screen_height() const;
	// the size of logical screen
	Size get_screen_size() const;
	// convert window coordinates into logical screen coordinates
	Vector to_logical(int physical_x, int physical_y) const;
};

#endif