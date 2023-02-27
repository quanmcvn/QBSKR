#ifndef HEADER_QBSKR_MOUSE_MOTION_MANAGER_HPP
#define HEADER_QBSKR_MOUSE_MOTION_MANAGER_HPP

#include "control/controller.hpp"

class InputManager;
struct SDL_MouseMotionEvent;

class MouseMotionManager final {
private:
	MouseMotionManager(const MouseMotionManager&) = delete;
	MouseMotionManager& operator=(const MouseMotionManager&) = delete;

private:
	InputManager* m_parent;

public:
	MouseMotionManager(InputManager* parent);
	void process_motion_event(const SDL_MouseMotionEvent& event);
};

#endif