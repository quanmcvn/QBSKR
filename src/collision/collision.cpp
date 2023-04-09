#include "collision/collision.hpp"

#include <algorithm>
#include <limits>

#include "math/rectf.hpp"

namespace {
	constexpr float INFINITY = std::numeric_limits<float>::has_infinity ?
	                           std::numeric_limits<float>::infinity() :
	                           std::numeric_limits<float>::max();
}

namespace collision {

	Constraints::Constraints() :
		hit(),
		position_left(-INFINITY),
		position_right(+INFINITY),
		position_top(-INFINITY),
		position_bottom(+INFINITY)
	{}

	bool Constraints::has_constraints() const
	{
		return
			has_constraint_left() ||
			has_constraint_right() ||
			has_constraint_top() ||
			has_constraint_bottom();
	}

	bool Constraints::has_constraint_left() const { return position_left > -INFINITY; }
	bool Constraints::has_constraint_right() const { return position_right < +INFINITY; }
	bool Constraints::has_constraint_top() const { return position_top > -INFINITY; }
	bool Constraints::has_constraint_bottom() const { return position_bottom < +INFINITY; }

	void Constraints::constrain_left(float position) { position_left = std::max(position_left, position); }
	void Constraints::constrain_right(float position) {position_right = std::min(position_right, position); }
	void Constraints::constrain_top(float position) { position_top = std::max(position_top, position); }
	void Constraints::constrain_bottom(float position) {position_bottom = std::min(position_bottom, position); }

	float Constraints::get_position_left() const { return position_left; }
	float Constraints::get_position_right() const { return position_right; }
	float Constraints::get_position_top() const { return position_top; }
	float Constraints::get_position_bottom() const { return position_bottom; }

	void Constraints::merge_constraints(const Constraints& other)
	{
		constrain_left(other.get_position_left());
		constrain_right(other.get_position_right());
		constrain_top(other.get_position_top());
		constrain_bottom(other.get_position_bottom());

		hit.left |= other.hit.left;
		hit.right |= other.hit.right;
		hit.top |= other.hit.top;
		hit.bottom |= other.hit.bottom;
	}

	bool intersects(const Rectf& r1, const Rectf& r2)
	{
		if (r1.get_right() < r2.get_left() || r2.get_right() < r1.get_left())
			return false;
		if (r1.get_bottom() < r2.get_top() || r2.get_bottom() < r1.get_top())
			return false;
		return true;
	}
}