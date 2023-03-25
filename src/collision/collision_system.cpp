#include "collision/collision_system.hpp"

#include <cmath>

#include "collision/collision_object.hpp"
#include "object/player.hpp"
#include "object/tile_map.hpp"
#include "qbskr/constants.hpp"
#include "qbskr/room.hpp"
#include "video/drawing_context.hpp"

namespace {
	// abitrary (but < 16) choice of MAX_SPEED (pixel/frame)
	// has to cap speed since I don't have collision checking for fast object
	const float MAX_SPEED = 8.0f;
}

CollisionSystem::CollisionSystem(Room& room) :
	m_room(room),
	m_objects()
{}

void CollisionSystem::add(CollisionObject* object)
{
	m_objects.push_back(object);
}

void CollisionSystem::remove(CollisionObject* object)
{
	// O(n), but not a big problem
	m_objects.erase(std::find(m_objects.begin(), m_objects.end(), object));
}

namespace {
	collision::Constraints check_collision(const Vector& object_movenent, const Rectf& moving_object_rect, const Rectf& other_object_rect, 
	                                       CollisionObject* moving_object = nullptr)
	{
		// slightly grow the static object
		const Rectf grown_other_object_rect = other_object_rect.grown(EPSILON);
	
		collision::Constraints constraints;

		if (!collision::intersects(moving_object_rect, grown_other_object_rect)) {
			return constraints;
		}

		const float intersect_left   = moving_object_rect.get_right() - grown_other_object_rect.get_left();
		const float intersect_right  = grown_other_object_rect.get_right() - moving_object_rect.get_left();
		const float intersect_top    = moving_object_rect.get_bottom() - grown_other_object_rect.get_top();
		const float intersect_bottom = grown_other_object_rect.get_bottom() - moving_object_rect.get_top();

		bool shift_out = false;

		if (moving_object) {
			if (dynamic_cast<Player*>(moving_object->get_listener())) {
				if (std::abs(object_movenent.x) < std::abs(object_movenent.y)) {
					if (intersect_left < SHIFT_DELTA) {
						constraints.constrain_right(grown_other_object_rect.get_left());
						shift_out = true;
					} else if (intersect_right < SHIFT_DELTA) {
						constraints.constrain_left(grown_other_object_rect.get_right());
						shift_out = true;
					}
				} else {
					if (intersect_top < SHIFT_DELTA) {
						constraints.constrain_bottom(grown_other_object_rect.get_top());
						shift_out = true;
					} else if (intersect_bottom < SHIFT_DELTA) {
						constraints.constrain_top(grown_other_object_rect.get_bottom());
						shift_out = true;
					}
				}
			}
		}

		if (!shift_out) {
			const float horizontal_intersect = std::min(intersect_left, intersect_right);
			const float vertical_intersect = std::min(intersect_top, intersect_bottom);

			if (horizontal_intersect < vertical_intersect) {
				if (intersect_left < intersect_right) {
					constraints.constrain_right(grown_other_object_rect.get_left());
					constraints.hit.right = true;
				} else {
					constraints.constrain_left(grown_other_object_rect.get_right());
					constraints.hit.left = true;
				}
			} else {
				if (intersect_top < intersect_bottom) {
					constraints.constrain_bottom(grown_other_object_rect.get_top());
					constraints.hit.bottom = true;
				} else {
					constraints.constrain_top(grown_other_object_rect.get_bottom());
					constraints.hit.top = true;
				}
			}
		}

		return constraints;
	}

	// set CollisionHit and Vector normal of 2 intersecting rects
	void get_hit_normal(const Rectf& r1, const Rectf& r2, CollisionHit& hit, Vector& normal)
	{
		const float intersect_left   = r1.get_right() - r2.get_left();
		const float intersect_right  = r2.get_right() - r1.get_left();
		const float intersect_top    = r1.get_bottom() - r2.get_top();
		const float intersect_bottom = r2.get_bottom() - r1.get_top();
	
		const float horizontal_intersect = std::min(intersect_left, intersect_right);
		const float vertical_intersect = std::min(intersect_top, intersect_bottom);

		if (horizontal_intersect < vertical_intersect) {
			if (intersect_left < intersect_right) {
				hit.right = true;
				normal.x = horizontal_intersect;
			} else {
				hit.left = true;
				normal.x = -horizontal_intersect;
			}
		} else {
			if (intersect_top < intersect_bottom) {
				hit.bottom = true;
				normal.y = vertical_intersect;
			} else {
				hit.top = true;
				normal.y = -vertical_intersect;
			}
		}
	}

	int sign(int x) { return (0 < x) - (x < 0); }

	// use exclusive in following algorithm
	struct VectorInt {
		int x;
		int y;
		VectorInt() : x(0), y(0) {}
		VectorInt(int x_, int y_) : x(x_), y(y_) {}
		Vector operator*(float m) const { return Vector(x, y) * m; }
	};

	std::vector<VectorInt> tile_position_intersect_line(int x1, int y1, int x2, int y2)
	{
		// Bresenham's line algorithm
		// https://www.uobabylon.edu.iq/eprints/publication_2_22893_6215.pdf

		std::vector<VectorInt> ret;
		int x = x1;
		int y = y1;
		int dx = std::abs(x2 - x1);
		int dy = std::abs(y2 - y1);
		const int signx = sign(x2 - x1);
		const int signy = sign(y2 - y1);
		bool interchange = false;
		if (dy > dx) {
			std::swap(dx, dy);
			interchange = true;
		}
		int error = 2 * dy - dx;
		const int A = 2 * dy;
		const int B = 2 * (dy - dx);

		ret.emplace_back(x, y);
		for (int i = 1; i <= dx; ++ i) {
			if (error < 0) {
				if (interchange) y += signy;
				else x += signx; 
				error += A;
			} else {
				y += signy;
				x += signx;
				error += B;
			}
			ret.emplace_back(x, y);
		}
		return ret;
	}
}

void CollisionSystem::update()
{
	// calculate destitaion of objects
	for (const auto& object : m_objects) {
		const Vector& mov = object->get_movement();

		if (math::length(mov) > MAX_SPEED) {
			object->set_movement(math::normalize(mov) * MAX_SPEED);
		}

		object->m_dest = object->get_bounding_box();
		object->m_dest.move(object->get_movement());
	}

	// part 1: COLLISION_GROUP_MOVING vs COLLISION_GROUP_STATIC (NYI) and tilemap
	for (const auto& object : m_objects) {
		if (!object->is_valid()) continue;
		if (object->get_group() != COLLISION_GROUP_MOVING &&
		    object->get_group() != COLLISION_GROUP_STATIC)
			continue;

		collision_static_tilemap_object(*object);
	}

	// part ???

	// part last: COLLISION_GROUP_MOVING vs COLLISION_GROUP_MOVING
	for (auto it = m_objects.begin(); it != m_objects.end(); ++ it) {
		// deference it since it is (approximately) CollisionObject**
		auto object = *it;

		if (!object->is_valid()) continue;
		if (object->get_group() != COLLISION_GROUP_MOVING) continue;

		for (auto jt = it + 1; jt != m_objects.end(); ++ jt) {
			auto object2 = *jt;
			if (!object2->is_valid()) continue;
			if (object2->get_group() != COLLISION_GROUP_MOVING) continue;
		
			collision_object(*object, *object2);
		}
	}

	// apply object movement
	for (auto* object : m_objects) {
		object->m_bounding_box = object->m_dest;
		object->m_movement = Vector(0.0f, 0.0f);
	}
}

void CollisionSystem::draw_debug(DrawingContext& drawing_context)
{
	const Color violet(0.5f, 0.0f, 1.0f, 0.75f);
	const Color green_bright(0.7f, 1.0f, 0.7f, 0.75f);
	for (auto& object : m_objects) {
		Color color;
		switch (object->get_group()) {
			case COLLISION_GROUP_MOVING:
				color = violet;
				break;
			default:
				color = green_bright;
				break;
		}
		drawing_context.get_canvas().draw_filled_rect(object->get_bounding_box(), color, LAYER_FOREGROUND + 10);
	}
}

void CollisionSystem::collision_static_constraints(collision::Constraints* /* constraints */, const Vector& /* movement */, const Rectf& /* dest */, CollisionObject& /* object */) const
{
	// NYI
}

void CollisionSystem::collision_tilemap_constraints(collision::Constraints* constraints, const Vector& movement, const Rectf& dest, CollisionObject& object) const
{
	for (auto solid_tilemap : m_room.get_solid_tilemaps()) {
		const Rect test_tiles = solid_tilemap->get_tiles_overlap(dest);

		for (int x = test_tiles.left; x < test_tiles.right; ++ x) {
			for (int y = test_tiles.top; y < test_tiles.bottom; ++ y) {
				const Tile& tile = solid_tilemap->get_tile(x, y);

				// skip non solid tiles
				if (!tile.is_solid()) continue;
				Rectf tile_bounding_box = solid_tilemap->get_tile_bounding_box(x, y);

				collision::Constraints new_constraints = check_collision(movement, dest, tile_bounding_box, &object);
				constraints->merge_constraints(new_constraints);
			}
		}
	}
}

void CollisionSystem::collision_static_tilemap_constraints(collision::Constraints* constraints, const Vector& movement, const Rectf& dest, CollisionObject& object) const
{
	collision_tilemap_constraints(constraints, movement, dest, object);
	collision_static_constraints(constraints, movement, dest, object);
}

void CollisionSystem::collision_static_tilemap_object(CollisionObject& object) const
{
	collision::Constraints constraints;
	const Vector movement = object.m_movement;
	Rectf& dest = object.m_dest;

	for (int i = 0; i < 2; ++ i) {
		collision_static_tilemap_constraints(&constraints, Vector(0.0f, movement.y), dest, object);
		if (!constraints.has_constraints()) {
			break;
		}

		if (constraints.has_constraint_bottom()) {
			dest.set_bottom(constraints.get_position_bottom() - EPSILON);
			dest.set_top(dest.get_bottom() - object.get_bounding_box().get_height());
		} else if (constraints.has_constraint_top()) {
			dest.set_top(constraints.get_position_top() + EPSILON);
			dest.set_bottom(dest.get_top() + object.get_bounding_box().get_height());
		}
	}
	if (constraints.has_constraints()) {
		if (constraints.hit.top || constraints.hit.bottom) {
			constraints.hit.left = false;
			constraints.hit.right = false;
			object.collision_solid(constraints.hit);
		}
	}

	constraints = collision::Constraints();
	for (int i = 0; i < 2; ++ i) {
		collision_static_tilemap_constraints(&constraints, movement, dest, object);
		if (!constraints.has_constraints()) {
			break;
		}
		if (constraints.has_constraint_right()) {
			dest.set_right(constraints.get_position_right() - EPSILON);
			dest.set_left(dest.get_right() - object.get_bounding_box().get_width());
		} else if (constraints.has_constraint_left()) {
			dest.set_left(constraints.get_position_left() + EPSILON);
			dest.set_right(dest.get_left() + object.get_bounding_box().get_width());
		} 
	}
	if (constraints.has_constraints()) {
		if (constraints.hit.left || constraints.hit.right || constraints.hit.top || constraints.hit.bottom) {
			object.collision_solid(constraints.hit);
		}
	}
}

void CollisionSystem::collision_object(CollisionObject& object1, CollisionObject& object2) const
{
	for (int i = 0; i < 4; ++ i) {
		const Rectf& r1 = object1.m_dest;
		const Rectf& r2 = object2.m_dest;

		if (!collision::intersects(r1, r2)) return;
		CollisionHit hit;
		Vector normal(0.0f, 0.0f);
		get_hit_normal(r1, r2, hit, normal);

		HitResponse response1 = object1.collision(object2, hit);
		std::swap(hit.left, hit.right);
		std::swap(hit.top, hit.bottom);
		HitResponse response2 = object2.collision(object1, hit);
		std::swap(hit.left, hit.right);
		std::swap(hit.top, hit.bottom);
		
		if (response1 == CONTINUE && response2 == CONTINUE) {
			normal *= (0.5f + EPSILON);
			object1.m_dest.move(-normal);
			object2.m_dest.move(normal);
		} else if (response1 == CONTINUE && response2 == FORCE_MOVE) {
			normal *= (1.0f + EPSILON);
			object1.m_dest.move(-normal);
		} else if (response1 == FORCE_MOVE && response2 == CONTINUE) {
			normal *= (1.0f + EPSILON);
			object2.m_dest.move(normal);
		}
	}
}

bool CollisionSystem::is_free_of_tiles(const Rectf& rect, uint32_t tiletype) const
{
	for (const auto& solids : m_room.get_solid_tilemaps()) {
		const Rect test_tiles = solids->get_tiles_overlap(rect);

		for (int x = test_tiles.left; x < test_tiles.right; ++ x) {
			for (int y = test_tiles.top; y < test_tiles.bottom; ++ y) {
				const Tile& tile = solids->get_tile(x, y);

				if (!(tile.get_attributes() & tiletype)) continue;

				return false;
			}
		}
	}

	return true;
}

bool CollisionSystem::free_line_of_sight(const Vector& line_start, const Vector& line_end) const
{
	// normalizing line_start and line_end
	// note the / BLOCK_SIZE
	int x1 = std::floor(line_start.x / BLOCK_SIZE);
	int y1 = std::floor(line_start.y / BLOCK_SIZE);
	int x2 = std::floor(line_end.x / BLOCK_SIZE);
	int y2 = std::floor(line_end.y / BLOCK_SIZE);

	std::vector<VectorInt> points_to_test = tile_position_intersect_line(x1, y1, x2, y2);
	for (const auto& solids : m_room.get_solid_tilemaps()) {
		for (const auto& point_ : points_to_test) {
			// convert back by * BLOCK_SIZE
			const Vector point = point_ * BLOCK_SIZE;

			if (solids->is_outside_bounds(point)) continue;
			const auto& tile = solids->get_tile_at(point);
			

			if (tile.is_solid()) return false;
		}
	}
	return true;
}