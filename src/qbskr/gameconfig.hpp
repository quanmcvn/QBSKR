#ifndef HEADER_QBSKR_QBSKR_GAMECONFIG_HPP
#define HEADER_QBSKR_QBSKR_GAMECONFIG_HPP

#include "control/keyboard_config.hpp"
#include "control/mouse_button_config.hpp"
#include "math/size.hpp"

class Config final {
public:
	Config();

	Size window_size;

	float magnification;

	KeyboardConfig keyboard_config;
	MouseButtonConfig mouse_button_config;
};

#endif