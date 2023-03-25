#ifndef HEADER_QBSKR_BADGUY_BADGUY_HPP
#define HEADER_QBSKR_BADGUY_BADGUY_HPP

#include "object/direction.hpp"
#include "object/moving_sprite.hpp"
#include "object/physic.hpp"

class DrawingContext;

class BadGuy : public MovingSprite {
private:
	BadGuy(const BadGuy&) = delete;
	BadGuy& operator=(const BadGuy&) = delete;

protected:
	enum State {
		STATE_INACTIVE,
		STATE_ACTIVE
	};

protected:
	Physic m_physic;
	Direction m_direction;
	State m_state;

public:
	BadGuy(const std::string& sprite_filename);
	BadGuy(const Sprite* sprite);

public:
	virtual void update(float dt_sec) override;
	virtual void draw(DrawingContext& drawing_context) override;
	static std::string class_name();
	virtual std::string get_class_name() const;
	virtual void active_update(float dt_sec) = 0;
	virtual void inactive_update(float dt_sec) = 0;
	virtual void try_change_state() = 0;
	// assume the cloned badguy is put into Room
	virtual std::unique_ptr<BadGuy> clone(const Vector& pos) const = 0;

public:
	// (potentialy) expensive
	bool can_see_player() const;
};

#endif