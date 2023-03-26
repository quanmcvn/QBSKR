#include "qbskr/room.hpp"

#include <limits>

#include "collision/collision_system.hpp"
#include "object/moving_object.hpp"
#include "object/player.hpp"
#include "object/tile_map.hpp"
#include "qbskr/gameconfig.hpp"
#include "qbskr/globals.hpp"
#include "video/drawing_context.hpp"

Room* Room::s_current = nullptr;

Room::Room() :
	m_collision_system(std::make_unique<CollisionSystem>(*this))
{}

Room::~Room()
{
	deactivate();
	clear_objects();
}

Room& Room::get() { assert(s_current != nullptr); return *s_current; }
Room* Room::current() { return s_current; }

void Room::update(float dt_sec)
{
	GameObjectManager::update(dt_sec);
	m_collision_system->update();
	flush_game_objects();
}

void Room::draw(DrawingContext& drawing_context)
{
	drawing_context.push_transform();

	GameObjectManager::draw(drawing_context);
	
	if (g_config->debug) m_collision_system->draw_debug(drawing_context);

	drawing_context.pop_transform();
}

void Room::activate()
{
	s_current = this;
}

void Room::deactivate()
{
	s_current = nullptr;
}

bool Room::before_object_add(GameObject& object)
{
	if (auto moving_object = dynamic_cast<MovingObject*>(&object)) {
		m_collision_system->add(moving_object->get_collision_object());
	}

	return true;
}

void Room::before_object_remove(GameObject& object)
{
	if (auto moving_object = dynamic_cast<MovingObject*>(&object)) {
		m_collision_system->remove(moving_object->get_collision_object());
	}
}

bool Room::is_free_of_tiles(const Rectf& rect, uint32_t tiletype) const
{
	return m_collision_system->is_free_of_tiles(rect, tiletype);
}

bool Room::free_line_of_sight(const Vector& line_start, const Vector& line_end) const
{
	return m_collision_system->free_line_of_sight(line_start, line_end);
}

bool Room::can_see_player(const Vector& eye) const
{
	for (auto player_ptr : get_objects_by_type_index(typeid(Player))) {
		Player& player = *static_cast<Player*>(player_ptr);
		// test for free line of sight to any of 4 corners and middle of player's bounding box
		if (free_line_of_sight(eye, Vector(player.get_bounding_box().get_left(), player.get_bounding_box().get_top()))) return true;
		if (free_line_of_sight(eye, Vector(player.get_bounding_box().get_left(), player.get_bounding_box().get_bottom()))) return true;
		if (free_line_of_sight(eye, Vector(player.get_bounding_box().get_right(), player.get_bounding_box().get_bottom()))) return true;
		if (free_line_of_sight(eye, Vector(player.get_bounding_box().get_right(), player.get_bounding_box().get_top()))) return true;
		if (free_line_of_sight(eye, player.get_bounding_box().get_middle())) return true;
	}
	return false;
}

bool Room::inside(const Rectf& rect) const
{
	for (const auto& solids : get_solid_tilemaps()) {
		Rectf bounding_box = solids->get_bounding_box();
		if (!(bounding_box.contains(rect))) return false;
	}
	return true;
}

Player* Room::get_nearest_player(const Vector& pos) const
{
	Player* nearest_player = nullptr;
	float nearest_dist = std::numeric_limits<float>::max();

	for (auto player_ptr : get_objects_by_type_index(typeid(Player))) {
		Player& player = *static_cast<Player*>(player_ptr);

		float dist = player.get_bounding_box().distance(pos);

		if (dist < nearest_dist) {
			nearest_player = &player;
			nearest_dist = dist;
		}
	}

	return nearest_player;
}