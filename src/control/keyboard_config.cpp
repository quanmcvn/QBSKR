#include "control/keyboard_config.hpp"

#include <optional>

#include "util/log.hpp"

KeyboardConfig::KeyboardConfig() :
	m_keymap()
{
	m_keymap[SDLK_a] = { 0, Control::LEFT };
	m_keymap[SDLK_d] = { 0, Control::RIGHT };
	m_keymap[SDLK_w] = { 0, Control::UP };
	m_keymap[SDLK_s] = { 0, Control::DOWN };
}

SDL_Keycode KeyboardConfig::reversemap_key(int player, Control control) const
{
	for (const auto& ctrl : m_keymap) {
		if (ctrl.second == PlayerControl{player, control}) {
			return ctrl.first;
		}
	}

	return SDLK_UNKNOWN;
}