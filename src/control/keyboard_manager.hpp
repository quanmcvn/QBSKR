#ifndef HEADER_QBSKR_CONTROL_KEYBOARD_MANAGER_HPP
#define HEADER_QBSKR_CONTROL_KEYBOARD_MANAGER_HPP

#include "control/controller.hpp"
#include "control/keyboard_config.hpp"

class InputManager;
struct SDL_KeyboardEvent;

class KeyboardManager final {
private:
	KeyboardManager(const KeyboardManager&) = delete;
	KeyboardManager& operator=(const KeyboardManager&) = delete;

private:
	InputManager* m_parent;
	KeyboardConfig& m_keyboard_config;

public:
	KeyboardManager(InputManager* parent, KeyboardConfig& keyboard_config);
	void process_key_event(const SDL_KeyboardEvent& event);
};

#endif