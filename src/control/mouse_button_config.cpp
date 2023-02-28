#include "control/mouse_button_config.hpp"

#include <optional>

#include "util/log.hpp"

MouseButtonConfig::MouseButtonConfig() :
	m_mouse_button_map()
{
	m_mouse_button_map[SDL_BUTTON(SDL_BUTTON_LEFT)] = { 0, Control::ATTACK };
}

SDL_Keycode MouseButtonConfig::reversemap_mousebutton(int player, Control control) const
{
	for (const auto& i : m_mouse_button_map) {
		if (i.second == PlayerControl{player, control}) {
			return i.first;
		}
	}

	return SDLK_UNKNOWN;
}