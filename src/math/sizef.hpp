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
	Sizef();

	Sizef(const Sizef& rhs) = default;
	Sizef& operator=(const Sizef& rhs) = default;

	explicit Sizef(const Vector& v);

	Sizef(float width_, float height_);
	Sizef(const Size& rhs);

	Sizef& operator*=(float factor);
	Sizef& operator/=(float divisor);
	Sizef& operator+=(const Sizef& rhs);
	Sizef& operator-=(const Sizef& rhs);

	Vector as_vector() const;
	bool is_valid() const;
};

inline Sizef operator*(const Sizef& lhs, float factor)
{
	return Sizef(lhs.width * factor, lhs.height * factor);
}

inline Sizef operator*(float factor, const Sizef& rhs)
{
	return Sizef(rhs.width * factor, rhs.height * factor);
}

inline Sizef operator/(const Sizef& lhs, float divisor)
{
	return Sizef(lhs.width / divisor, lhs.height / divisor);
}

inline Sizef operator+(const Sizef& lhs, const Sizef& rhs)
{
	return Sizef(lhs.width + rhs.width, lhs.height + rhs.height);
}

inline Sizef operator-(const Sizef& lhs, const Sizef& rhs)
{
	return Sizef(lhs.width - rhs.width, lhs.height - rhs.height);
}

inline bool operator==(const Sizef& lhs, const Sizef& rhs)
{
	return (lhs.width == rhs.width) && (rhs.height == rhs.height);
}

inline bool operator!=(const Sizef& lhs, const Sizef& rhs)
{
	return (lhs.width != rhs.width) || (lhs.height != rhs.height);
}

std::ostream& operator<<(std::ostream& s, const Sizef& size);

#endif