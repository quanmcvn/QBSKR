#ifndef HEADER_QBSKR_CONTROL_MOUSE_BUTTON_MANAGER_HPP
#define HEADER_QBSKR_CONTROL_MOUSE_BUTTON_MANAGER_HPP

#include "control/controller.hpp"
#include "control/mouse_button_config.hpp"

class InputManager;
struct SDL_MouseButtonEvent;

/**
 *  Class to process SDL_MouseButtonEvent
 *  Fed by InputManager
*/
class MouseButtonManager final {
private:
	MouseButtonManager(const MouseButtonManager&) = delete;
	MouseButtonManager& operator=(const MouseButtonManager&) = delete;

private:
	InputManager* m_parent;
	MouseButtonConfig& m_mouse_button_config;

public:
	MouseButtonManager(InputManager* parent, MouseButtonConfig& mouse_button_config);
	void process_button_event(const SDL_MouseButtonEvent& event);
};

#endif