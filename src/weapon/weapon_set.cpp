#include "weapon/weapon_set.hpp"

#include <sstream>

#include "util/crappy_reader.hpp"
#include "util/log.hpp"
#include "weapon/shooting_weapon/generic_shooting_weapon.hpp"
#include "weapon_set.hpp"

WeaponSet::WeaponSet() :
	m_filename("images/weapons/weapons-weapon-set.txt"),
	m_weapons(1)
{
	parse(m_filename);
}

WeaponSet::~WeaponSet()
{
	m_weapons.clear();
}

WeaponSet::WeaponSet(const std::string& filename) :
	m_filename(filename),
	m_weapons(1)
{
	parse(m_filename);
}

void WeaponSet::parse(const std::string& filename)
{
	CrappyReader cr(filename);

	while (cr.parse("weapon-set")) {}

	CrappyReaderData* crd = cr.get_root()->get_child("weapon-set");

	if (!crd) {
		std::ostringstream msg;
		msg << "File '" << filename << "' is not projectile-set file";
		throw std::runtime_error(msg.str());
	}

	for (const auto& child : crd->m_childs) {
		if (child->m_data == "weapon") parse_weapon(child);
	}
}

void WeaponSet::parse_weapon(const CrappyReaderData* crd)
{
	uint32_t id;
	if (!crd->get("id", id)) {
		throw std::runtime_error("Missing id in weapon");
	}

	std::string weapon_type;
	if (!crd->get("weapon-type", weapon_type)) {
		throw std::runtime_error("Missing weapon-type in weapon");
	}

	const CrappyReaderData* crd_weapon_specific = crd->get_child("weapon-specific");

	if (!crd_weapon_specific) {
		throw std::runtime_error("Missing weapon-specific in weapon");
	}

	std::unique_ptr<Weapon> weapon;
	if (weapon_type == "generic-shooting-weapon") {
		weapon = GenericShootingWeapon::from_reader(crd_weapon_specific);
	} else {
		throw std::runtime_error("doesn't exist / NYI");
	}

	add_weapon(id, std::move(weapon));
}

void WeaponSet::add_weapon(uint32_t id, std::unique_ptr<Weapon> weapon)
{
	if (id >= static_cast<uint32_t>(m_weapons.size())) {
		m_weapons.resize(id + 1);
	}

	if (m_weapons[id]) {
		log_warning << "Weapon id " << id << " redefined" << std::endl;
	} else {
		m_weapons[id] = std::move(weapon);
	}
}

const Weapon& WeaponSet::get_weapon(uint32_t id) const
{
	if (id >= static_cast<uint32_t>(m_weapons.size())) {
		log_warning << "Invalid weapon id: " << id << std::endl;
		return *m_weapons[0];
	} else {
		Weapon* weapon = m_weapons[id].get();
		if (weapon) {
			return *weapon;
		} else {
			log_warning << "Invalid weapon id: " << id << std::endl;
			return *m_weapons[0];
		}
	}
}