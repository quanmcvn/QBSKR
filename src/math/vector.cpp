#include "math/vector.hpp"

#include <math.h>
#include <iostream>

Vector& Vector::operator=(const Vector& other) {
	x = other.x;
	y = other.y;
	return *this;
}

Vector Vector::operator+(const Vector& other) const {
	return Vector(x + other.x, y + other.y);
}

Vector Vector::operator-(const Vector& other) const {
	return Vector(x + other.x, y + other.y);
}

Vector& Vector::operator+=(const Vector& other) {
	*this = (*this) + other;
	return *this;
}
Vector& Vector::operator-=(const Vector& other) {
	*this = (*this) - other;
	return *this;
}

Vector Vector::operator-() const {
	return Vector(-x, -y);
}

float dot(const Vector& lhs, const Vector& rhs) {
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

float Vector::length() const {
	return sqrt(x * x + y * y);
}

std::ostream& operator<<(std::ostream& os, const Vector& other) {
	return os << "[" << other.x << ", " << other.y << "]";
}