#ifndef HEADER_QBSKR_BADGUY_BADGUY_SET_HPP
#define HEADER_QBSKR_BADGUY_BADGUY_SET_HPP

#include <vector>
#include <memory>
#include <string>

#include "util/currenton.hpp"

class CrappyReaderData;
class BadGuy;

/**
 *  A xxxSet for cloning (same as WeaponSet, ProjectileSet)
*/
class BadGuySet final : public Currenton<BadGuySet> {
public:
	BadGuySet();
	virtual ~BadGuySet() override;

private:
	BadGuySet(const BadGuySet&) = delete;
	BadGuySet& operator=(const BadGuySet&) = delete;

private:
	std::string m_filename;
	std::vector<std::unique_ptr<BadGuy>> m_badguys;

public:
	BadGuySet(const std::string& filename);

private:
	void parse(const std::string& filename);
	void parse_badguy(const CrappyReaderData* crd);

public:
	void add_badguy(uint32_t id, std::unique_ptr<BadGuy> badguy);
	const BadGuy& get_badguy(uint32_t id) const;
};



#endif