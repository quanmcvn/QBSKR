#include "qbskr/room.hpp"

#include <limits>

#include "collision/collision_system.hpp"
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
	m_room_data(std::move(room_data)),
	room_left(),
	room_right(),
	room_up(),
	room_down()
{
	m_room_data->m_parent = this;
	// stealing tilemap from room_data, sorry
	add_object(std::move(m_room_data->m_tilemap));
	flush_game_objects();
}

Room& Room::get() 
{
	if (s_current) return *s_current;
	assert(s_current != nullptr);
	// make compiler happy 
	return *s_current;
}

Room* Room::current() { return s_current; }

void Room::update(float dt_sec)
{
	GameObjectManager::update(dt_sec);
	m_collision_system->update();
	m_room_data->update();
	flush_game_objects();
}

void Room::draw(DrawingContext& drawing_context)
{
	drawing_context.push_transform();

	Camera& camera = Room::get().get_camera();
	drawing_context.set_translation(camera.get_translation());

	GameObjectManager::draw(drawing_context);
	
	if (g_config->debug) m_collision_system->draw_debug(drawing_context);

	drawing_context.pop_transform();
}

void Room::activate()
{
	s_current = this;
	get_camera().update(1);
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
	Rectf bounding_box = get_solid_tilemaps()[0]->get_bounding_box();
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
	if (static_cast<int>(cameras.size()) != 1) {
		log_fatal << "cameras.size() is " << cameras.size() << " not 1" << std::endl;
		assert(cameras.size() == 1);
	}
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

void Room::spawn_badguy() { m_room_data->spawn_badguy(); }
bool Room::is_turn_cleared() const { return m_room_data->is_turn_cleared(); }
bool Room::is_room_cleared() const { return m_room_data->is_room_cleared(); }

RoomType Room::get_room_type() const { return m_room_data->m_type; }

void Room::clone_camera_and_players_to(Room& other)
{
	for (auto& object : get_objects_non_const()) {
		if (dynamic_cast<Camera*>(object.get())) {
			other.add_object(static_cast<Camera*>(object.get())->clone());
			object->remove_me();
		} else if (dynamic_cast<Player*>(object.get())) {
			other.add_object(static_cast<Player*>(object.get())->clone());
			object->remove_me();
		}
	}
	other.flush_game_objects();
}

void Room::close_room()
{
	const uint32_t FENCE_DOWN = 3;
	const uint32_t FENCE_UP = 4;
	for (auto& solid : get_solid_tilemaps_non_const()) {
		if (room_left) for (int y = 0; y < solid->get_height(); ++ y) if (solid->get_tile_id(0, y) == FENCE_DOWN) solid->change(0, y, FENCE_UP);
		if (room_right) for (int y = 0; y < solid->get_height(); ++ y) if (solid->get_tile_id(solid->get_width() - 1, y) == FENCE_DOWN) solid->change(solid->get_width() - 1, y, FENCE_UP);
		if (room_up) for (int x = 0; x < solid->get_width(); ++ x) if (solid->get_tile_id(x, 0) == FENCE_DOWN) solid->change(x, 0, FENCE_UP);
		if (room_down) for (int x = 0; x < solid->get_width(); ++ x) if (solid->get_tile_id(x, solid->get_height() - 1) == FENCE_DOWN) solid->change(x, solid->get_height() - 1, FENCE_UP);
	}
}

void Room::open_room()
{
	const uint32_t FENCE_DOWN = 3;
	const uint32_t FENCE_UP = 4;
	for (auto& solid : get_solid_tilemaps_non_const()) {
		if (room_left) for (int y = 0; y < solid->get_height(); ++ y) if (solid->get_tile_id(0, y) == FENCE_UP) solid->change(0, y, FENCE_DOWN);
		if (room_right) for (int y = 0; y < solid->get_height(); ++ y) if (solid->get_tile_id(solid->get_width() - 1, y) == FENCE_UP) solid->change(solid->get_width() - 1, y, FENCE_DOWN);
		if (room_up) for (int x = 0; x < solid->get_width(); ++ x) if (solid->get_tile_id(x, 0) == FENCE_UP) solid->change(x, 0, FENCE_DOWN);
		if (room_down) for (int x = 0; x < solid->get_width(); ++ x) if (solid->get_tile_id(x, solid->get_height() - 1) == FENCE_UP) solid->change(x, solid->get_height() - 1, FENCE_DOWN);
	}
}