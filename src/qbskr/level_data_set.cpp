#include "qbskr/level_data_set.hpp"

#include "qbskr/level_data.hpp"

LevelDataSet::LevelDataSet() :
	m_level_datas()
{}

LevelDataSet::~LevelDataSet()
{
	m_level_datas.clear();
}

const LevelData& LevelDataSet::get_level_data(const std::string& level_filename)
{
	auto it = m_level_datas.find(level_filename);
	if (it != m_level_datas.end()) {
		return *it->second;
	} else {
		m_level_datas[level_filename] = std::make_unique<LevelData>(level_filename);
		return *m_level_datas[level_filename];
	}
}