#ifndef HEADER_QBSKR_SPRITE_SPRITE_DATA_HPP
#define HEADER_QBSKR_SPRITE_SPRITE_DATA_HPP

#include <map>
#include <string>
#include <vector>

#include "video/surface_ptr.hpp"

class CrappyReader;
class CrappyReaderData;

class SpriteData final {
public:
	~SpriteData();
	SpriteData(CrappyReader& cr);

private:
	friend class Sprite;

	struct Action {
		std::string name;
	
		float x_offset;
		float y_offset;

		float hitbox_w;
		float hitbox_h;

		float fps;

		std::vector<SurfacePtr> surfaces;

		Action();
	};

	using Actions = std::map<std::string, std::unique_ptr<Action>>;

	Actions actions;
	std::string name;

public:
	void parse_action(CrappyReaderData* crd, const std::string& parent_path);
	const Action* get_action(const std::string& name) const;
};

#endif