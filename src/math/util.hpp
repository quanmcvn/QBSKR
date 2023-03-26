#ifndef HEADER_QBSKR_MATH_UTIL_HPP
#define HEADER_QBSKR_MATH_UTIL_HPP

#include <cmath>

class Vector;

namespace math {
	float degree_to_radian(float x);
	float radian_to_degree(float x);
	float sin_degree(float x);
	float cos_degree(float x);
	// angle of vector with respect to Ox
	float angle(const Vector& vector);
} // namespace math

#endif