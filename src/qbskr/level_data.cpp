#include "qbskr/level_data.hpp"

#include <sstream>

#include "math/random.hpp"
#include "math/rectf.hpp"
#include "object/direction.hpp"
#include "object/tile_map.hpp"
#include "qbskr/constants.hpp"
#include "qbskr/level.hpp"
#include "util/crappy_reader.hpp"

namespace {
	// distance between room, in tiles
	const int DISTANCE_BETWEEN_ROOM = 30;
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

	Vector get_next_center_pos(const Vector& center_pos, Direction direction)
	{
		if (direction == Direction::LEFT) {
			return center_pos + Vector(-BLOCK_SIZE * DISTANCE_BETWEEN_ROOM, 0.0f);
		} else if (direction == Direction::RIGHT) {
			return center_pos + Vector(BLOCK_SIZE * DISTANCE_BETWEEN_ROOM, 0.0f);
		} else if (direction == Direction::UP) {
			return center_pos + Vector(0.0f, - BLOCK_SIZE * DISTANCE_BETWEEN_ROOM);
		} else if (direction == Direction::DOWN) {
			return center_pos + Vector(0.0f, BLOCK_SIZE * DISTANCE_BETWEEN_ROOM);
		} else throw;
	}

	std::tuple<Vector, int> get_bridge_pos_and_length(const Rectf& prev_bounding_box, const Rectf& bounding_box, Direction direction)
	{
		Vector bridge_pos;
		int bridge_length = 0;
		if (direction == Direction::LEFT) {
			bridge_pos.x = bounding_box.get_right();
			bridge_pos.y = bounding_box.get_middle().y - BRIDGE_SHIFT_VALUE;
			bridge_length = std::round((prev_bounding_box.get_left() - bounding_box.get_right()) / BLOCK_SIZE);
		} else if (direction == Direction::RIGHT) {
			bridge_pos.x = prev_bounding_box.get_right();
			bridge_pos.y = prev_bounding_box.get_middle().y - BRIDGE_SHIFT_VALUE;
			bridge_length = std::round((bounding_box.get_left() - prev_bounding_box.get_right()) / BLOCK_SIZE);
		} else if (direction == Direction::UP) {
			bridge_pos.x = bounding_box.get_middle().x - BRIDGE_SHIFT_VALUE;
			bridge_pos.y = bounding_box.get_bottom();
			bridge_length = std::round((prev_bounding_box.get_top() - bounding_box.get_bottom()) / BLOCK_SIZE);
		} else if (direction == Direction::DOWN) {
			bridge_pos.x = prev_bounding_box.get_middle().x - BRIDGE_SHIFT_VALUE;
			bridge_pos.y = prev_bounding_box.get_bottom();
			bridge_length = std::round((bounding_box.get_top() - prev_bounding_box.get_bottom()) / BLOCK_SIZE);
		} else throw;

		return std::tuple<Vector, int>(bridge_pos, bridge_length);
	}

	
}

LevelData::LevelData(const std::string& filename) :
	m_rooms_number_random_info(),
	m_rooms_info(),
	m_room_data_set(),
	m_next_level()
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

	if (!crd->get("next-level", m_next_level)) {
		throw std::runtime_error("No next-level in level file");
	}

	for (size_t i = 0; i < m_room_data_set->m_room_datas.size(); ++ i) {
		const auto& room_data = m_room_data_set->m_room_datas[i];
		if (!room_data.get()) continue;
		m_rooms_info[room_data->m_type].push_back(i);
	}
}

std::unique_ptr<Level> LevelData::make_level() const
{
	auto level = std::make_unique<Level>();
	// the result of random 'how many room of this type?'
	std::map<RoomType, int> room_numbers;
	for (const auto& [room_type, rand_lr] : m_rooms_number_random_info) {
		room_numbers[room_type] = g_game_random.rand_inclusive(rand_lr.first, rand_lr.second);
	}

	// spawn 'start' room
	// then chain spawn 'normal' room
	// spawn additional room in the way (if the level has)
	// then spawn 'goal' room at the end of the chain
	// hardcoded a level must have 1 start and 1 goal room

	Direction dir_prev = Direction::AUTO;
	const auto& start_room_ids = m_rooms_info.at(RoomType::START);
	const auto& normal_room_ids = m_rooms_info.at(RoomType::NORMAL);
	const auto& goal_room_ids = m_rooms_info.at(RoomType::GOAL);
	std::vector<uint32_t> additional_room_ids;
	for (const auto& [roomtype, values] : m_rooms_info) {
		if (roomtype == RoomType::START || roomtype == RoomType::NORMAL || roomtype == RoomType::GOAL) continue;
		// additional will be generate differently from 'normal' and 'goal'
		// first random the ids here
		for (int i = 0; i < room_numbers.at(roomtype); ++ i) {
			additional_room_ids.push_back(values[g_game_random.rand(0, values.size())]);
		}
	}
	// tries to random by shuffle and take the back()
	std::random_shuffle(additional_room_ids.begin(), additional_room_ids.end());

	Room* prev_room = nullptr;
	Rectf prev_bounding_box;
	// spawn room in such a way that its center is center_pos
	Vector center_pos(0.0f, 0.0f);
	{
		int start_room_id = start_room_ids[g_game_random.rand(0, start_room_ids.size())];
		Vector spawn_room_pos = center_pos - m_room_data_set->get_room_data(start_room_id).m_tilemap->get_bounding_box().get_middle();
		prev_bounding_box = Rectf(spawn_room_pos, m_room_data_set->get_room_data(start_room_id).m_tilemap->get_bounding_box().get_size());
		auto start_room = std::make_unique<Room>(m_room_data_set->get_room_data(start_room_id).clone(spawn_room_pos));
		prev_room = start_room.get();
		level->add_room(std::move(start_room));
	}
	// note that there is no Direction::LEFT
	// intentional going right
	const std::vector<Direction> directions = {Direction::RIGHT, Direction::UP, Direction::DOWN};
	// also no Direction::RIGHT
	// intentional
	const std::vector<Direction> additional_directions = {Direction::LEFT, Direction::UP, Direction::DOWN};
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
					if (d == Direction::LEFT) return dir_prev == Direction::RIGHT;
					else if (d == Direction::RIGHT) return dir_prev == Direction::LEFT;
					else if (d == Direction::UP) return dir_prev == Direction::DOWN;
					else if (d == Direction::DOWN) return dir_prev == Direction::UP;
					else return true;
				}), 
			dir_possible.end()
		);

		Direction dir_now = dir_possible[g_game_random.rand(0, dir_possible.size())];

		std::vector<Direction> additional_dir_possible = additional_directions;
		// x2 verbose way of removing easy to spot conflict direction
		additional_dir_possible.erase(
			std::remove_if(
				additional_dir_possible.begin(),
				additional_dir_possible.end(),
				[dir_prev, dir_now](Direction d) {
					if (d == Direction::LEFT) return dir_prev == Direction::RIGHT;
					// ban the Direction::RIGHT right away (eventhough there is not a chance of it being here)
					else if (d == Direction::RIGHT) return true;
					else if (d == Direction::UP) return dir_prev == Direction::DOWN || dir_now == Direction::UP;
					else if (d == Direction::DOWN) return dir_prev == Direction::UP || dir_now == Direction::DOWN;
					else return true;
				}), 
			additional_dir_possible.end()
		);

		if (!additional_room_ids.empty()) {
			int additionals = g_game_random.rand(0, std::min(static_cast<int>(additional_dir_possible.size()), 2));
			// tries to random by shuffle and take the back()
			std::random_shuffle(additional_dir_possible.begin(), additional_dir_possible.end());
			for (int i = 0; i < additionals; ++ i) {
				Direction dir_addition = additional_dir_possible.back(); additional_dir_possible.pop_back();
				int room_id_addition = additional_room_ids.back(); additional_room_ids.pop_back();

				auto additional_room = make_room(level.get(), prev_room, prev_bounding_box, center_pos, dir_addition, room_id_addition);
				level->add_room(std::move(additional_room));
			}
		}

		int room_id = 0;
		if (i < room_numbers[RoomType::NORMAL]) room_id = normal_room_ids[g_game_random.rand(0, normal_room_ids.size())];
		else room_id = goal_room_ids[g_game_random.rand(0, goal_room_ids.size())];

		auto normal_room = make_room(level.get(), prev_room, prev_bounding_box, center_pos, dir_now, room_id);
		prev_room = normal_room.get();
		
		level->add_room(std::move(normal_room));

		center_pos = get_next_center_pos(center_pos, dir_now);
		dir_prev = dir_now;
		prev_bounding_box = prev_room->get_bounding_box();
	}

	level->set_next_level(m_next_level);

	return level;
}

std::unique_ptr<Room> LevelData::make_room(Level* level, Room* prev_room, const Rectf& prev_bounding_box, const Vector& center_pos, Direction direction, int room_id) const
{
	Vector center_pos_next = get_next_center_pos(center_pos, direction);
	Vector room_pos = center_pos_next - m_room_data_set->get_room_data(room_id).m_tilemap->get_bounding_box().get_middle();
	Rectf bounding_box = Rectf(room_pos, m_room_data_set->get_room_data(room_id).m_tilemap->get_bounding_box().get_size());
	// calculating bridge's spawn pos and length
	auto[bridge_pos, bridge_length] = get_bridge_pos_and_length(prev_bounding_box, bounding_box, direction);

	// hardcode wall and floor tile
	// TODO: fix later
	auto bridge_room_data = bridge_maker(bridge_length, direction, bridge_pos, 2, 1);
	level->add_room(std::make_unique<Room>(std::move(bridge_room_data)));

	auto room = std::make_unique<Room>(m_room_data_set->get_room_data(room_id).clone(room_pos));

	if (direction == Direction::LEFT) {
		prev_room->room_left = true;
		room->room_right = true;
	} else if (direction == Direction::RIGHT) {
		prev_room->room_right = true;
		room->room_left = true;
	} else if (direction == Direction::UP) {
		prev_room->room_up = true;
		room->room_down = true;
	} else if (direction == Direction::DOWN) {
		prev_room->room_down = true;
		room->room_up = true;
	} else throw;

	return room;
}