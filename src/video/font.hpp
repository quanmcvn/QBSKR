#ifndef HEADER_QBSKR_VIDEO_FONT_HPP
#define HEADER_QBSKR_VIDEO_FONT_HPP

#include <string>

class Canvas;
class Color;
class Vector;

enum FontAlignment {
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT,
};

/**
 * Abstract font class
 * Needed for complete-ness in Canvas
*/
class Font {
public:
	virtual ~Font();

public:
	virtual float get_height() const = 0;

	virtual void draw_text(Canvas& canvas, const std::string& text,
	                       const Vector& pos, FontAlignment alignment, int layer, const Color& color) = 0;
};

#endif