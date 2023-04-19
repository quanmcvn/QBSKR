#ifndef HEADER_QBSKR_VIDEO_COLOR_HPP
#define HEADER_QBSKR_VIDEO_COLOR_HPP

#include <stdint.h>

/**
 *  Class to store Color (sure)
*/
class Color final {
public:
	Color();

public:
	static const Color BLACK;
	static const Color RED;
	static const Color WHITE;

public:
	float red, green, blue, alpha;

public:
	static Color from_rgb(uint8_t red_, uint8_t green_, uint8_t blue_);

public:
	Color(float red_, float green_, float blue_, float alpha_ = 1.0f);

	uint8_t r8() const;
	uint8_t g8() const;
	uint8_t b8() const;
	uint8_t a8() const;

	Color operator+(const Color& other) const;
	Color operator-(const Color& other) const;
	Color operator*(const Color& other) const;
	Color operator/(const Color& other) const;

	Color operator*(float m) const;
	Color operator/(float d) const;
};

#endif