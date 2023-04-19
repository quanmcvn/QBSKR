#ifndef HEADER_QBSKR_OBJECT_INFO_BOX_LINE_HPP
#define HEADER_QBSKR_OBJECT_INFO_BOX_LINE_HPP

#include <string>

#include "math/rectf.hpp"
#include "video/color.hpp"
#include "video/font_ptr.hpp"
#include "video/surface_ptr.hpp"

class Canvas;

class InfoBoxLine final {
private:
	InfoBoxLine(const InfoBoxLine&) = delete;
	InfoBoxLine& operator=(const InfoBoxLine&) = delete;

public:
	enum LineType { NORMAL, SMALL, HEADING, IMAGE};

private:
	LineType m_line_type;
	FontPtr m_font;
	Color m_color;
	std::string m_text;
	SurfacePtr m_image;

public:
	InfoBoxLine(char format_char, const std::string& text);

public:
	// draw the box (as the name suggests)
	// catch: assume everything is at the middle of bounding_box (bounding_box.get_middle())
	// maybe changed later (or never)
	void draw(Canvas& canvas, const Rectf& bounding_box, int layer);
	float get_height() const;
};

#endif