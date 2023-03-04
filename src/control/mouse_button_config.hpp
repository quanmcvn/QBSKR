#ifndef HEADER_QBSKR_CONTROL_MOUSE_BUTTON_CONFIG_HPP
#define HEADER_QBSKR_CONTROL_MOUSE_BUTTON_CONFIG_HPP

#include <SDL.h>
#include <map>

#include "control/controller.hpp"

#include <SDL_mouse.h>

class MouseButtonConfig final {
	friend class MouseButtonManager;

private:
	MouseButtonConfig(const MouseButtonConfig&) = delete;
	MouseButtonConfig& operator=(const MouseButtonConfig&) = delete;

public:
	class PlayerControl final {
	public:
		int player;
		Control control;

	public:
		inline bool operator==(const PlayerControl& other) const {
			return player == other.player && control == other.control;
		}
	};

private:
	std::map<int, PlayerControl> m_mouse_button_map;

public:
	MouseButtonConfig();

	int reversemap_mousebutton(int player, Control control) const;
	
	// // Bind control of player with button
	// void bind_mousebutton(int mousebutton, int player, Control control);

};

#endif