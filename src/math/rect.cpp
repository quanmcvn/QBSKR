#include "math/rect.hpp"

#include <ostream>

#include "math/rectf.hpp"

Rect::Rect(const Rectf& other) :
	left(static_cast<int>(other.get_left())),
	top(static_cast<int>(other.get_top())),
	right(static_cast<int>(other.get_right())),
	bottom(static_cast<int>(other.get_bottom()))
{}

std::ostream& operator<<(std::ostream& out, const Rect& rect)
{
	out << "Rect("
		<< rect.left << ", " << rect.top << ", "
		<< rect.right << ", " << rect.bottom
		<< ")";
	return out;
}