#ifndef HEADER_QBSKR_QBSKR_LEVEL_DATA_HPP
#define HEADER_QBSKR_QBSKR_LEVEL_DATA_HPP

#include <map>

#include "qbskr/room_data_set.hpp"
#include "qbskr/room_data.hpp"

class DrawingContext;
class Level;

/**
 *  Class to hold level's data
 *  (how many room per room type)
 *  Make a level base on it's data
*/
class LevelData final {
private:
	LevelData(const LevelData&) = delete;
	LevelData& operator=(const LevelData&) = delete;

private:
	// holds info
	// rand(min, max) numbers of roomtype
	std::map<RoomType, std::pair<int, int>> m_rooms_number_random_info;
	// holds info
	// which ids has room type
	std::map<RoomType, std::vector<uint32_t>> m_rooms_info;
	std::unique_ptr<RoomDataSet> m_room_data_set;
	std::string m_next_level;

public:
	LevelData(const std::string& filename);

public:
	// make a level based on data
	std::unique_ptr<Level> make_level() const;
};

#endif