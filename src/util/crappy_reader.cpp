#include "util/crappy_reader.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <optional>
#include <sstream>

namespace {
	std::string getline(std::ifstream& is)
	{
		std::string temp;
		std::getline(is, temp);
		// remove tab from string so that I can tab to format
		temp.erase(std::remove(temp.begin(), temp.end(), '\t'), temp.end());
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
	m_dir(filename),
	m_data_holder()
{
	if (!std::filesystem::exists(filename)) {
		std::ostringstream msg;
		msg << "File '" << filename << "' not exist";
		throw std::runtime_error(msg.str());
	}
	if (!m_is.is_open()) {
		std::ostringstream msg;
		msg << "Can't open file '" << filename << "'";
		throw std::runtime_error(msg.str());
	}
	clear();
}

CrappyReader::~CrappyReader()
{
	m_data_holder.clear();
	m_is.close();
}

void CrappyReader::clear()
{
	m_data_holder.clear();
	m_root = get_new_node("root");
}

bool CrappyReader::parse(const std::string& desired)
{
	m_root->m_childs.push_back(get_new_node(desired));
	skip_until(desired + "-start");
	dfs_parse(m_root->m_childs.back(), desired + "-end");
	return !m_is.eof();
}


CrappyReaderData* CrappyReader::get_root() const
{
	return m_root;
}

std::string CrappyReader::get_dir() const
{
	return m_dir;
}

void CrappyReader::dfs_parse(CrappyReaderData* node, const std::string& desired)
{
	std::string line = getline(m_is);
	while (!m_is.eof()) {
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
			std::string token;
			while (ss >> token) {
				node->m_childs.back()->m_childs.push_back(get_new_node(token));
			}
		}
		line = getline(m_is);
	}
}

void CrappyReader::skip_until(const std::string& desired)
{
	while (!m_is.eof()) {
		if (getline(m_is) == desired) break;
	}
}

CrappyReaderData* CrappyReader::get_new_node(const std::string& s)
{
	m_data_holder.push_back(std::make_unique<CrappyReaderData>(s));
	return m_data_holder.back().get();
}