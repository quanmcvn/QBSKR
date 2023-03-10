#ifndef HEADER_QBSKR_MATH_RECTF_HPP
#define HEADER_QBSKR_MATH_RECTF_HPP

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
	static Rectf from_center(const Vector& center, const Sizef& size);

public:
	Rectf();

	Rectf(const Rectf& rhs) = default;
	Rectf& operator=(const Rectf& rhs) = default;

	Rectf(const Vector& np1, const Vector& np2);
	Rectf(float x1, float y1, float x2, float y2);
	Rectf(const Vector& p1, const Sizef& size);

	Rectf(const Rect& rect);

	bool operator==(const Rectf& other) const;

	float get_left() const;
	float get_right() const;
	float get_top() const;
	float get_bottom() const;
	float get_width() const;
	float get_height() const;
	Sizef get_size() const;

	void set_left(float v);
	void set_right(float v);
	void set_top(float v);
	void set_bottom(float v);
	void set_width(float width);
	void set_height(float height);
	void set_size(float width, float height);

	Vector get_middle() const;

	void set_pos(const Vector& v);

	void move(const Vector& v);
	Rectf moved(const Vector& v) const;

	bool contains(const Vector& v) const;
	bool contains(const Rectf& other) const;

	float distance(const Vector& other, AnchorPoint ap = ANCHOR_MIDDLE) const
	{
		Vector v = get_anchor_pos(*this, ap);
		return math::distance(v, other);
	}

	float distance(const Rectf& other, AnchorPoint ap = ANCHOR_MIDDLE) const
	{
		Vector v1 = get_anchor_pos(*this, ap);
		Vector v2 = get_anchor_pos(other, ap);

		return math::distance(v1, v2);
	}

	Rectf grown(float border) const;
	Vector p1() const;
	Vector p2() const;

	void set_p1(const Vector& p);
	void set_p2(const Vector& p);
};

std::ostream& operator<<(std::ostream& out, const Rectf& rect);


#endif