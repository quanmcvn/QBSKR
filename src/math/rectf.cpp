#include "math/rectf.hpp"

#include <ostream>

#include "math/rect.hpp"

Rectf::Rectf(const Rect& rect) :
	m_p1(static_cast<float>(rect.left),
		static_cast<float>(rect.top)),
	m_size(static_cast<float>(rect.get_width()),
		static_cast<float>(rect.get_height()))
{}

std::ostream& operator<<(std::ostream& out, const Rectf& rect)
{
	out << "Rectf("
		<< rect.get_left() << ", " << rect.get_top() << ", "
		<< rect.get_right() << ", " << rect.get_bottom()
		<< ")";
	return out;
}