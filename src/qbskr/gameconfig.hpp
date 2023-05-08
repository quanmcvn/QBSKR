#ifndef HEADER_QBSKR_QBSKR_GAMECONFIG_HPP
#define HEADER_QBSKR_QBSKR_GAMECONFIG_HPP

#include <string>

#include "control/keyboard_config.hpp"
#include "control/mouse_button_config.hpp"
#include "math/size.hpp"

/**
 *  Class to hold some config (although I NYI the change config thing)
*/
class Config final {
public:
	Config();

public:
	Size window_size;

	float magnification;

	bool debug;

	int sound_volume;
	int music_volume;

	KeyboardConfig keyboard_config;
	MouseButtonConfig mouse_button_config;

public:
	Config(const std::string& filename);
};

#endif