#ifndef HEADER_QBSKR_QBSKR_LEVEL_HPP
#define HEADER_QBSKR_QBSKR_LEVEL_HPP

#include "qbskr/room.hpp"

/**
 *  A level holds whole bunch of room
*/
class Level final {
public:
	Level();
	~Level();

private:
	Level(const Level&) = delete;
	Level& operator=(const Level&) = delete;

private:
	std::vector<std::unique_ptr<Room>> m_rooms;
	std::string m_next_level;

public:
	void update(float dt_sec);
	void draw(DrawingContext& drawing_context);
	// steal room
	void add_room(std::unique_ptr<Room> room);
	// activate the 'start' room
	void activate();
	std::string get_next_level() const;
	void set_next_level(const std::string& next_level);
};

#endif