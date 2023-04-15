#include "badguy/badguy_set.hpp"

#include <sstream>

#include "badguy/balbusour.hpp"
#include "badguy/generic_badguy.hpp"
#include "util/crappy_reader.hpp"
#include "util/log.hpp"
#include "weapon/weapon.hpp"

BadGuySet::BadGuySet() :
	m_filename("images/badguys/badguy-badguy-set.txt"),
	m_badguys(1)
{
	parse(m_filename);
}

BadGuySet::~BadGuySet()
{
	m_badguys.clear();
}

BadGuySet::BadGuySet(const std::string& filename) :
	m_filename(filename),
	m_badguys(1)
{
	parse(m_filename);
}

void BadGuySet::parse(const std::string& filename)
{
	CrappyReader cr(filename);

	while (cr.parse("badguy-set")) {}

	CrappyReaderData* crd = cr.get_root()->get_child("badguy-set");

	if (!crd) {
		std::ostringstream msg;
		msg << "File '" << filename << "' is not badguy-set file";
		throw std::runtime_error(msg.str());
	}

	for (const auto& child : crd->m_childs) {
		if (child->m_data == "badguy") parse_badguy(child);
	}
}

void BadGuySet::parse_badguy(const CrappyReaderData* crd)
{
	uint32_t id;
	if (!crd->get("id", id)) {
		throw std::runtime_error("Missing id in badguy");
	}

	std::string badguy_type;
	if (!crd->get("badguy-type", badguy_type)) {
		throw std::runtime_error("Missing badguy-type in badguy");
	}

	const CrappyReaderData* crd_badguy_specific = crd->get_child("badguy-specific");

	if (!crd_badguy_specific) {
		throw std::runtime_error("Missing badguy-specific in badguy");
	}

	std::unique_ptr<BadGuy> badguy;
	if (badguy_type == "generic-badguy") {
		badguy = GenericBadGuy::from_reader(crd_badguy_specific);
	} else if (badguy_type == "balbusour") {
		badguy = Balbusour::from_reader(crd_badguy_specific);
	} else {
		std::ostringstream msg;
		msg << "Badguy " << badguy_type << " doesn't exist / NYI";
		throw std::runtime_error(msg.str());
	}

	add_badguy(id, std::move(badguy));
}

void BadGuySet::add_badguy(uint32_t id, std::unique_ptr<BadGuy> badguy)
{
	if (id >= static_cast<uint32_t>(m_badguys.size())) {
		m_badguys.resize(id + 1);
	}

	if (m_badguys[id]) {
		log_warning << "Badguy id " << id << " redefined" << std::endl;
	} else {
		m_badguys[id] = std::move(badguy);
	}
}

const BadGuy& BadGuySet::get_badguy(uint32_t id) const
{
	if (id >= static_cast<uint32_t>(m_badguys.size())) {
		log_warning << "Invalid badguy id: " << id << std::endl;
		return *m_badguys[0];
	} else {
		BadGuy* badguy = m_badguys[id].get();
		if (badguy) {
			return *badguy;
		} else {
			log_warning << "Invalid badguy id: " << id << std::endl;
			return *m_badguys[0];
		}
	}
}