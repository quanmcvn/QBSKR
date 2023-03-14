#ifndef HEADER_QBSKR_UTIL_CRAPPY_READER_DATA_HPP
#define HEADER_QBSKR_UTIL_CRAPPY_READER_DATA_HPP

#include <string>
#include <vector>
#include <iosfwd>

/**
 * Class to hold data of CrappyReader
 * 
 * Design problem:
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
 *             └── "name3"
 *                    ├── "property0"
 *                    ├── "property1"
 *                    └── "property2"
 *      in the a tree-like format
 *     + get a child a parent with a specific name in O(n)
 *     + can't use std::map since 
 *         1) change listing order
 *         2) this is not designed with {key : value} in mind, just value
*/
class CrappyReaderData final {
public:
	std::string m_data;
	using Child = CrappyReaderData*;
	std::vector<Child> m_childs;

	CrappyReaderData();
	CrappyReaderData(const std::string& data);

	void dfs_print(std::ostream& os, int depth = 0);
};

#endif