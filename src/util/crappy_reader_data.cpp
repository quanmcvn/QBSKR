#include "util/crappy_reader_data.hpp"

#include <iostream>

CrappyReaderData::CrappyReaderData() :
	m_data(),
	m_childs()
{}

CrappyReaderData::CrappyReaderData(const std::string& data) :
	m_data(data),
	m_childs()
{}

void CrappyReaderData::dfs_print(std::ostream& os, int depth)
{
	os << std::string(depth, '\t');
	os << m_data << "\n";
	for (const auto& val : m_childs) {
		val->dfs_print(os, depth + 1);
	}
}