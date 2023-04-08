#ifndef HEADER_QBSKR_VIDEO_TTF_FONT_HPP
#define HEADER_QBSKR_VIDEO_TTF_FONT_HPP

#include <SDL_ttf.h>
#include <string>

#include "video/font.hpp"

class TTFFont final : public Font {
public:
	~TTFFont() override;

private:
	TTFFont(const TTFFont&) = delete;
	TTFFont& operator=(const TTFFont&) = delete;

public:
	TTFFont(const std::string& filename, int size, float line_spacing = 1.0f);

private:
	TTF_Font* m_font;
	std::string m_filename;
	int m_font_size;
	float m_line_spacing;

public:
	TTF_Font* get_ttf_font() const;
	int get_font_size() const;

	virtual float get_height() const override;

	virtual float get_text_width(const std::string& text) const override;

	virtual void draw_text(Canvas& canvas, const std::string& text,
	                       const Vector& pos, FontAlignment alignment, int layer, const Color& color) override;
};

#endif