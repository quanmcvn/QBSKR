#include "math/util.hpp"

constexpr float PI = 3.14159265358979323846f;

namespace math {
	float degree_to_radian(float x) { return (x / 180.0f) * PI; }
	float radian_to_degree(float x) { return (x / PI) * 180.0f; }

	// https://stackoverflow.com/a/31525208 for more information
	// although using float instead of double defeat the purpose -.-
	float sin_degree(float x)
	{
		if (x < 0.0f) return -sin_degree(-x);
		int quo;
		float x90 = std::remquof(std::fabs(x), 90.0f, &quo);
		switch (quo % 4) {
			case 0:
				// Use * 1.0 to avoid -0.0
				return std::sin(degree_to_radian(x90) * 1.0);
			case 1:
				return std::cos(degree_to_radian(x90));
			case 2:
				return std::sin(degree_to_radian(-x90) * 1.0);
			case 3:
				return -std::cos(degree_to_radian(x90));
		}
		// should never happend, added to make compiler happy
		return 0.0f;
	}

	float cos_degree(float x) 
	{
		// good enough if x is not large
		return sin_degree(90.0 - fabs(x));
	}
} // namespace math