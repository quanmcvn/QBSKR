#include "qbskr/room_data_set.hpp"

#include <sstream>

#include "qbskr/room_data.hpp"
#include "util/crappy_reader.hpp"
#include "util/log.hpp"

RoomDataSet::~RoomDataSet()
{
	m_room_datas.clear();
}

RoomDataSet::RoomDataSet(const std::string& filename) :
	m_room_datas()
{
	parse(filename);
}

void RoomDataSet::parse(const std::string& filename)
{
	CrappyReader cr(filename);

	while (cr.parse("room-data-set")) {}

	CrappyReaderData* crd = cr.get_root()->get_child("room-data-set");
	if (!crd) {
		std::ostringstream msg;
		msg << "File '" << filename << "' is not room-data-set file";
		throw std::runtime_error(msg.str());
	}

	for (const auto& child : crd->m_childs) {
		if (child->m_data == "room-data") parse_room_data(child);
	}
}

void RoomDataSet::parse_room_data(const CrappyReaderData* crd)
{
	uint32_t id;
	if (!crd->get("id", id)) {
		throw std::runtime_error("Missing id in room-data");
	}

	std::unique_ptr<RoomData> room_data = RoomData::from_reader(crd);

	add_room_data(id, std::move(room_data));
}

void RoomDataSet::add_room_data(uint32_t id, std::unique_ptr<RoomData> room_data)
{
	if (id >= static_cast<uint32_t>(m_room_datas.size())) {
		m_room_datas.resize(id + 1);
	}

	if (m_room_datas[id]) {
		log_warning << "Room data id " << id << " redefined" << std::endl;
	} else {
		m_room_datas[id] = std::move(room_data);
	}
}

const RoomData& RoomDataSet::get_room_data(const uint32_t id) const
{
	if (id >= static_cast<uint32_t>(m_room_datas.size())) {
		log_warning << "Invalid room data id: " << id << std::endl;
		return *m_room_datas[0];
	} else {
		RoomData* room_data = m_room_datas[id].get();
		if (room_data) {
			return *room_data;
		} else {
			log_warning << "Invalid room data id: " << id << std::endl;
			return *m_room_datas[0];
		}
	}
}