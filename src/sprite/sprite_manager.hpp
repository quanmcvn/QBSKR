#ifndef HEADER_QBSKR_SPRITE_SPRITE_MANAGER_HPP
#define HEADER_QBSKR_SPRITE_SPRITE_MANAGER_HPP

#include <map>
#include <memory>
#include <string>

#include "sprite/sprite_ptr.hpp"
#include "util/currenton.hpp"

class SpriteData;

/**
 * How all of this (suppose to) work (pt.3)
 *   - SpriteManager is the class which creates and holds SpriteData
 *   - SpriteData is the data needed to create Sprite
 *   - SpriteData::Action is the struct holding:
 *      + name
 *      + x, y, w, h of hitbox
 *      + surfaces, listing in order for Sprite to work
 *   - Sprite is the class calculating and drawing
 *      + frame is calculated time-wise so it can have non integer fps
*/

/**
 * Class to create Sprite and store them
*/
class SpriteManager final : public Currenton<SpriteManager> {
public:
	SpriteManager();
	~SpriteManager() override;

private:
	SpriteManager(const SpriteManager&) = delete;
	SpriteManager& operator=(const SpriteManager&) = delete;

private:
	using Sprites = std::map<std::string, std::unique_ptr<SpriteData>>;
	Sprites sprites;

public:
	SpritePtr create(const std::string& filename);

private:
	SpriteData* load(const std::string& filename);
};

#endif