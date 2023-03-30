#ifndef HEADER_QBSKR_QBSKR_ROOM_HPP
#define HEADER_QBSKR_QBSKR_ROOM_HPP

#include "object/game_object_manager.hpp"
#include "object/tile.hpp"
#include "qbskr/room_data.hpp"

class Camera;
class CollisionSystem;
class Player;
class Rectf;

class Room final : public GameObjectManager {
public:
	friend class CollisionSystem;

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
	Rectf get_bounding_box() const;
	Player* get_nearest_player(const Vector& pos) const;
	Camera& get_camera() const;
	std::vector<Player*> get_players() const;
	void spawn_badguy();
	// is all badguy dead?
	bool is_turn_cleared() const;
};

#endif