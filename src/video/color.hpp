#ifndef HEADER_QBSKR_VIDEO_COLOR_HPP
#define HEADER_QBSKR_VIDEO_COLOR_HPP

#include <stdint.h>

class Color final {
public:
	Color();

public:
	float red, green, blue, alpha;

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