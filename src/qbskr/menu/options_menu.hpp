#ifndef HEADER_QBSKR_QBSKR_MENU_OPTIONS_MENU_HPP
#define HEADER_QBSKR_QBSKR_MENU_OPTIONS_MENU_HPP

#include "gui/menu.hpp"

class OptionsMenu final : public Menu {
private:
	OptionsMenu(const OptionsMenu&) = delete;
	OptionsMenu& operator=(const OptionsMenu&) = delete;

private:
	int selected_sound_volume;
	int selected_music_volume;

	std::vector<std::string> sound_volumes;
	std::vector<std::string> music_volumes;

public:
	OptionsMenu();

	void item_do_menu_action(MenuItem& item) override;
};

#endif