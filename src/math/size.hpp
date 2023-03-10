#ifndef HEADER_QBSKR_MATH_SIZE_HPP
#define HEADER_QBSKR_MATH_SIZE_HPP

#include <iosfwd>

class Sizef;

class Size final {
public:
	int width;
	int height;
public:
	Size();

	Size(const Size& rhs) = default;
	Size& operator=(const Size& rhs) = default;

	explicit Size(const Sizef& rhs);

	Size(int width_, int height_);

	Size& operator*=(int factor);
	Size& operator/=(int divisor);
	Size& operator+=(const Size& rhs);
	Size& operator-=(const Size& rhs);

	bool is_valid() const;
};

inline Size operator*(const Size& lhs, int factor)
{
	return Size(lhs.width * factor, lhs.height * factor);
}

inline Size operator*(int factor, const Size& rhs)
{
	return Size(rhs.width * factor, rhs.height * factor);
}

inline Size operator/(const Size& lhs, int divisor)
{
	return Size(lhs.width / divisor, lhs.height / divisor);
}

inline Size operator+(const Size& lhs, const Size& rhs)
{
	return Size(lhs.width + rhs.width, lhs.height + rhs.height);
}

inline Size operator-(const Size& lhs, const Size& rhs)
{
	return Size(lhs.width - rhs.width, lhs.height - rhs.height);
}

inline bool operator==(const Size& lhs, const Size& rhs)
{
	return (lhs.width == rhs.width) && (lhs.height == rhs.height);
}

inline bool operator!=(const Size& lhs, const Size& rhs)
{
	return (lhs.width != rhs.width) || (lhs.height != rhs.height);
}

std::ostream& operator<<(std::ostream& s, const Size& size);

#endif