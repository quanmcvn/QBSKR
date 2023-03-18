#ifndef HEADER_QBSKR_COLLISION_COLLISION_HPP
#define HEADER_QBSKR_COLLISION_COLLISION_HPP

#include "collision/collision_hit.hpp"

class Rectf;

namespace collision {
	/**
	 * Class to store more infomation about collision (although not used much now)
	*/
	class Constraints final {
	public:
		Constraints();

	public:
		CollisionHit hit;

	private:
		float position_left;
		float position_right;
		float position_top;
		float position_bottom;

	public:
		bool has_constraints() const;

		bool has_constraint_left() const;
		bool has_constraint_right() const;
		bool has_constraint_top() const;
		bool has_constraint_bottom() const;

		void constrain_left(float position);
		void constrain_right(float position);
		void constrain_top(float position);
		void constrain_bottom(float position);

		float get_position_left() const;
		float get_position_right() const;
		float get_position_top() const;
		float get_position_bottom() const;

		void merge_constraints(const Constraints& other);
	};

	// checks if 2 rectangle intersect each other 
	bool intersects(const Rectf& r1, const Rectf& r2);
} // namespace collision

#endif