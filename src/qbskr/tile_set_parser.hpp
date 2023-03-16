#ifndef HEADER_QBSKR_QBSKR_TILE_SET_PARSER_HPP
#define HEADER_QBSKR_QBSKR_TILE_SET_PARSER_HPP

#include <string>

class CrappyReaderData;
class TileSet;

class TileSetParser final {
private:
	TileSetParser(const TileSetParser&) = delete;
	TileSetParser& operator=(const TileSetParser&) = delete;

private:
	TileSet& m_tileset;
	std::string m_filename;
	std::string m_parent_path;

public:
	TileSetParser(TileSet& tileset, const std::string& filename);

public:
	void parse();
	void parse_tile(CrappyReaderData* crd);
};

#endif