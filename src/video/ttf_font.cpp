#include "video/ttf_font.hpp"

#include <sstream>

#include "util/line_iterator.hpp"
#include "util/log.hpp"
#include "video/canvas.hpp"
#include "video/ttf_surface.hpp"
#include "video/ttf_surface_manager.hpp"

TTFFont::~TTFFont()
{
	TTF_CloseFont(m_font);
}

TTFFont::TTFFont(const std::string& filename, int font_size, float line_spacing) :
	m_font(),
	m_filename(filename),
	m_font_size(font_size),
	m_line_spacing(line_spacing)
{
	m_font = TTF_OpenFont(m_filename.c_str(), m_font_size);
	if (!m_font) {
		std::ostringstream msg;
		msg << "Couldn't load TTFFont: " << m_filename << ": " << SDL_GetError();
		throw std::runtime_error(msg.str());
	}
}

TTF_Font* TTFFont::get_ttf_font() const { return m_font; }
int TTFFont::get_font_size() const { return m_font_size; }
float TTFFont::get_height() const { return static_cast<float>(m_font_size) * m_line_spacing; }

float TTFFont::get_text_width(const std::string& text) const
{
	if (text.empty()) return 0.0f;

	float max_width = 0.0f;

	LineIterator it(text);
	while (it.next()) {
		const std::string& line = it.get();
		int w;
		int h;
		int ret = TTF_SizeUTF8(m_font, line.c_str(), &w, &h);
		if (ret < 0) {
			log_warning << "TTFFont::get_text_width(): " << TTF_GetError() << std::endl;
		}
		max_width = std::max(max_width, static_cast<float>(w));
	}

	return max_width;
}

void TTFFont::draw_text(Canvas& canvas, const std::string& text,
                        const Vector& pos, FontAlignment alignment, int layer, const Color& color)
{
	// - (static_cast<float>(TTF_FontHeight(m_font)) - get_height()) / 2.0f to balance
	float last_y = pos.y - (static_cast<float>(TTF_FontHeight(m_font)) - get_height()) / 2.0f;

	LineIterator it(text);
	while (it.next()) {
		const std::string& line = it.get();

		if (!line.empty()) {
			TTFSurfacePtr ttf_surface = TTFSurfaceManager::current()->get_surface(*this, text);

			Vector new_pos(pos.x, last_y);

			if (alignment == ALIGN_CENTER) {
				new_pos.x -= static_cast<float>(ttf_surface->get_width()) / 2.0f;
			} else if (alignment == ALIGN_RIGHT) {
				new_pos.x -= static_cast<float>(ttf_surface->get_width());
			}

			// real draw text
			canvas.draw_surface(ttf_surface->get_surface(), new_pos, 0.0f, color, layer);
		}

		last_y += get_height();
	}
}