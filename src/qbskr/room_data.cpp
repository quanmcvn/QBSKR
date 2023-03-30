#include "qbskr/room_data.hpp"

#include <sstream>

#include "util/crappy_reader_data.hpp"

namespace {
	RoomType string_to_room_type(const std::string& string)
	{
		if (string == "start") return START;
		if (string == "bridge") return BRIDGE;
		if (string == "goal") return GOAL;
		if (string == "normal") return NORMAL;
		std::ostringstream msg;
		msg << "Unknown room type: '" << string << "'";
		throw std::runtime_error(msg.str());
	}
}

RoomData::RoomData() :
	m_type(),
	m_tilemap(),
	m_badguys(),
	m_turns(0),
	m_min_per_turn(0),
	m_max_per_turn(0)
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
	m_max_per_turn(other.m_max_per_turn)
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

std::unique_ptr<RoomData> RoomData::clone(const Vector& pos) const
{
	auto room_data = std::make_unique<RoomData>(*this);
	room_data->m_tilemap->set_offset(pos);
	return room_data;
}