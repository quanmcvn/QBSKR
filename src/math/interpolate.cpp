#include "math/interpolate.hpp"

namespace interpolate {
	// line y = x
	float linear_interpolation(float t, float start, float end)
	{
		return start + (end - start) * t;
	}

	// parabola y = x^2
	float quadratic_ease_in(float t, float start, float end)
	{
		return start + (end - start) * t * t;
	}

	// parabola y = -(x - 1)^2 + 1 = -x^2 + 2x = -x(x - 2)
	float quadratic_ease_out(float t, float start, float end)
	{
		return start + (end - start) * (-t * (t - 2));
	}

	// cubic y = x^3
	float cubic_ease_in(float t, float start, float end)
	{
		return start + (end - start) * (t * t * t);
	}

	// cubic y = (x - 1)^3 + 1
	float cubic_ease_out(float t, float start, float end)
	{
		t -= 1;
		return start + (end - start) * (t * t * t + 1);
	}
} // namespace interpolate
