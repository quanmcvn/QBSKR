#ifndef HEADER_QBSKR_CONTROL_KEYBOARD_CONFIG_HPP
#define HEADER_QBSKR_CONTROL_KEYBOARD_CONFIG_HPP

#include <SDL.h>
#include <map>

#include "control/controller.hpp"

class KeyboardConfig final {
	friend class KeyboardManager;

private:
	KeyboardConfig(const KeyboardConfig&) = delete;
	KeyboardConfig& operator=(const KeyboardConfig&) = delete;

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
	std::map<SDL_Keycode, PlayerControl> m_keymap;

public:
	KeyboardConfig();

	SDL_Keycode reversemap_key(int player, Control control) const;
	
	// // Bind control of player with key 
	// void bind_key(SDL_Keycode key, int player, Control control);

};

#endif