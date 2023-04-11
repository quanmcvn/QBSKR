#ifndef HEADER_QBSKR_BADGUY_GENERIC_BADGUY_HPP
#define HEADER_QBSKR_BADGUY_GENERIC_BADGUY_HPP

#include "badguy/badguy.hpp"
#include "util/timer.hpp"

class DrawingContext;
class Weapon;

/**
 *  Class for generic badguy (no special mechanic)
 *  The design here is quite same with weapon (but maybe can't apply that design to badguy)
 *  But "If it ain't broke don't fix it" :D
*/
class GenericBadGuy final : public BadGuy {
public:
	~GenericBadGuy();

private:
	GenericBadGuy(const GenericBadGuy&) = delete;
	GenericBadGuy& operator=(const GenericBadGuy&) = delete;

private:
	std::unique_ptr<Weapon> m_weapon;
	Timer m_attack_timer;
	float m_attack_chance;
	Timer m_die_animation_timer;
	Timer m_knockback_animation_timer;

public:
	GenericBadGuy(const std::string& sprite_filename);

public:
	static std::unique_ptr<GenericBadGuy> from_reader(const CrappyReaderData* crd);

public:
	virtual void update(float dt_sec) override;
	virtual void draw(DrawingContext& drawing_context) override;
	virtual void collision_solid(const CollisionHit& hit) override;
	virtual HitResponse collision(GameObject& other, const CollisionHit& hit) override;
	virtual void active_update(float dt_sec) override;
	virtual void inactive_update(float dt_sec) override;
	virtual void try_change_state() override;
	virtual std::unique_ptr<BadGuy> clone(const Vector& pos) const override;

private:
	void wander();
};

#endif