#ifndef HEADER_QBSKR_BADGUY_BALBUSOUR_HPP
#define HEADER_QBSKR_BADGUY_BALBUSOUR_HPP

#include "badguy/badguy.hpp"
#include "util/timer.hpp"

/**
 *  The first (and maybe final) boss of the game
 *  In case someone ask, its name is from Bulbasour (I just swap 'a' and 'u' :D)
*/
class Balbusour final : public BadGuy {
public:
	Balbusour();

private:
	Balbusour(const Balbusour&) = delete;
	Balbusour& operator=(const Balbusour&) = delete;

private:
	int m_full_health;
	// timers for attack
	// I don't have better design

	Timer m_attack1_cooldown;

	Timer m_attack2_cooldown;
	Timer m_attack2_attack_time;
	Timer m_attack2_attack_freq;

public:
	static std::unique_ptr<Balbusour> from_reader(const CrappyReaderData* crd);

public:
	virtual void draw(DrawingContext& drawing_context) override;
	virtual HitResponse collision(GameObject& other, const CollisionHit& hit) override;
	virtual void active_update(float dt_sec) override;
	virtual void inactive_update(float dt_sec) override;
	virtual void try_change_state() override;
	virtual std::unique_ptr<BadGuy> clone(const Vector& pos) const override;

private:
	void do_attack1();
	void do_attack2();
};

#endif