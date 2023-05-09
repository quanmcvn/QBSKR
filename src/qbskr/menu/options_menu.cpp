#include "qbskr/menu/options_menu.hpp"

#include <algorithm>
#include <sstream>

#include "audio/sound_manager.hpp"
#include "gui/menu_item.hpp"
#include "qbskr/gameconfig.hpp"
#include "qbskr/globals.hpp"

namespace {
	bool less_than_volume(const std::string& lhs, const std::string& rhs)
	{
		int lhs_ = 0, rhs_ = 0;
		std::stringstream ss_lhs(lhs); ss_lhs >> lhs_;
		std::stringstream ss_rhs(rhs); ss_rhs >> rhs_;
		return lhs_ < rhs_;
	}
}

enum OptionsMenuIDs {
	MNID_SOUND_VOLUME,
	MNID_MUSIC_VOLUME,
};

OptionsMenu::OptionsMenu() :
	selected_sound_volume(),
	selected_music_volume(),
	sound_volumes(),
	music_volumes()
{
	add_label("Options");
	add_horizontal_line();

	{
		// sound volumes
		sound_volumes.clear();
		for (const char* percent : {"0%", "10%", "20%", "30%", "40%", "50%", "60%", "70%", "80%", "90%", "100%"}) {
			sound_volumes.push_back(percent);
		}
		std::ostringstream ss;
		ss << g_config->sound_volume << "%";
		std::string sound_vol_string = ss.str();

		if (std::find(sound_volumes.begin(), sound_volumes.end(), sound_vol_string) == sound_volumes.end()) {
			sound_volumes.push_back(sound_vol_string);
		}

		std::sort(sound_volumes.begin(), sound_volumes.end(), less_than_volume);
		for (int i = 0; i < static_cast<int>(sound_volumes.size()); ++ i) {
			if (sound_volumes[i] == sound_vol_string) {
				selected_sound_volume = i;
				break;
			}
		}
	}

	{
		// music volumes
		music_volumes.clear();
		for (const char* percent : {"0%", "10%", "20%", "30%", "40%", "50%", "60%", "70%", "80%", "90%", "100%"}) {
			music_volumes.push_back(percent);
		}
		std::ostringstream ss;
		ss << g_config->music_volume << "%";
		std::string music_vol_string = ss.str();

		if (std::find(music_volumes.begin(), music_volumes.end(), music_vol_string) == music_volumes.end()) {
			music_volumes.push_back(music_vol_string);
		}

		std::sort(music_volumes.begin(), music_volumes.end(), less_than_volume);
		for (int i = 0; i < static_cast<int>(music_volumes.size()); ++ i) {
			if (music_volumes[i] == music_vol_string) {
				selected_music_volume = i;
				break;
			}
		}
	}

	add_string_select(MNID_SOUND_VOLUME, "Sound Volume", selected_sound_volume, sound_volumes);
	add_string_select(MNID_MUSIC_VOLUME, "Music Volume", selected_music_volume, music_volumes);

	add_horizontal_line();
	add_back("Back");
}

void OptionsMenu::item_do_menu_action(MenuItem& item)
{
	switch (item.get_id()) {
		case MNID_SOUND_VOLUME:
			{
				std::stringstream ss(sound_volumes[selected_sound_volume]);
				ss >> g_config->sound_volume;
				SoundManager::current()->set_sound_volume(g_config->sound_volume);
				g_config->save();
			}
			break;

		case MNID_MUSIC_VOLUME:
			{
				std::stringstream ss(music_volumes[selected_music_volume]);
				ss >> g_config->music_volume;
				SoundManager::current()->set_music_volume(g_config->music_volume);
				g_config->save();
			}
			break;

		default:
			break;
	}
}