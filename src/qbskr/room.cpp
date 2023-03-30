#include "qbskr/room.hpp"

#include <limits>

#include "badguy/badguy_set.hpp"
#include "badguy/badguy.hpp"
#include "collision/collision_system.hpp"
#include "math/random.hpp"
#include "object/camera.hpp"
#include "object/moving_object.hpp"
#include "object/player.hpp"
#include "object/tile_map.hpp"
#include "qbskr/gameconfig.hpp"
#include "qbskr/globals.hpp"
#include "qbskr/room_data_set.hpp"
#include "util/log.hpp"
#include "video/drawing_context.hpp"

Room* Room::s_current = nullptr;

Room::~Room()
{
	deactivate();
	clear_objects();
}

Room::Room(std::unique_ptr<RoomData> room_data) :
	m_collision_system(std::make_unique<CollisionSystem>(*this)),
	m_room_data(std::move(room_data))
{
	// stealing tilemap from room_data, sorry
	add_object(std::move(m_room_data->m_tilemap));
	flush_game_objects();
}

Room& Room::get() { assert(s_current != nullptr); return *s_current; }
Room* Room::current() { return s_current; }

void Room::update(float dt_sec)
{
	spawn_badguy();
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
	if (object.is_singleton()) {
		const auto& objects = get_objects_by_type_index(typeid(object));
		if (!objects.empty()) {
			log_warning << "Can't insert multiple GameObject of type '" << typeid(object).name() << "', discarding" << std::endl;
			return false;
		}
	}

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

Rectf Room::get_bounding_box() const
{
	Rectf bounding_box;
	for (const auto& solids : get_solid_tilemaps()) {
		Rectf solid_bounding_box = solids->get_bounding_box();
		if (bounding_box.get_left() > solid_bounding_box.get_left()) bounding_box.set_left(solid_bounding_box.get_left());
		if (bounding_box.get_right() < solid_bounding_box.get_right()) bounding_box.set_right(solid_bounding_box.get_right());
		if (bounding_box.get_top() > solid_bounding_box.get_top()) bounding_box.set_top(solid_bounding_box.get_top());
		if (bounding_box.get_bottom() < solid_bounding_box.get_bottom()) bounding_box.set_bottom(solid_bounding_box.get_bottom());
	}
	return bounding_box;
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

Camera& Room::get_camera() const
{
	auto cameras = get_objects_by_type_index(std::type_index(typeid(Camera)));
	assert(cameras.size() == 1);
	return *static_cast<Camera*>(cameras[0]);
}

std::vector<Player*> Room::get_players() const
{
	std::vector<Player*> ret;
	for (const auto& player_ptr : get_objects_by_type_index(std::type_index(typeid(Player)))) {
		ret.push_back(static_cast<Player*>(player_ptr));
	}
	return ret;
}

void Room::spawn_badguy()
{
	if (m_room_data->m_turns <= 0) return;
	if (!is_turn_cleared()) return;

	-- m_room_data->m_turns;
	int badguy_spawns = g_game_random.rand_inclusive(m_room_data->m_min_per_turn, m_room_data->m_max_per_turn);
	
	// spawn badguy
	// try at most 10 times
	// if fail then give up
	for (int i = 0; i < badguy_spawns; ++ i) {
		int spawn_id = m_room_data->m_badguys[g_game_random.rand(0, m_room_data->m_badguys.size())];
		Rectf bounding_box = get_bounding_box();
		Vector spawn_pos(g_game_random.randf(bounding_box.get_left(), bounding_box.get_right()), g_game_random.randf(bounding_box.get_top(), bounding_box.get_bottom()));
		Rectf badguy_bounding_box = BadGuySet::current()->get_badguy(spawn_id).get_bounding_box();
		
		const int max_tries = 10;
		for (int tries = 0; tries < max_tries; ++ tries) {
			badguy_bounding_box.set_pos(spawn_pos);
			if (is_free_of_tiles(badguy_bounding_box)) break;
			spawn_pos = Vector(g_game_random.randf(bounding_box.get_left(), bounding_box.get_right()), g_game_random.randf(bounding_box.get_top(), bounding_box.get_bottom()));
		}

		if (!is_free_of_tiles(badguy_bounding_box)) {
			log_warning << "Can't spawn badguy id '" << spawn_id << "'" << std::endl;
			continue;
		}

		add_object(BadGuySet::current()->get_badguy(spawn_id).clone(spawn_pos));
	}
}

bool Room::is_turn_cleared() const
{
	// if any of gameobjects are badguy and not dead
	// note that can't use std::all_of() here since need to get badguys and its inherits 
	// get_objects_by_type_index use typeid, and typeid is exact match, not considering inheritance
	return !std::any_of(get_objects().begin(), get_objects().end(), 
		[] (const std::unique_ptr<GameObject>& object) {
			auto badguy_ptr = dynamic_cast<BadGuy*>(object.get());
			if (!badguy_ptr) return false;
			return !badguy_ptr->is_dead();
		});
}