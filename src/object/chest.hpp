#ifndef HEADER_QBSKR_OBJECT_CHEST_HPP
#define HEADER_QBSKR_OBJECT_CHEST_HPP

#include "object/moving_sprite.hpp"

class Chest final : public MovingSprite {
public:
	~Chest() override;

private:
	Chest(const Chest&) = delete;
	Chest& operator=(const Chest&) = delete;

private:
	bool m_open;

public:
	Chest(const Vector& pos);

public:
	virtual void draw(DrawingContext& drawing_context) override;
	virtual HitResponse collision(GameObject& other, const CollisionHit& hit) override;
	void try_open();
};

#endif