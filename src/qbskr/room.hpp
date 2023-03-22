#ifndef HEADER_QBSKR_QBSKR_ROOM_HPP
#define HEADER_QBSKR_QBSKR_ROOM_HPP

#include "object/game_object_manager.hpp"

class CollisionSystem;

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

	virtual bool before_object_add(GameObject& object) override;
	virtual void before_object_remove(GameObject& object) override;
};

#endif