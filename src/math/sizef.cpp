#include "math/sizef.hpp"

#include <ostream>

#include "math/size.hpp"

Sizef::Sizef(const Size& rhs):
	width(static_cast<float>(rhs.width)),
	height(static_cast<float>(rhs.height)) {}

std::ostream& operator<<(std::ostream& s, const Sizef& size) {
	return s << "Sizef(" << size.width << ", " << size.height << ")";
}