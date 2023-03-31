#include "qbskr/level_data.hpp"

#include <sstream>

#include "math/random.hpp"
#include "math/rectf.hpp"
#include "object/direction.hpp"
#include "qbskr/constants.hpp"
#include "qbskr/level.hpp"
#include "util/crappy_reader.hpp"

namespace {
	// distance between room, in tiles
	const int DISTANCE_BETWEEN_ROOM = 20;
	// hardcode bridge size
	const int BRIDGE_SIZE = 7;
	// for shifting to make bridge at the center
	const float BRIDGE_SHIFT_VALUE = static_cast<float>(BRIDGE_SIZE) / 2 * BLOCK_SIZE;
	// re-use object/direction.hpp here
	// bad design but anyway...
	std::unique_ptr<RoomData> bridge_maker(int length, Direction direction, const Vector& offset, uint32_t road_tile, uint32_t wall_tile)
	{

		std::unique_ptr<TileMap> tilemap = std::make_unique<TileMap>();
		if (direction == Direction::LEFT || direction == Direction::RIGHT) {
			tilemap->resize(length, BRIDGE_SIZE);
			for (int x = 0; x < length; ++ x) {
				for (int y = 0; y < BRIDGE_SIZE; ++ y) {
					if (y == 0 || y == BRIDGE_SIZE - 1) {
						tilemap->change(x, y, wall_tile);
					} else {
						tilemap->change(x, y, road_tile);
					}
				}
			}
		} else if (direction == Direction::UP || direction == Direction::DOWN) {
			tilemap->resize(BRIDGE_SIZE, length);
			for (int x = 0; x < BRIDGE_SIZE; ++ x) {
				for (int y = 0; y < length; ++ y) {
					if (x == 0 || x == BRIDGE_SIZE - 1) {
						tilemap->change(x, y, wall_tile);
					} else {
						tilemap->change(x, y, road_tile);
					}
				}
			}
		} else {
			throw std::runtime_error("Can't make bridge with direction AUTO");
		}

		tilemap->set_offset(offset);

		auto room_data = std::make_unique<RoomData>(RoomType::BRIDGE, std::move(tilemap));
		return room_data;
	}
}

LevelData::LevelData(const std::string& filename) :
	m_rooms_number_random_info(),
	m_rooms_info(),
	m_room_data_set()
{
	CrappyReader cr(filename);
	while (cr.parse("level")) {}

	CrappyReaderData* crd = cr.get_root()->get_child("level");
	if (!crd) {
		std::ostringstream msg;
		msg << "File '" << filename << "' is not level file";
		throw std::runtime_error(msg.str());
	}

	CrappyReaderData* crd_room_number = crd->get_child("room-numbers");
	if (!crd_room_number) {
		throw std::runtime_error("No room-numbers in level file");
	}
	for (const auto& crd_room_type : crd_room_number->m_childs) {
		std::string string_room_type = crd_room_type->m_data;
		RoomType room_type = string_to_room_type(string_room_type);
		std::vector<int> values;
		crd_room_type->get_child_values(values);
		std::pair<int, int> rand_lr;
		if (values.size() == 2) {
			rand_lr = std::pair<int, int>(values[0], values[1]);
		} else if (values.size() == 1) {
			rand_lr = std::pair<int, int>(values[0], values[0]);
		} else {
			throw std::runtime_error("Invalid number of value in room type");
		}
		this->m_rooms_number_random_info[room_type] = rand_lr;
	}
	std::string room_data_set_filename;
	if (!crd->get("room-data-set-filename", room_data_set_filename)) {
		throw std::runtime_error("No room-data-set-filename in level file");
	}
	m_room_data_set = std::make_unique<RoomDataSet>(crd->m_parent_path + room_data_set_filename);

	for (size_t i = 0; i < m_room_data_set->m_room_datas.size(); ++ i) {
		const auto& room_data = m_room_data_set->m_room_datas[i];
		if (!room_data.get()) continue;
		m_rooms_info[room_data->m_type].push_back(i);
	}
}

std::unique_ptr<Level> LevelData::make_level() const
{
	auto level = std::make_unique<Level>();
	std::map<RoomType, int> room_numbers;
	for (const auto& [room_type, rand_lr] : m_rooms_number_random_info) {
		room_numbers[room_type] = g_game_random.rand_inclusive(rand_lr.first, rand_lr.second);
	}

	// spawn 'start' room
	// then chain spawn 'normal' room
	// then spawn 'goal' room at the end of the chain
	// then spawn aditional room (if the level has)
	// hardcoded a level must have 1 start and 1 goal room

	Direction dir_prev = Direction::AUTO;
	const auto& start_room_ids = m_rooms_info.at(RoomType::START);
	const auto& normal_room_ids = m_rooms_info.at(RoomType::NORMAL);
	const auto& goal_room_ids = m_rooms_info.at(RoomType::GOAL);

	Rectf prev_bounding_box;
	// spawn room in such a way that its center is center_pos
	Vector center_pos(0.0f, 0.0f);
	{
		int start_room_id = start_room_ids[g_game_random.rand(0, start_room_ids.size())];
		Vector spawn_room_pos = center_pos - m_room_data_set->get_room_data(start_room_id).m_tilemap->get_bounding_box().get_middle();
		prev_bounding_box = Rectf(spawn_room_pos, m_room_data_set->get_room_data(start_room_id).m_tilemap->get_bounding_box().get_size());
		auto start_room = std::make_unique<Room>(m_room_data_set->get_room_data(start_room_id).clone(spawn_room_pos));
		level->add_room(std::move(start_room));
	}
	// note that there is no Direction::LEFT
	// intentional going right
	const std::vector<Direction> directions = {Direction::RIGHT, Direction::UP, Direction::DOWN};
	// +1 for goal room
	for (int i = 0; i < room_numbers[RoomType::NORMAL] + 1; ++ i) {
		std::vector<Direction> dir_possible = directions;
		// verbose way of removing easy to spot conflict direction
		// eg. previous direction is up then now can't be down
		dir_possible.erase(
			std::remove_if(
				dir_possible.begin(), 
				dir_possible.end(), 
				[dir_prev](Direction d) {
					if (dir_prev == Direction::LEFT) return d == Direction::RIGHT;
					else if (dir_prev == Direction::RIGHT) return d == Direction::LEFT;
					else if (dir_prev == Direction::UP) return d == Direction::DOWN;
					else if (dir_prev == Direction::DOWN) return d == Direction::UP;
					else return false;
				}), 
			dir_possible.end());
		int room_id = 0;
		if (i < room_numbers[RoomType::NORMAL]) room_id = normal_room_ids[g_game_random.rand(0, normal_room_ids.size())];
		else room_id = goal_room_ids[g_game_random.rand(0, goal_room_ids.size())];
		Direction dir_now = dir_possible[g_game_random.rand(0, dir_possible.size())];
		Vector center_pos_next = center_pos;
		
		if (dir_now == Direction::RIGHT) {
			center_pos_next = center_pos + Vector(BLOCK_SIZE * DISTANCE_BETWEEN_ROOM, 0.0f);
		} else if (dir_now == Direction::UP) {
			center_pos_next = center_pos + Vector(0.0f, - BLOCK_SIZE * DISTANCE_BETWEEN_ROOM);
		} else if (dir_now == Direction::DOWN) {
			center_pos_next = center_pos + Vector(0.0f, BLOCK_SIZE * DISTANCE_BETWEEN_ROOM);
		} else throw;

		Vector room_pos = center_pos_next - m_room_data_set->get_room_data(room_id).m_tilemap->get_bounding_box().get_middle();
		Rectf bounding_box = Rectf(room_pos, m_room_data_set->get_room_data(room_id).m_tilemap->get_bounding_box().get_size());
		// calculating bridge's spawn pos and length
		Vector bridge_pos;
		int bridge_length = 0;
		if (dir_now == Direction::RIGHT) {
			bridge_pos.x = prev_bounding_box.get_right();
			bridge_pos.y = prev_bounding_box.get_middle().y - BRIDGE_SHIFT_VALUE;
			bridge_length = std::round((bounding_box.get_left() - prev_bounding_box.get_right()) / BLOCK_SIZE);
		} else if (dir_now == Direction::UP) {
			bridge_pos.x = bounding_box.get_middle().x - BRIDGE_SHIFT_VALUE;
			bridge_pos.y = bounding_box.get_bottom();
			bridge_length = std::round((prev_bounding_box.get_top() - bounding_box.get_bottom()) / BLOCK_SIZE);
		} else if (dir_now == Direction::DOWN) {
			bridge_pos.x = prev_bounding_box.get_middle().x - BRIDGE_SHIFT_VALUE;
			bridge_pos.y = prev_bounding_box.get_bottom();
			bridge_length = std::round((bounding_box.get_top() - prev_bounding_box.get_bottom()) / BLOCK_SIZE);
		} else throw;

		// hardcode wall and floor tile
		// TODO: fix later
		auto bridge_room_data = bridge_maker(bridge_length, dir_now, bridge_pos, 2, 1);
		level->add_room(std::make_unique<Room>(std::move(bridge_room_data)));

		auto room = std::make_unique<Room>(m_room_data_set->get_room_data(room_id).clone(room_pos));
		level->add_room(std::move(room));

		center_pos = center_pos_next;
		dir_prev = dir_now;
		prev_bounding_box = bounding_box;
	}
	return level;
}