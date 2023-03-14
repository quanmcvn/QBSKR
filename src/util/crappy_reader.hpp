#ifndef HEADER_QBSKR_UTIL_CRAPPY_READER_HPP
#define HEADER_QBSKR_UTIL_CRAPPY_READER_HPP

#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <sstream>

#include "util/crappy_reader_data.hpp"

/**
 * Class to read data from a file with MY own specific format 
 * (don't know if anyone came out with this first tho)
 * Specific format:
 *   - If:
 *        a b c ...
 *     Then b c ... is child of a
 *   - If:
 *        a-start
 *        b
 *        c
 *        ...
 *        a-end
 *     Then b c ... is child of a
 * This is bad but it gets the job done :D
*/
class CrappyReader final {
private:
	CrappyReader() = delete;

private:
	std::ifstream m_is;
	CrappyReaderData* m_root;
	std::vector<std::unique_ptr<CrappyReaderData>> m_data_holder;

public:
	CrappyReader(const std::string& filename);

public:
	void clear();
	bool skip_until(const std::string& desired);
	void parse_until(const std::string& desired);
	CrappyReaderData* get_root() const;

private:
	void dfs_parse(CrappyReaderData* node, const std::string& desired);

	CrappyReaderData* get_new_node(const std::string& s);
};

#endif