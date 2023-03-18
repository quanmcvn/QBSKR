#include "object/tile_set_parser.hpp"

#include <filesystem>

#include "object/tile_set.hpp"
#include "video/surface.hpp"
#include "util/crappy_reader.hpp"

TileSetParser::TileSetParser(TileSet& tileset, const std::string& filename) :
	m_tileset(tileset),
	m_filename(filename)
{}

void TileSetParser::parse()
{
	CrappyReader cr(m_filename);

	m_parent_path = std::filesystem::path(cr.get_dir()).parent_path().string() + '/';

	while (cr.parse("tileset")) {}

	CrappyReaderData* root_tileset = cr.get_root()->get_child("tileset");

	for (const auto& child : root_tileset->m_childs) {
		if (child->m_data == "tile") {
			parse_tile(child);
		}
	}
}

void TileSetParser::parse_tile(CrappyReaderData* crd)
{
	uint32_t id;
	if (!crd->get("id", id)) {
		throw std::runtime_error("Missing tile id");
	}

	float fps = 1;
	crd->get("fps", fps);

	uint32_t attributes = 0;
	if (CrappyReaderData::Child crd_attributes = crd->get_child("attributes")) {
		bool value;
		if (crd_attributes->get("solid", value) && value) attributes |= Tile::SOLID;
		if (crd_attributes->get("goal", value) && value) attributes |= Tile::GOAL;
	}

	std::string name;
	crd->get("name", name);

	std::vector<std::string> image_filenames;
	if (!crd->get("images", image_filenames)) {
		throw std::runtime_error("Tile without image");
	}
	std::vector<SurfacePtr> surfaces;
	for (const auto& image_filename : image_filenames) {
		SurfacePtr surface = Surface::from_file(m_parent_path + image_filename);
		surfaces.push_back(std::move(surface));
	}

	std::unique_ptr<Tile> tile = std::make_unique<Tile>(surfaces, attributes, fps, name);

	m_tileset.add_tile(id, std::move(tile));
}

