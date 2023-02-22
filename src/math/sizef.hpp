#ifndef HEADER_QBSKR_MATH_SIZEF_HPP
#define HEADER_QBSKR_MATH_SIZEF_HPP

#include <iosfwd>
#include "math/vector.hpp"

class Size;

class Sizef final {
public:
	float width;
	float height;
public:
	Sizef():
		width(0.0f),
		height(0.0f) {}

	Sizef(const Sizef& rhs) = default;
	Sizef& operator=(const Sizef& rhs) = default;

	explicit Sizef(const Vector& v):
		width(v.x),
		height(v.y) {}

	Sizef(float width_, float height_):
		width(width_),
		height(height_) {}

	Sizef(const Size& rhs);

	Sizef& operator*=(float factor) {
		width *= factor;
		height *= factor;
		return *this;
	}

	Sizef& operator/=(float divisor) {
		width /= divisor;
		height /= divisor;
		return *this;
	}

	Sizef& operator+=(const Sizef& rhs) {
		width += rhs.width;
		height += rhs.height;
		return *this;
	}

	Sizef& operator-=(const Sizef& rhs) {
		width -= rhs.width;
		height -= rhs.height;
		return *this;
	}

	Vector as_vector() const {
		return Vector(width, height);
	}

	bool is_valid() const {
		return width > 0 && height > 0;
	}
};

inline Sizef operator*(const Sizef& lhs, float factor) {
	return Sizef(lhs.width * factor,
		lhs.height * factor);
}

inline Sizef operator*(float factor, const Sizef& rhs) {
	return Sizef(rhs.width * factor,
	             rhs.height * factor);
}

inline Sizef operator/(const Sizef& lhs, float divisor) {
	return Sizef(lhs.width / divisor,
	             lhs.height / divisor);
}

inline Sizef operator+(const Sizef& lhs, const Sizef& rhs) {
	return Sizef(lhs.width + rhs.width,
	             lhs.height + rhs.height);
}

inline Sizef operator-(const Sizef& lhs, const Sizef& rhs) {
	return Sizef(lhs.width - rhs.width,
	             lhs.height - rhs.height);
}

inline bool operator==(const Sizef& lhs, const Sizef& rhs) {
	return (lhs.width == rhs.width) && (rhs.height == rhs.height);
}

inline bool operator!=(const Sizef& lhs, const Sizef& rhs) {
	return (lhs.width != rhs.width) || (lhs.height != rhs.height);
}

std::ostream& operator<<(std::ostream& s, const Sizef& size);

#endif