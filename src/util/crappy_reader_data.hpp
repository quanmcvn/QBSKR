#ifndef HEADER_QBSKR_UTIL_CRAPPY_READER_DATA_HPP
#define HEADER_QBSKR_UTIL_CRAPPY_READER_DATA_HPP

#include <string>
#include <vector>
#include <iosfwd>

/**
 * Class to hold data of CrappyReader
 * 
 * Design:
 *   - Currently, the CrappyReaderData is designed to:
 *     + Hold a string
 *     + Hold a vector of CrappyReaderData* to present childs (kind of a trie)
 *   - What this design do:
 *     + store the
 *        .
 *        ├──"name0"
 *        │    └── "property"
 *        ├──"name1"
 *        │    ├── "property0"
 *        │    └── "property1"
 *        └──"name2"
 *             ├── "name3"
 *             │      ├── "property0"
 *             │      └── "property1"
 *             └── "name4"
 *                    ├── "property0"
 *                    ├── "property1"
 *                    └── "property2"
 *      in the a tree-like format
 *     + get a child a parent with a specific name in O(n)
 *     + can't use std::map since 
 *         1) change listing order
 *         2) this is not designed with {key : value} in mind, just value
 *   - Hardcoded a lot of functions
 *   - Because use std::ifstream, std::stringstream as backbone,
 *     this reader is line and whitespace sensitive
*/
class CrappyReaderData final {
public:
	CrappyReaderData();

public:
	std::string m_data;
	using Child = CrappyReaderData*;
	std::vector<Child> m_childs;
	// parent path of file the CrappyReader reads
	// shared between every CrappyReaderData
	const std::string& m_parent_path;

public:
	CrappyReaderData(const std::string& data, const std::string& parent_path);

	Child get_child(const std::string& name) const;
	std::vector<Child> get_child_all(const std::string& name) const;
	std::vector<std::string> convert_child() const;

	bool get(const std::string& name, bool& value) const;
	bool get(const std::string& name, int& value) const;
	bool get(const std::string& name, uint32_t& value) const;
	bool get(const std::string& name, float& value) const;
	bool get(const std::string& name, std::string& value) const;
	bool get(const std::string& name, std::vector<int>& values) const;
	bool get(const std::string& name, std::vector<uint32_t>& values) const;
	bool get(const std::string& name, std::vector<float>& values) const;
	bool get(const std::string& name, std::vector<std::string>& values) const;

	bool get_child_values(std::vector<int>& values) const;
	bool get_child_values(std::vector<uint32_t>& values) const;

	void dfs_print(std::ostream& os, int depth = 0) const;
};

#endif