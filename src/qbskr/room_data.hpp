#ifndef HEADER_QBSKR_QBSKR_ROOM_DATA_HPP
#define HEADER_QBSKR_QBSKR_ROOM_DATA_HPP

#include <memory>

#include "object/tile_map.hpp"

class CrappyReaderData;

enum RoomType {
	START,
	BRIDGE,
	GOAL,

	NORMAL
};

class RoomData final {
public:
	friend class Room;

public:
	RoomData();
	~RoomData();

public:
	RoomData(const RoomData& other);

private:
	RoomData& operator=(const RoomData&) = delete;

public:
	static std::unique_ptr<RoomData> from_reader(const CrappyReaderData* crd);

private:
	RoomType m_type;
	std::unique_ptr<TileMap> m_tilemap;
	std::vector<uint32_t> m_badguys;
	int m_turns;
	int m_min_per_turn;
	int m_max_per_turn;

public:
	std::unique_ptr<RoomData> clone(const Vector& pos) const;
};

#endif