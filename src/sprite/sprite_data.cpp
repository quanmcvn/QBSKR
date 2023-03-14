#include "sprite/sprite_data.hpp"

#include <filesystem>
#include <sstream>

#include "util/crappy_reader.hpp"
#include "video/surface.hpp"

SpriteData::Action::Action() :
	name(),
	x_offset(0),
	y_offset(0),
	hitbox_w(0),
	hitbox_h(0),
	fps(8),
	surfaces()
{}

SpriteData::~SpriteData()
{
	actions.clear();
}

SpriteData::SpriteData(CrappyReader& cr) :
	actions(),
	name()
{
	name = cr.get_dir();

	const std::string parent_dir = std::filesystem::path(cr.get_dir()).parent_path().string();

	const CrappyReaderData* root = cr.get_root();
	
	// ugly hardcode
	while (cr.parse("action")) {
		parse_action(root->m_childs.back(), parent_dir);
	}
}

void SpriteData::parse_action(CrappyReaderData* crd, const std::string& parent_dir)
{
	auto action = std::make_unique<Action>();

	if (!crd->get("name", action->name)) {
		throw std::runtime_error("Action should have name!");
	}

	std::vector<float> hitbox;
	if (crd->get("hitbox", hitbox)) {
		if (hitbox.size() != 4) {
			throw std::runtime_error("Hitbox should specify 4 coords");
		}
		action->x_offset = hitbox[0];
		action->y_offset = hitbox[1];
		action->hitbox_w = hitbox[2];
		action->hitbox_h = hitbox[3];
	}

	crd->get("fps", action->fps);

	std::string mirror_action;
	if (crd->get("mirror-action", mirror_action)) {
		const auto action_temp = get_action(mirror_action);
		if (action_temp == nullptr) {
			std::ostringstream msg;
			msg << "Couldn't mirror action, action not found: \"" << mirror_action << "\"\n";
			msg << "Mirror actions must be defined after real one";
			throw std::runtime_error(msg.str());
		} else {
			action->fps = action_temp->fps;
			
			action->x_offset = action_temp->x_offset;
			action->y_offset = action_temp->y_offset;
			action->hitbox_w = action_temp->hitbox_w;
			action->hitbox_h = action_temp->hitbox_h;

			for (const auto& surf : action_temp->surfaces) {
				auto surface = surf->clone_flip(HORIZONTAL_FLIP);
				action->surfaces.push_back(surface);
			}
		}
	} else {
		// load images
		std::vector<std::string> image_filenames;
		if (crd->get("images", image_filenames)) {
			for (const auto& image_filename : image_filenames) {
				auto surface = Surface::from_file(parent_dir + "/" + image_filename);
				action->surfaces.push_back(surface);
			}
		} else {
			std::ostringstream msg;
			msg << "Sprite '" << name << "' contains no images in action '" << action->name << "'.";
			throw std::runtime_error(msg.str());
		}
	}

	actions[action->name] = std::move(action);
}

const SpriteData::Action* SpriteData::get_action(const std::string& name) const
{
	auto it = actions.find(name);
	if (it == actions.end()) {
		return nullptr;
	}
	return it->second.get();
}