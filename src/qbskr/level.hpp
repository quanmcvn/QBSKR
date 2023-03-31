#ifndef HEADER_QBSKR_QBSKR_LEVEL_HPP
#define HEADER_QBSKR_QBSKR_LEVEL_HPP

#include "qbskr/room.hpp"

class Level final {
public:
	Level();
	~Level();

private:
	Level(const Level&) = delete;
	Level& operator=(const Level&) = delete;

private:
	std::vector<std::unique_ptr<Room>> m_rooms;

public:
	void update(float dt_sec);
	void draw(DrawingContext& drawing_context);
	// steal room
	void add_room(std::unique_ptr<Room> room);
	// activate the 'start' room
	void activate();
};

#endif