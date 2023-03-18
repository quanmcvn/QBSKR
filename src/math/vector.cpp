#include "math/vector.hpp"

#include <math.h>
#include <iostream>

Vector::Vector() :
	x(0), 
	y(0)
{}

Vector::Vector(const Vector& other) :
	x(other.x), 
	y(other.y)
{}

Vector::Vector(float x_, float y_) :
	x(x_), 
	y(y_)
{}

Vector& Vector::operator=(const Vector& other)
{
	x = other.x;
	y = other.y;
	return *this;
}

Vector Vector::operator+(const Vector& other) const { return Vector(x + other.x, y + other.y); }
Vector Vector::operator-(const Vector& other) const { return Vector(x - other.x, y - other.y); }
Vector Vector::operator*(float factor) const { return Vector(x * factor, y * factor); }
Vector Vector::operator/(float factor) const { return Vector(x / factor, y / factor); }

Vector& Vector::operator+=(const Vector& other) { *this = (*this) + other; return *this; }
Vector& Vector::operator-=(const Vector& other) { *this = (*this) - other; return *this; }
Vector& Vector::operator*=(float factor) { *this = (*this) * factor; return *this; }

bool Vector::operator==(const Vector& other) const { return (x == other.x && y == other.y); }
Vector Vector::operator-() const { return Vector(-x, -y); }

std::ostream& operator<<(std::ostream& os, const Vector& other)
{
	return os << "[" << other.x << ", " << other.y << "]";
}

namespace math {
	float length(const Vector& vector)
	{
		return sqrt(vector.x * vector.x + vector.y * vector.y);
	}

	Vector normalize(const Vector& vector)
	{
		if (length(vector) == 0) return Vector(0.0f, 0.0f);
		return vector / math::length(vector);
	}

	float dot(const Vector& lhs, const Vector& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}

	float distance(const Vector& lhs, const Vector& rhs)
	{
		return sqrt((rhs.x - lhs.x) * (rhs.x - lhs.x) + (rhs.y - lhs.y) * (rhs.y - lhs.y));
	}
} // namespace math