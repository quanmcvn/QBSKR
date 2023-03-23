#ifndef HEADER_QBSKR_MATH_VECTOR_HPP
#define HEADER_QBSKR_MATH_VECTOR_HPP

#include <iosfwd>

class Vector {
public:
	float x, y;
public:
	Vector();
	Vector(const Vector& other);

	Vector& operator=(const Vector& other);

public:
	Vector(float x_, float y_);

public:
	Vector operator+(const Vector& other) const;
	Vector operator-(const Vector& other) const;
	Vector operator*(float factor) const;
	Vector operator/(float factor) const;
	Vector& operator+=(const Vector& other);
	Vector& operator-=(const Vector& other);
	Vector& operator*=(float factor);

	bool operator==(const Vector& other) const;

	Vector operator-() const;

	friend std::ostream& operator<<(std::ostream& os, const Vector& other);
};

namespace math {

	float length(const Vector& vector);
	Vector normalize(const Vector& vector);
	Vector rotate(const Vector& vector, float angle);
	float dot(const Vector& lhs, const Vector& rhs);
	float distance(const Vector& lhs, const Vector& rhs);

} // namespace math

#endif