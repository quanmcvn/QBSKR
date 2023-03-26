#ifndef HEADER_QBSKR_MATH_INTERPOLATE_HPP
#define HEADER_QBSKR_MATH_INTERPOLATE_HPP

namespace interpolate {
	float linear_interpolation(float t, float start = 0.0f, float end = 1.0f);
	float quadratic_ease_in(float t, float start = 0.0f, float end = 1.0f);
	float quadratic_ease_out(float t, float start = 0.0f, float end = 1.0f);
	float cubic_ease_in(float t, float start = 0.0f, float end = 1.0f);
	float cubic_ease_out(float t, float start = 0.0f, float end = 1.0f);
} // namespace interpolate

#endif