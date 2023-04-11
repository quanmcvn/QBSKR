#ifndef HEADER_QBSKR_WEAPON_SHOOTING_WEAPON_PROJECTILE_PROJECTILE_SET_HPP
#define HEADER_QBSKR_WEAPON_SHOOTING_WEAPON_PROJECTILE_PROJECTILE_SET_HPP

#include <vector>
#include <map>
#include <string>
#include <memory>

#include "util/currenton.hpp"

class CrappyReaderData;
class Vector;
class Projectile;

/**
 *  Class to hold Projectile for Projectile cloning
 *  Should've used a different design...
*/
class ProjectileSet final : public Currenton<ProjectileSet> {
public:
	ProjectileSet();
	virtual ~ProjectileSet() override;

private:
	ProjectileSet(const ProjectileSet&) = delete;
	ProjectileSet& operator=(const ProjectileSet&) = delete;

private:
	std::string m_filename;
	std::vector<std::unique_ptr<Projectile>> m_projectiles;

public:
	ProjectileSet(const std::string& filename);

private:
	void parse(const std::string& filename);
	void parse_projectile(const CrappyReaderData* crd, const std::string& parent_path);

public:
	void add_projectile_sprite(uint32_t id, std::unique_ptr<Projectile> projectiles);
	const Projectile& get_projectile(uint32_t id) const;
};

#endif