#include "util/crappy_reader.hpp"

#include <optional>
#include <iostream>
#include <sstream>

namespace {
	std::string getline(std::ifstream& is)
	{
		std::string temp;
		std::getline(is, temp);
		return temp;
	}

	std::optional<std::string> check_end(const std::string& s, const std::string& pat)
	{
		if (s.size() < pat.size()) return std::nullopt;
		if (s.substr(s.size() - pat.size()) == pat) {
			return s.substr(0, s.size() - pat.size());
		}
		return std::nullopt;
	}
}

CrappyReader::CrappyReader(const std::string& filename) :
	m_is(filename),
	m_data_holder()
{
	clear();
}

void CrappyReader::clear()
{
	m_data_holder.clear();
	m_root = get_new_node("");
}

bool CrappyReader::skip_until(const std::string& desired)
{
	while (!m_is.eof()) {
		if (getline(m_is) == desired) break;
	}
	return !m_is.eof();
}

void CrappyReader::parse_until(const std::string& desired)
{
	dfs_parse(m_root, desired);
}

CrappyReaderData* CrappyReader::get_root() const
{
	return m_root;
}

void CrappyReader::dfs_parse(CrappyReaderData* node, const std::string& desired)
{
	std::string line = getline(m_is);
	while (m_is.is_open()) {
		if (line == desired) break;
		std::optional<std::string> start_name = check_end(line, "-start");
		if (start_name.has_value()) {
			node->m_childs.push_back(get_new_node(*start_name));
			dfs_parse(node->m_childs.back(), *start_name + "-end");
		} else {
			std::stringstream ss(line);
			std::string name;
			ss >> name;
			node->m_childs.push_back(get_new_node(name));
			while (ss.tellp() == std::streampos(0)) {
				std::string token;
				ss >> token;
				node->m_childs.back()->m_childs.push_back(get_new_node(token));
			}
		}
		line = getline(m_is);
	}
}

CrappyReaderData* CrappyReader::get_new_node(const std::string& s)
{
	m_data_holder.push_back(std::make_unique<CrappyReaderData>(s));
	return m_data_holder.back().get();
}


