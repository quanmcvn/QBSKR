#include "qbskr/level.hpp"

#include "object/camera.hpp"
#include "object/player.hpp"
#include "qbskr/game_session.hpp"
#include "qbskr/player_status.hpp"
#include "qbskr/savegame.hpp"
#include "video/drawing_context.hpp"

Level::Level() :
	m_rooms(),
	m_next_level()
{}

Level::~Level()
{
	m_rooms.clear();
}

void Level::update(float dt_sec)
{
	// bool has_active_room = std::any_of(
	// 	m_rooms.begin(), 
	// 	m_rooms.end(), 
	// 	[] (const std::unique_ptr<Room>& room) {
	// 		return room.get() == Room::current();
	// 	}
	// );

	// if (!has_active_room) return;
	const auto& players = Room::get().get_players();
	if (!players.empty()) {
		const auto& player_ptr = players[0];
		if (player_ptr) {
			for (const auto& room : m_rooms) {
				if (room.get() != Room::current()) {
					// only check first player
					// sorry multiplay enjoyer
					if (room->inside(player_ptr->get_bounding_box())) {
						Room::get().clone_camera_and_players_to(*room);
						room->activate();
						break;
					}
				}
			}
		}
	}
	// Room::get().update(dt_sec);
	for (const auto& room : m_rooms) room->update(dt_sec);
}

void Level::draw(DrawingContext& drawing_context)
{
	for (const auto& room : m_rooms) {
		room->draw(drawing_context);
	}
}

void Level::add_room(std::unique_ptr<Room> room)
{
	m_rooms.push_back(std::move(room));
}

void Level::activate()
{
	for (const auto& room : m_rooms) {
		if (room->get_room_type() == RoomType::START) {
			room->add_object(GameSession::current()->get_savegame().get_status().get_player());
			room->add<Camera>();
			room->flush_game_objects();
			room->activate();
		}
		if (room->get_room_type() != RoomType::BRIDGE) {
			room->open_room();
		}
	}
}

std::string Level::get_next_level() const { return m_next_level; }
void Level::set_next_level(const std::string& next_level) { m_next_level = next_level; }