#ifndef HEADER_QBSKR_OBJECT_MOVING_OBJECT_HPP
#define HEADER_QBSKR_OBJECT_MOVING_OBJECT_HPP

#include "collision/collision_listener.hpp"
#include "collision/collision_object.hpp"
#include "object/game_object.hpp"

class MovingObject : public GameObject, public CollisionListener {
	friend class Room;
	friend class CollisionSystem;

public:
	MovingObject();
	~MovingObject() override;

private:
	MovingObject(const MovingObject&) = delete;
	MovingObject& operator=(const MovingObject&) = delete;

protected:
	CollisionObject m_collision_object;

public:
	virtual void collision_solid(const CollisionHit& hit) override;
	virtual HitResponse collision(GameObject& other, const CollisionHit& hit) override;
	virtual void collision_tile(uint32_t tile_attributes) override;
	virtual bool listener_is_valid() const override;
	virtual int get_layer() const = 0;

public:
	Rectf get_bounding_box() const;

	void set_movement(const Vector& movement);
	Vector get_movement() const;
	
	Vector get_pos() const;
	void set_pos(const Vector& pos);

	CollisionGroup get_group() const;

	CollisionObject* get_collision_object();
	const CollisionObject* get_collision_object() const;
};

#endif