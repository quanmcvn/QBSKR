#ifndef HEADER_QBSKR_OBJECT_PLAYER_HPP
#define HEADER_QBSKR_OBJECT_PLAYER_HPP

#include "object/moving_object.hpp"
#include "object/physic.hpp"
#include "object/direction.hpp"
#include "sprite/sprite_ptr.hpp"
#include "util/timer.hpp"
#include "weapon/weapon.hpp"

class Controller;

class Player final : public MovingObject {
public:
	~Player() override;

public:
	Player(const Player& other);

private:
	Player& operator=(const Player&) = delete;

public:
	Player(int player_id, uint32_t weapon_id);

private:
	int m_id;
	const Controller* m_controller;
	Direction m_direction;
	Physic m_physic;
	SpritePtr m_sprite;
	std::unique_ptr<Weapon> m_weapon;
	int m_max_health;
	int m_health;
	bool m_die;
	// if hit this frame
	std::optional<int> m_hit_damage;
	Timer m_die_animation_timer;
	bool m_is_switching_weapon;

public:
	virtual void update(float dt_sec) override;
	virtual void draw(DrawingContext& drawing_context) override;
	virtual void collision_solid(const CollisionHit& hit) override;
	virtual HitResponse collision(GameObject& other, const CollisionHit& hit) override;
	virtual void collision_tile(uint32_t tile_attributes) override;
	virtual int get_layer() const override;

public:
	int get_id() const;
	void set_id(int id);

	bool is_dying() const;

private:
	void handle_input();
	void handle_movement_input();
	void handle_weapon();

public:
	std::unique_ptr<Player> clone() const;
};

#endif