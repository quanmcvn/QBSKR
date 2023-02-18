#ifndef HEADER_QBMG_MATH_RECTF_HPP
#define HEADER_QBMG_MATH_RECTF_HPP

#include <assert.h>
#include <iosfwd>

#include "math/anchor_point.hpp"
#include "math/sizef.hpp"
#include "math/vector.hpp"

class Rect;

class Rectf final {
private:
	/// upper left edge
	Vector m_p1;
	Sizef m_size;
public:
	static Rectf from_center(const Vector& center, const Sizef& size) {
		return Rectf(center.x - size.width / 2.0f,
		             center.y - size.height / 2.0f,
		             center.x + size.width / 2.0f,
		             center.y + size.height / 2.0f);
	}

public:
	Rectf():
		m_p1(0.0f, 0.0f),
		m_size() {}

	Rectf(const Rectf& rhs) = default;
	Rectf& operator=(const Rectf& rhs) = default;

	Rectf(const Vector& np1, const Vector& np2):
		m_p1(np1), m_size(np2.x - np1.x, np2.y - np1.y) {
		assert(m_size.width >= 0 &&
		       m_size.height >= 0);
	}

	Rectf(float x1, float y1, float x2, float y2):
		m_p1(x1, y1), m_size(x2 - x1, y2 - y1) {
		assert(m_size.width >= 0 &&
		       m_size.height >= 0);
	}

	Rectf(const Vector& p1, const Sizef& size):
		m_p1(p1),
		m_size(size) {}

	Rectf(const Rect& rect);

	bool operator==(const Rectf& other) const {
		return (m_p1 == other.m_p1 &&
		        m_size == other.m_size);
	}

	float get_left() const { return m_p1.x; }
	float get_right() const { return m_p1.x + m_size.width; }
	float get_top() const { return m_p1.y; }
	float get_bottom() const { return m_p1.y + m_size.height; }

	float get_width() const { return m_size.width; }
	float get_height() const { return m_size.height; }

	void set_left(float v) { m_size.width -= v - m_p1.x; m_p1.x = v; }
	void set_right(float v) { m_size.width += v - get_right(); }

	void set_top(float v) { m_size.height -= v - m_p1.y; m_p1.y = v; }
	void set_bottom(float v) { m_size.height += v - get_bottom(); }

	Vector get_middle() const {
		return Vector(m_p1.x + m_size.width / 2.0f,
		              m_p1.y + m_size.height / 2.0f);
	}

	void set_pos(const Vector& v) { m_p1 = v; }

	void set_width(float width) { m_size.width = width; }
	void set_height(float height) { m_size.height = height; }
	void set_size(float width, float height) { m_size = Sizef(width, height); }
	Sizef get_size() const { return m_size; }

	void move(const Vector& v) { m_p1 += v; }
	Rectf moved(const Vector& v) const { return Rectf(m_p1 + v, m_size); }

	bool contains(const Vector& v) const {
		return get_left() <= v.x && v.x < get_right() && get_top() <= v.y && v.y < get_bottom();
	}

	bool contains(const Rectf& other) const {
		return (get_left() <= other.get_left() && other.get_right() <= get_right() &&
		        get_top() <= other.get_top() && other.get_bottom() <= get_bottom());
	}

	float distance(const Vector& other, AnchorPoint ap = ANCHOR_MIDDLE) const {
		Vector v = get_anchor_pos(*this, ap);
		return glm::distance(v, other);
	}

	float distance(const Rectf& other, AnchorPoint ap = ANCHOR_MIDDLE) const {
		Vector v1 = get_anchor_pos(*this, ap);
		Vector v2 = get_anchor_pos(other, ap);

		return glm::distance(v1, v2);
	}

	Rectf grown(float border) const {
		return Rectf(m_p1.x - border, m_p1.y - border,
			get_right() + border, get_bottom() + border);
	}

	Vector p1() const { return m_p1; }
	Vector p2() const { return Vector(m_p1.x + m_size.width, m_p1.y + m_size.height); }

	void set_p1(const Vector& p) {
		m_size = Sizef(m_size.width + (m_p1.x - p.x),
		               m_size.height + (m_p1.y - p.y));
		m_p1 = p;
	}
	void set_p2(const Vector& p) {
		m_size = Sizef(p.x - m_p1.x,
		               p.y - m_p1.y);
	}
};

std::ostream& operator<<(std::ostream& out, const Rectf& rect);


#endif