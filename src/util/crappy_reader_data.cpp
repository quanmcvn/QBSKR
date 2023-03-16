#include "util/crappy_reader_data.hpp"

#include <iostream>
#include <assert.h>
#include <sstream>

CrappyReaderData::CrappyReaderData() :
	m_data(),
	m_childs()
{}

CrappyReaderData::CrappyReaderData(const std::string& data) :
	m_data(data),
	m_childs()
{}

CrappyReaderData* CrappyReaderData::get_child(const std::string& name) const
{
	for (const auto& child : m_childs) {
		if (child->m_data == name) return child;
	}
	return nullptr;
}

std::vector<std::string> CrappyReaderData::convert_child() const
{
	std::vector<std::string> ret;
	for (const auto& child : m_childs) {
		ret.push_back(child->m_data);
	}
	return ret;
}

namespace {
	void assert_size(const std::vector<std::string>& vec, const size_t size) {
		assert(vec.size() == size);
	}
}

/**
 * The reason that I use macro but not template is that 
 *    get(const std::string& name, T& value) can match T = std::vector<T> in
 *    get(const std::string& name, std::vector<T>& value)
*/

#define GET_VALUE_MACRO()                                              \
	CrappyReaderData* child = get_child(name);                         \
	if (child == nullptr) {                                            \
		return false;                                                  \
	} else {                                                           \
		std::vector<std::string> child_data = child->convert_child();  \
		assert_size(child_data, 1);                                    \
		std::stringstream ss(child_data[0]);                           \
		ss >> value;                                                   \
		return true;                                                   \
	}

bool CrappyReaderData::get(const std::string& name, bool& value)  const
{
	GET_VALUE_MACRO()
}

bool CrappyReaderData::get(const std::string& name, uint32_t& value)  const
{
	GET_VALUE_MACRO()
}

bool CrappyReaderData::get(const std::string& name, float& value)  const
{
	GET_VALUE_MACRO()
}

bool CrappyReaderData::get(const std::string& name, std::string& value) const
{
	GET_VALUE_MACRO()
}

#undef GET_VALUE_MACRO

#define GET_VALUES_MACRO(type)                                         \
	CrappyReaderData* child = get_child(name);                         \
	if (child == nullptr) {                                            \
		return false;                                                  \
	} else {                                                           \
		std::vector<std::string> child_data = child->convert_child();  \
		for (const auto& str : child_data) {                           \
			type value;                                                \
			std::stringstream ss(str);                                 \
			ss >> value;                                               \
			values.push_back(value);                                   \
		}                                                              \
		return true;                                                   \
	}

bool CrappyReaderData::get(const std::string& name, std::vector<float>& values) const
{
	GET_VALUES_MACRO(float)
}

bool CrappyReaderData::get(const std::string& name, std::vector<std::string>& values) const
{
	GET_VALUES_MACRO(std::string)
}

#undef GET_VALUES_MACRO

void CrappyReaderData::dfs_print(std::ostream& os, int depth) const
{
	os << std::string(depth, '\t');
	os << m_data << "\n";
	for (const auto& val : m_childs) {
		val->dfs_print(os, depth + 1);
	}
}