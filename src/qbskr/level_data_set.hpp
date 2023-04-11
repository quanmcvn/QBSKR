#ifndef HEADER_QBSKR_QBSKR_LEVEL_DATA_SET_HPP
#define HEADER_QBSKR_QBSKR_LEVEL_DATA_SET_HPP

#include <map>
#include <memory>

#include "util/currenton.hpp"

class LevelData;

/**
 *  Hold a lot of level datas
 *  It's different from other xxxSet (no clone)
*/
class LevelDataSet final : public Currenton<LevelDataSet> {
public:
	LevelDataSet();
	~LevelDataSet() override;

private:
	LevelDataSet(const LevelDataSet&) = delete;
	LevelDataSet& operator=(const LevelDataSet&) = delete;

private:
	std::map<std::string, std::unique_ptr<LevelData>> m_level_datas;

public:
	const LevelData& get_level_data(const std::string& level_filename);
};

#endif