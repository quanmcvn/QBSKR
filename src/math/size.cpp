#include "math/size.hpp"

#include <ostream>

#include "math/sizef.hpp"


Size::Size() :
		width(0),
		height(0)
	{}

Size::Size(const Sizef& rhs) :
	width(static_cast<int>(rhs.width)),
	height(static_cast<int>(rhs.height))
{}


Size::Size(int width_, int height_) :
	width(width_),
	height(height_)
{}

Size& Size::operator*=(int factor)
{
	width *= factor;
	height *= factor;
	return *this;
}

Size& Size::operator/=(int divisor)
{
	width /= divisor;
	height /= divisor;
	return *this;
}

Size& Size::operator+=(const Size& rhs)
{
	width += rhs.width;
	height += rhs.height;
	return *this;
}

Size& Size::operator-=(const Size& rhs)
{
	width -= rhs.width;
	height -= rhs.height;
	return *this;
}

bool Size::is_valid() const
{
	return width > 0 && height > 0;
}

std::ostream& operator<<(std::ostream& s, const Size& size)
{
	return s << "Size(" << size.width << ", " << size.height << ")";
}