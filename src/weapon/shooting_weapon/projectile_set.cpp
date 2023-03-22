#include "weapon/shooting_weapon/projectile_set.hpp"

#include <sstream>
#include <filesystem>

#include "sprite/sprite_manager.hpp"
#include "util/crappy_reader.hpp"
#include "util/log.hpp"
#include "weapon/hurt.hpp"
#include "weapon/shooting_weapon/projectile.hpp"

ProjectileSet::ProjectileSet() :
	m_filename("images/weapons/shooting_weapons/projectiles-projectile-set.txt"),
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

	std::string sprite_filename;
	if (!crd->get("sprite-filename", sprite_filename)) {
		throw std::runtime_error("Missing projectile sprite-filename");
	}

	auto projectile = std::make_unique<Projectile>(parent_path + sprite_filename, 0);

	add_projectile_sprite(id, std::move(projectile));
}

void ProjectileSet::add_projectile_sprite(uint32_t id, std::unique_ptr<Projectile> projectile)
{
	if (id >= static_cast<uint32_t>(m_projectiles.size())) {
		m_projectiles.resize(id + 1);
	}

	m_projectiles[id] = std::move(projectile);
}

const Projectile& ProjectileSet::get(uint32_t id) const
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