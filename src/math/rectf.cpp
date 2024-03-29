#include "math/rectf.hpp"

#include <ostream>

#include "math/rect.hpp"

Rectf Rectf::from_center(const Vector& center, const Sizef& size)
{
	return Rectf(center.x - size.width / 2.0f,
	             center.y - size.height / 2.0f,
	             center.x + size.width / 2.0f,
	             center.y + size.height / 2.0f);
}

Rectf::Rectf() :
	m_p1(0.0f, 0.0f),
	m_size()
{}

Rectf::Rectf(const Vector& np1, const Vector& np2) :
	m_p1(np1),
	m_size(np2.x - np1.x, np2.y - np1.y)
{
	assert(m_size.width >= 0 && m_size.height >= 0);
}

Rectf::Rectf(float x1, float y1, float x2, float y2) :
	m_p1(x1, y1),
	m_size(x2 - x1, y2 - y1)
{
	assert(m_size.width >= 0 && m_size.height >= 0);
}

Rectf::Rectf(const Vector& p1, const Sizef& size) :
	m_p1(p1),
	m_size(size)
{}

Rectf::Rectf(const Rect& rect) :
	m_p1(static_cast<float>(rect.left), static_cast<float>(rect.top)),
	m_size(static_cast<float>(rect.get_width()), static_cast<float>(rect.get_height()))
{}

bool Rectf::operator==(const Rectf& other) const 
{
	return (m_p1 == other.m_p1 && m_size == other.m_size);
}

float Rectf::get_left() const { return m_p1.x; }
float Rectf::get_right() const { return m_p1.x + m_size.width; }
float Rectf::get_top() const { return m_p1.y; }
float Rectf::get_bottom() const { return m_p1.y + m_size.height; }
float Rectf::get_width() const { return m_size.width; }
float Rectf::get_height() const { return m_size.height; }
Sizef Rectf::get_size() const {	return m_size; }

void Rectf::set_left(float v) { m_size.width -= v - m_p1.x; m_p1.x = v; }
void Rectf::set_right(float v) { m_size.width += v - get_right(); }
void Rectf::set_top(float v) { m_size.height -= v - m_p1.y; m_p1.y = v; }
void Rectf::set_bottom(float v) { m_size.height += v - get_bottom(); }
void Rectf::set_width(float width) { m_size.width = width; }
void Rectf::set_height(float height) { m_size.height = height; }
void Rectf::set_size(float width, float height) { m_size = Sizef(width, height); }

Vector Rectf::get_middle() const
{
	return Vector(m_p1.x + m_size.width / 2.0f, m_p1.y + m_size.height / 2.0f);
}

void Rectf::set_pos(const Vector& v) { m_p1 = v; }

void Rectf::move(const Vector& v) { m_p1 += v; }
Rectf Rectf::moved(const Vector& v) const {	return Rectf(m_p1 + v, m_size); }

bool Rectf::contains(const Vector& v) const
{
	return get_left() <= v.x && v.x < get_right() && get_top() <= v.y && v.y < get_bottom();
}

bool Rectf::contains(const Rectf& other) const
{
	return (get_left() <= other.get_left() && other.get_right() <= get_right() &&
	        get_top() <= other.get_top() && other.get_bottom() <= get_bottom());
}

float Rectf::distance(const Vector& other) const
{
	return math::distance(get_middle(), other);
}

float Rectf::distance(const Rectf& other) const
{
	return math::distance(get_middle(), other.get_middle());
}

Rectf Rectf::grown(float border) const
{
	return Rectf(m_p1.x - border, m_p1.y - border, get_right() + border, get_bottom() + border);
}

Vector Rectf::p1() const { return m_p1; }
Vector Rectf::p2() const { return Vector(m_p1.x + m_size.width, m_p1.y + m_size.height); }

void Rectf::set_p1(const Vector& p)
{
	m_size = Sizef(m_size.width + (m_p1.x - p.x), m_size.height + (m_p1.y - p.y));
	m_p1 = p;
}

void Rectf::set_p2(const Vector& p)
{
	m_size = Sizef(p.x - m_p1.x, p.y - m_p1.y);
}

std::ostream& operator<<(std::ostream& out, const Rectf& rect)
{
	out << "Rectf("
	    << rect.get_left() << ", " << rect.get_top() << ", "
	    << rect.get_right() << ", " << rect.get_bottom()
	    << ")";
	return out;
}