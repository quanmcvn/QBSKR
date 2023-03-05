#ifndef HEADER_QBSKR_QBSKR_GAMECONFIG_HPP
#define HEADER_QBSKR_QBSKR_GAMECONFIG_HPP

#include "control/keyboard_config.hpp"
#include "control/mouse_button_config.hpp"

class Config final {
public:
	Config();

	KeyboardConfig keyboard_config;
	MouseButtonConfig mouse_button_config;
};

#endif