#include "qbskr/resources.hpp"

#include "video/font.hpp"
#include "video/ttf_font.hpp"

Resources::Resources() { load(); }
Resources::~Resources() { unload(); }

FontPtr Resources::fixed_font;
FontPtr Resources::normal_font;

void Resources::load()
{
	const std::string font = "fonts/minecraft_font.ttf";
	fixed_font = std::make_shared<TTFFont>(font, 8.0f, 1.25f);
	normal_font = fixed_font;
}

void Resources::unload()
{
	fixed_font.reset();
	normal_font.reset();
}