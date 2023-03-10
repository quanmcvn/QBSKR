#include "math/sizef.hpp"

#include <ostream>

#include "math/size.hpp"

Sizef::Sizef() :
	width(0.0f),
	height(0.0f)
{}

Sizef::Sizef(const Vector& v) :
	width(v.x),
	height(v.y)
{}

Sizef::Sizef(float width_, float height_) :
	width(width_),
	height(height_)
{}

Sizef::Sizef(const Size& rhs) :
	width(static_cast<float>(rhs.width)),
	height(static_cast<float>(rhs.height))
{}

Sizef& Sizef::operator*=(float factor)
{
	width *= factor;
	height *= factor;
	return *this;
}

Sizef& Sizef::operator/=(float divisor)
{
	width /= divisor;
	height /= divisor;
	return *this;
}

Sizef& Sizef::operator+=(const Sizef& rhs)
{
	width += rhs.width;
	height += rhs.height;
	return *this;
}

Sizef& Sizef::operator-=(const Sizef& rhs)
{
	width -= rhs.width;
	height -= rhs.height;
	return *this;
}

Vector Sizef::as_vector() const
{
	return Vector(width, height);
}

bool Sizef::is_valid() const
{
	return width > 0 && height > 0;
}

std::ostream& operator<<(std::ostream& s, const Sizef& size)
{
	return s << "Sizef(" << size.width << ", " << size.height << ")";
}