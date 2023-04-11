#ifndef HEADER_QBSKR_WEAPON_WEAPON_SET_HPP
#define HEADER_QBSKR_WEAPON_WEAPON_SET_HPP

#include <vector>
#include <string>
#include <memory>
#include <string>

#include "util/currenton.hpp"

class CrappyReaderData;
class MovingObject;
class Weapon;

/**
 *  Class to hold Weapon for Weapon cloning
 *  Should've used a different design...
*/
class WeaponSet : public Currenton<WeaponSet> {
public:
	WeaponSet();
	virtual ~WeaponSet() override;

private:
	WeaponSet(const WeaponSet&) = delete;
	WeaponSet& operator=(const WeaponSet&) = delete;

private:
	std::string m_filename;
	std::vector<std::unique_ptr<Weapon>> m_weapons;

public:
	WeaponSet(const std::string& filename);

private:
	void parse(const std::string& filename);
	void parse_weapon(const CrappyReaderData* crd);

public:
	void add_weapon(uint32_t id, std::unique_ptr<Weapon> weapon);
	const Weapon& get_weapon(uint32_t id) const;
};

#endif