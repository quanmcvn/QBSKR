#ifndef HEADER_QBSKR_QBSKR_ROOM_DATA_HPP
#define HEADER_QBSKR_QBSKR_ROOM_DATA_HPP

#include <memory>
#include <vector>

#include "math/vector.hpp"

class CrappyReaderData;
class TileMap;
class Room;

enum class RoomType {
	START,
	BRIDGE,
	GOAL,

	NORMAL,
	LOOT
};

/**
 *  Class to store data of the room
 *  (what badguy can spawn, how many per turn, what map is it)
*/
class RoomData final {
public:
	friend class Room;
	friend class LevelData;

public:
	RoomData();
	~RoomData();

public:
	RoomData(const RoomData& other);

private:
	RoomData& operator=(const RoomData&) = delete;

private:
	Room* m_parent;
	RoomType m_type;
	std::unique_ptr<TileMap> m_tilemap;
	std::vector<uint32_t> m_badguys;
	int m_turns;
	int m_min_per_turn;
	int m_max_per_turn;
	bool m_loot_spawned;

public:
	// this constructor steal the tilemap given, sorry
	RoomData(RoomType type, std::unique_ptr<TileMap> tilemap, std::vector<uint32_t> badguys = std::vector<uint32_t>(),
	         int turns = 0, int min_per_turn = 0, int max_per_turn = 0);

public:
	static std::unique_ptr<RoomData> from_reader(const CrappyReaderData* crd);

private:
	// do update to the parent room
	void update();

public:
	void spawn_badguy();
	// is all badguy dead?
	bool is_turn_cleared() const;
	// is all badguy dead and all turn cleared?
	bool is_room_cleared() const;

public:
	std::unique_ptr<RoomData> clone(const Vector& pos) const;
};

RoomType string_to_room_type(const std::string& string);
std::string room_type_to_string(const RoomType& room_type);

#endif