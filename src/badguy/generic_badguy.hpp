#ifndef HEADER_QBSKR_BADGUY_GENERIC_BADGUY_HPP
#define HEADER_QBSKR_BADGUY_GENERIC_BADGUY_HPP

#include "badguy/badguy.hpp"

class DrawingContext;
class Weapon;

class GenericBadGuy final : public BadGuy {
private:
	GenericBadGuy(const GenericBadGuy&) = delete;
	GenericBadGuy& operator=(const GenericBadGuy&) = delete;

private:
	std::unique_ptr<Weapon> m_weapon;

public:
	GenericBadGuy(const std::string& sprite_filename);
	GenericBadGuy(const Sprite* sprite);

public:
	static std::unique_ptr<GenericBadGuy> from_reader(const CrappyReaderData* crd);

public:
	virtual void draw(DrawingContext& drawing_context) override;
	virtual void active_update(float dt_sec);
	virtual void inactive_update(float dt_sec);
	virtual void try_change_state();
	virtual std::unique_ptr<BadGuy> clone(const Vector& pos) const override;
};

#endif