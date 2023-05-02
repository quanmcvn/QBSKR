#ifndef HEADER_QBSKR_QBSKR_ROOM_DATA_SET_HPP
#define HEADER_QBSKR_QBSKR_ROOM_DATA_SET_HPP

#include <memory>
#include <vector>

#include "util/currenton.hpp"

class CrappyReaderData;
class RoomData;

/**
 *  Class to hold RoomData for cloning
*/
class RoomDataSet final {
private:
	friend class LevelData;

public:
	~RoomDataSet();

private:
	RoomDataSet(const RoomDataSet&) = delete;
	RoomDataSet& operator=(const RoomDataSet&) = delete;

private:
	std::vector<std::unique_ptr<RoomData>> m_room_datas;

public:
	RoomDataSet(const std::string& filename);

private:
	void parse(const std::string& filename);
	void parse_room_data(const CrappyReaderData* crd);

public:
	void add_room_data(uint32_t id, std::unique_ptr<RoomData> room_data);
	const RoomData& get_room_data(const uint32_t id) const;
};

#endif