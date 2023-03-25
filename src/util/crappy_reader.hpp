#ifndef HEADER_QBSKR_UTIL_CRAPPY_READER_HPP
#define HEADER_QBSKR_UTIL_CRAPPY_READER_HPP

#include <fstream>
#include <vector>
#include <memory>
#include <string>

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
 * Also kinda pointless since there is json/...
 * Anyway...
 * 
 * Problem arise:
 *   - MY specific format depends on line
 *   - So to parse thing like TileMap, I have 2 choices:
 *      1) 1 entry per line (super bad)
 *      2) add a prefix to each line (I'll go this way) (but it also create problems)
 *        + Have to change/add CrappyReaderData to parse this specific format
 *        + Hardcode the parse in TileMap
*/

class CrappyReader final {
private:
	CrappyReader() = delete;

public:
	~CrappyReader();

private:
	CrappyReader(const CrappyReader&) = delete;
	CrappyReader& operator=(const CrappyReader&) = delete;

private:
	std::ifstream m_is;
	std::string m_dir;
	std::string m_parent_path;
	CrappyReaderData* m_root;
	std::vector<std::unique_ptr<CrappyReaderData>> m_data_holder;

public:
	CrappyReader(const std::string& filename);

public:
	void clear();
	bool parse(const std::string& desired);
	CrappyReaderData* get_root() const;
	std::string get_dir() const;

private:
	void dfs_parse(CrappyReaderData* node, const std::string& desired);
	void skip_until(const std::string& desired);

	CrappyReaderData* get_new_node(const std::string& s);
};

#endif