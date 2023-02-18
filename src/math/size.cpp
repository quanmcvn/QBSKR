#include "math/size.hpp"

#include <ostream>

#include "math/sizef.hpp"

Size::Size(const Sizef& rhs):
	width(static_cast<int>(rhs.width)),
	height(static_cast<int>(rhs.height)) {}

std::ostream& operator<<(std::ostream& s, const Size& size) {
	return s << "Size(" << size.width << ", " << size.height << ")";
}