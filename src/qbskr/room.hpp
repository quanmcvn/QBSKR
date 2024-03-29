#ifndef HEADER_QBSKR_QBSKR_ROOM_HPP
#define HEADER_QBSKR_QBSKR_ROOM_HPP

#include "object/game_object_manager.hpp"
#include "object/tile.hpp"
#include "qbskr/room_data.hpp"

class Camera;
class CollisionSystem;
class Player;
class Rectf;

/**
 *  Room is an indivisible chunk where badguy spawn
 *  A level consists of a whole bunch of room
*/
class Room final : public GameObjectManager {
public:
	friend class CollisionSystem;
	friend class LevelData;

private:
	Room() = delete;

public:
	~Room() override;

private:
	Room(const Room&) = delete;
	Room& operator=(const Room&) = delete;

private:
	static Room* s_current;

	std::unique_ptr<CollisionSystem> m_collision_system;
	std::unique_ptr<RoomData> m_room_data;

	// use exclusively in changing fence and floor tile
	bool room_left;
	bool room_right;
	bool room_up;
	bool room_down;

public:
	Room(std::unique_ptr<RoomData> room_data);

public:
	static Room& get();
	static Room* current();

public:
	void update(float dt_sec);
	void draw(DrawingContext& drawing_context);

	void activate();
	void deactivate();

public:
	virtual bool before_object_add(GameObject& object) override;
	virtual void before_object_remove(GameObject& object) override;

public:
	bool is_free_of_tiles(const Rectf& rect, uint32_t tiletype = Tile::SOLID) const;
	bool free_line_of_sight(const Vector& line_start, const Vector& line_end) const;
	bool can_see_player(const Vector& eye) const;
	bool inside(const Rectf& rect) const;

public:
	Rectf get_bounding_box() const;
	Player* get_nearest_player(const Vector& pos) const;
	Camera& get_camera() const;
	std::vector<Player*> get_players() const;
	void spawn_badguy();
	// is all badguy dead?
	bool is_turn_cleared() const;
	// is all badguy dead and all turn cleared?
	bool is_room_cleared() const;
	RoomType get_room_type() const;
	// misleading name
	// clone camera and players to other room and remove
	// can't move since move is hard (can't just std::move() and just leave it there)
	void clone_camera_and_players_to(Room& other);
	// misleading name,
	// game play-wise
	void close_room();
	void open_room();
};

#endif