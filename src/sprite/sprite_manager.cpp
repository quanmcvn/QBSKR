#include "sprite/sprite_manager.hpp"

#include <sstream>

#include "sprite/sprite.hpp"
#include "util/crappy_reader.hpp"

SpriteManager::SpriteManager() :
	sprites()
{}

SpriteManager::~SpriteManager()
{
	sprites.clear();
}

SpritePtr SpriteManager::create(const std::string& name)
{
	auto it = sprites.find(name);
	SpriteData* data;
	if (it == sprites.end()) {
		data = load(name);
		if (data == nullptr) {
			std::ostringstream msg;
			msg << "Sprite '" << name << "' not found.";
			throw std::runtime_error(msg.str());
		}
	} else {
		data = it->second.get();
	}

	return std::make_unique<Sprite>(*data);
}

SpriteData* SpriteManager::load(const std::string& filename)
{
	auto data = std::make_unique<SpriteData>(filename);
	sprites[filename] = std::move(data);

	return sprites[filename].get();
}