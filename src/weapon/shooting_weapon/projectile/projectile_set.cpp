#include "weapon/shooting_weapon/projectile/projectile_set.hpp"

#include <sstream>
#include <filesystem>

#include "util/crappy_reader.hpp"
#include "util/log.hpp"
#include "weapon/hurt.hpp"
#include "weapon/shooting_weapon/projectile/projectile.hpp"
#include "weapon/shooting_weapon/projectile/generic_projectile.hpp"

ProjectileSet::ProjectileSet() :
	m_filename("images/weapons/shooting_weapons/projectiles/projectiles-projectile-set.txt"),
	m_projectiles(1)
{
	parse(m_filename);
}

ProjectileSet::~ProjectileSet()
{
	m_projectiles.clear();
}

ProjectileSet::ProjectileSet(const std::string& filename) :
	m_filename(filename),
	m_projectiles(1)
{
	parse(m_filename);
}

void ProjectileSet::parse(const std::string& filename)
{
	CrappyReader cr(filename);

	while (cr.parse("projectile-set")) {}

	CrappyReaderData* crd = cr.get_root()->get_child("projectile-set");

	if (!crd) {
		std::ostringstream msg;
		msg << "File '" << filename << "' is not projectile-set file";
		throw std::runtime_error(msg.str());
	}

	std::string parent_path = std::filesystem::path(cr.get_dir()).parent_path().string() + '/';

	for (const auto& child : crd->m_childs) {
		if (child->m_data == "projectile") parse_projectile(child, parent_path);
	}

	// // dummy value
	// (void) m_projectiles[0][0];
}

void ProjectileSet::parse_projectile(const CrappyReaderData* crd, const std::string& parent_path)
{
	uint32_t id;
	if (!crd->get("id", id)) {
		throw std::runtime_error("Missing projectile id");
	}

	std::string projectile_type;
	if (!crd->get("projectile-type", projectile_type)) {
		throw std::runtime_error("Missing projectile-type in projectile");
	}

	const CrappyReaderData* crd_projectile_specific = crd->get_child("projectile-specific");

	if (!crd_projectile_specific) {
		throw std::runtime_error("Missing projectile-specific in weapon");
	}

	std::unique_ptr<Projectile> projectile;
	if (projectile_type == "generic-projectile") {
		projectile = GenericProjectile::from_reader(crd_projectile_specific, parent_path);
	} else {
		throw std::runtime_error("doesn't exist / NYI");
	}

	add_projectile_sprite(id, std::move(projectile));
}

void ProjectileSet::add_projectile_sprite(uint32_t id, std::unique_ptr<Projectile> projectile)
{
	if (id >= static_cast<uint32_t>(m_projectiles.size())) {
		m_projectiles.resize(id + 1);
	}

	m_projectiles[id] = std::move(projectile);
}

const Projectile& ProjectileSet::get_projectile(uint32_t id) const
{
	if (id >= static_cast<uint32_t>(m_projectiles.size())) {
		log_warning << "Invalid projectile id: " << id << std::endl;
		return *m_projectiles[0];
	} else {
		Projectile* projectile = m_projectiles[id].get();
		if (projectile) {
			return *projectile;
		} else {
			log_warning << "Invalid projectile id: " << id << std::endl;
			return *m_projectiles[0];
		}
	}
}