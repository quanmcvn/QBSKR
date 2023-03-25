#ifndef HEADER_QBSKR_QBSKR_ROOM_HPP
#define HEADER_QBSKR_QBSKR_ROOM_HPP

#include "object/game_object_manager.hpp"
#include "object/tile.hpp"

class CollisionSystem;
class Player;
class Rectf;

class Room final : public GameObjectManager {
public:
	friend class CollisionSystem;

private:
	Room(const Room&) = delete;
	Room& operator=(const Room&) = delete;

public:
	Room();
	~Room() override;

private:
	static Room* s_current;

	std::unique_ptr<CollisionSystem> m_collision_system;

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
};

#endif