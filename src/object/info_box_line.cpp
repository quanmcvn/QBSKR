#include "object/info_box_line.hpp"

#include "qbskr/color_scheme.hpp"
#include "qbskr/resources.hpp"
#include "video/canvas.hpp"
#include "video/surface.hpp"

namespace {
	InfoBoxLine::LineType line_type_by_format_char(char format_char) 
	{
		switch (format_char)
		{
			case ' ':
				return InfoBoxLine::SMALL;
			case '\t':
				return InfoBoxLine::NORMAL;
			case '-':
				return InfoBoxLine::HEADING;
			case '!':
				return InfoBoxLine::IMAGE;
			default:
				return InfoBoxLine::SMALL;
		}
	}

	FontPtr font_by_format_char(char format_char) 
	{
		switch (format_char) {
			case ' ':
				return Resources::small_font;
			case '-':
				return Resources::big_font;
			case '\t':
			case '!':
				return Resources::normal_font;
			default:
				return Resources::normal_font;
		}
	}

	Color color_by_format_char(char format_char) 
	{
		switch (format_char) {
			case ' ':
				return ColorScheme::Text::small_color;
			case '-':
				return ColorScheme::Text::heading_color;
			case '\t':
			case '!':
				return ColorScheme::Text::normal_color;
			default:
				return ColorScheme::Text::normal_color;
		}
	}
}

InfoBoxLine::InfoBoxLine(char format_char, const std::string& text) :
	m_line_type(line_type_by_format_char(format_char)),
	m_font(font_by_format_char(format_char)),
	m_color(color_by_format_char(format_char)),
	m_text(text),
	m_image()
{
	if (m_line_type == InfoBoxLine::IMAGE) {
		m_image = Surface::from_file(text);
	}
}

void InfoBoxLine::draw(Canvas& canvas, const Rectf& bounding_box, int layer)
{
	switch (m_line_type) {
		case IMAGE:
			canvas.draw_surface(
				m_image, 
				Vector(
					(bounding_box.get_middle().x) - static_cast<float>(m_image->get_width()) / 2,
					bounding_box.get_top()
				),
				layer
			);
			break;
		
		default:
			canvas.draw_text(
				m_font, m_text,
				Vector(
					bounding_box.get_middle().x,
					bounding_box.get_top()
				),
				ALIGN_CENTER, layer, m_color
			);
			break;
	}
}

float InfoBoxLine::get_height() const
{
	const float ITEM_SPACE = 4;
	float ret;
	switch (m_line_type) {
		case IMAGE:
			ret = static_cast<float>(m_image->get_height());
			break;

		default:
			ret = m_font->get_height();
			break;
	}
	ret += ITEM_SPACE;
	return ret;
}