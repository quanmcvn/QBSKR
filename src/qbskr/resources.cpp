#include "qbskr/resources.hpp"

#include <string>

#include "gui/mouse_cursor.hpp"
#include "sprite/sprite_manager.hpp"
#include "sprite/sprite.hpp"
#include "video/font.hpp"
#include "video/ttf_font.hpp"

Resources::Resources() { load(); }
Resources::~Resources() { unload(); }

std::unique_ptr<MouseCursor> Resources::mouse_cursor;

FontPtr Resources::fixed_font;
FontPtr Resources::normal_font;
FontPtr Resources::big_font;

void Resources::load()
{
	mouse_cursor = std::make_unique<MouseCursor>(SpriteManager::current()->create("images/menu/mouse-cursor-sprite.txt"));

	const std::string font = "fonts/minecraft_font.ttf";
	fixed_font = std::make_shared<TTFFont>(font, 8.0f, 1.25f);
	normal_font = fixed_font;
	big_font = std::make_shared<TTFFont>(font, 12.0f, 1.25f);
}

void Resources::unload()
{
	fixed_font.reset();
	normal_font.reset();
	big_font.reset();
}