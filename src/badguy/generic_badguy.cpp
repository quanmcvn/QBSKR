#include "badguy/generic_badguy.hpp"

#include <stdexcept>

#include "qbskr/room.hpp"
#include "util/crappy_reader_data.hpp"
#include "weapon/weapon.hpp"
#include "weapon/weapon_set.hpp"

GenericBadGuy::GenericBadGuy(const std::string& sprite_filename) :
	BadGuy(sprite_filename)
{}

GenericBadGuy::GenericBadGuy(const Sprite* sprite) :
	BadGuy(sprite)
{}

std::unique_ptr<GenericBadGuy> GenericBadGuy::from_reader(const CrappyReaderData* crd)
{
	uint32_t weapon_id;
	if (!crd->get("weapon-id", weapon_id)) {
		throw std::runtime_error("Missing weapon-id in badguy-specific");
	}

	std::string sprite_filename;
	if (!crd->get("sprite-filename", sprite_filename)) {
		throw std::runtime_error("Missing sprite-filename in badguy-specific");
	}

	auto badguy = std::make_unique<GenericBadGuy>(crd->m_parent_path + sprite_filename);
	badguy->m_weapon = WeaponSet::current()->get_weapon(weapon_id).clone(badguy.get());

	return badguy;
}

void GenericBadGuy::draw(DrawingContext& drawing_context)
{
	if (m_weapon) m_weapon->draw(drawing_context);

	BadGuy::draw(drawing_context);
}

void GenericBadGuy::active_update(float /* dt_sec */)
{
	// NYI
}

void GenericBadGuy::inactive_update(float /* dt_sec */)
{
	// NYI
}

void GenericBadGuy::try_change_state()
{
	if (can_see_player()) {
		m_state = STATE_ACTIVE;
	} else {
		m_state = STATE_INACTIVE;
	}
}

std::unique_ptr<BadGuy> GenericBadGuy::clone(const Vector& pos) const
{
	auto badguy = std::make_unique<GenericBadGuy>(m_sprite.get());
	badguy->set_pos(pos);
	badguy->m_weapon = m_weapon->clone(badguy.get());

	return badguy;
}