#include "qbskr/room_data.hpp"

#include <sstream>

#include "badguy/badguy_set.hpp"
#include "badguy/badguy.hpp"
#include "badguy/balbusour.hpp"
#include "math/random.hpp"
#include "object/chest.hpp"
#include "object/tile_map.hpp"
#include "qbskr/room.hpp"
#include "util/crappy_reader_data.hpp"
#include "util/log.hpp"

RoomType string_to_room_type(const std::string& string)
{
	if (string == "start") return RoomType::START;
	if (string == "bridge") return RoomType::BRIDGE;
	if (string == "goal") return RoomType::GOAL;
	if (string == "normal") return RoomType::NORMAL;
	if (string == "loot") return RoomType::LOOT;
	std::ostringstream msg;
	msg << "Unknown room type: '" << string << "'";
	throw std::runtime_error(msg.str());
}

std::string room_type_to_string(const RoomType& room_type)
{
	if (room_type == RoomType::START) return "start";
	if (room_type == RoomType::BRIDGE) return "bridge";
	if (room_type == RoomType::GOAL) return "goal";
	if (room_type == RoomType::NORMAL) return "normal";
	if (room_type == RoomType::LOOT) return "loot";
	std::ostringstream msg;
	msg << "Unknown room type: '" << static_cast<int>(room_type) << "'";
	throw std::runtime_error(msg.str());
}

RoomData::RoomData() :
	m_type(),
	m_tilemap(),
	m_badguys(),
	m_turns(0),
	m_min_per_turn(0),
	m_max_per_turn(0),
	m_loot_spawned(false)
{}

RoomData::~RoomData()
{
	m_tilemap.release();
	m_badguys.clear();
}

RoomData::RoomData(const RoomData& other) :
	m_type(other.m_type),
	m_tilemap(other.m_tilemap->clone(other.m_tilemap->get_offset())),
	m_badguys(other.m_badguys),
	m_turns(other.m_turns),
	m_min_per_turn(other.m_min_per_turn),
	m_max_per_turn(other.m_max_per_turn),
	m_loot_spawned(false)
{}

RoomData::RoomData(RoomType type, std::unique_ptr<TileMap> tilemap, std::vector<uint32_t> badguys, 
                   int turns, int min_per_turn, int max_per_turn) :
	m_type(type),
	m_tilemap(std::move(tilemap)),
	m_badguys(std::move(badguys)),
	m_turns(turns),
	m_min_per_turn(min_per_turn),
	m_max_per_turn(max_per_turn),
	m_loot_spawned(false)
{}

std::unique_ptr<RoomData> RoomData::from_reader(const CrappyReaderData* crd)
{
	auto room_data = std::make_unique<RoomData>();

	std::string room_type;
	if (crd->get("room-type", room_type)) {
		room_data->m_type = string_to_room_type(room_type);
	} else {
		throw std::runtime_error("No room type in room data");
	}

	crd->get("badguys", room_data->m_badguys);
	crd->get("turns", room_data->m_turns);
	crd->get("min-per-turn", room_data->m_min_per_turn);
	crd->get("max-per-turn", room_data->m_max_per_turn);

	std::string tilemap_filename;
	if (crd->get("tilemap-filename", tilemap_filename)) {
		room_data->m_tilemap = TileMap::from_file(crd->m_parent_path + tilemap_filename);
	} else {
		throw std::runtime_error("No tilemap filename in room data");
	}

	return room_data;
}

void RoomData::update()
{
	switch (m_type) {
		case RoomType::START:
		case RoomType::BRIDGE:
		case RoomType::GOAL:
			// not doing anything
			break;

		case RoomType::NORMAL: 
			if (!m_parent->get_players().empty() && !is_room_cleared()) {
				m_parent->close_room();
				spawn_badguy();
			}
			if (is_room_cleared()) m_parent->open_room();
			break;

		case RoomType::LOOT:
			if (!m_loot_spawned) {
				m_parent->add<Chest>(m_parent->get_bounding_box().get_middle() - Vector(10, 0));
				m_loot_spawned = true;
			}
			break;

		default:
			break;
	}
}

void RoomData::spawn_badguy()
{
	if (m_turns <= 0) return;
	if (m_badguys.empty()) return;
	if (!is_turn_cleared()) return;

	-- m_turns;
	int badguy_spawns = g_game_random.rand_inclusive(m_min_per_turn, m_max_per_turn);
	
	// spawn badguy randomly
	// try at most 100 times
	// if fail then give up
	// potentially bad
	const int MAX_TRIES = 100;
	const Rectf bounding_box = m_parent->get_bounding_box();
	for (int i = 0; i < badguy_spawns; ++ i) {
		int spawn_id = m_badguys[g_game_random.rand(0, m_badguys.size())];
		Vector spawn_pos;
		Rectf badguy_bounding_box = BadGuySet::current()->get_badguy(spawn_id).get_bounding_box();
		
		if (dynamic_cast<const Balbusour*>(&BadGuySet::current()->get_badguy(spawn_id))) {
			// special spawn rule for Balbusour: spawn in the middle of the room
			spawn_pos = bounding_box.get_middle() - (badguy_bounding_box.get_middle() - badguy_bounding_box.p1());
		} else {
			for (int tries = 0; tries < MAX_TRIES; ++ tries) {
				spawn_pos = Vector(g_game_random.randf(bounding_box.get_left(), bounding_box.get_right()), g_game_random.randf(bounding_box.get_top(), bounding_box.get_bottom()));
				badguy_bounding_box.set_pos(spawn_pos);
				if (m_parent->inside(badguy_bounding_box) && m_parent->is_free_of_tiles(badguy_bounding_box)) break;
			}

			if (!(m_parent->inside(badguy_bounding_box) && m_parent->is_free_of_tiles(badguy_bounding_box))) {
				log_warning << "Can't spawn badguy id '" << spawn_id << "'" << std::endl;
				continue;
			}
		}

		m_parent->add_object(BadGuySet::current()->get_badguy(spawn_id).clone(spawn_pos));
	}

	m_parent->flush_game_objects();
}

bool RoomData::is_turn_cleared() const
{
	// if all of gameobjects are either (not badguy or (is badguy and dead))
	// = if all of badguy (if cast-down-able) is dead
	// note that can't use get_objects_by_type_index()
	// since it use typeid, and typeid is exact match, not considering inheritance
	return std::all_of(m_parent->get_objects().begin(), m_parent->get_objects().end(), 
		[] (const std::unique_ptr<GameObject>& object) {
			auto badguy_ptr = dynamic_cast<BadGuy*>(object.get());
			if (!badguy_ptr) return true;
			return badguy_ptr->is_dead();
		}
	);
}

bool RoomData::is_room_cleared() const { return (m_turns <= 0) && is_turn_cleared(); }

std::unique_ptr<RoomData> RoomData::clone(const Vector& pos) const
{
	auto room_data = std::make_unique<RoomData>(*this);
	room_data->m_tilemap->set_offset(pos);
	return room_data;
}