#ifndef HEADER_QBSKR_MATH_VECTOR_HPP
#define HEADER_QBSKR_MATH_VECTOR_HPP

#include <iosfwd>

class Vector {
public:
	float x, y;
public:
	Vector():
		x(0), y(0) {}

	Vector(const Vector& other):
		x(other.x), y(other.y) {}

	Vector& operator=(const Vector& other);
public:
	Vector(float x_, float y_):
		x(x_), y(y_) {}

public:
	Vector operator+(const Vector& other) const; 
	Vector operator-(const Vector& other) const;
	Vector& operator+=(const Vector& other);
	Vector& operator-=(const Vector& other);
	
	Vector operator-() const;

	friend float dot(const Vector& lhs, const Vector& rhs); 
	float length() const;
	
	friend std::ostream& operator<<(std::ostream& os, const Vector& other);
};


#endif