#include "control/mouse_motion_manager.hpp"

#include "control/input_manager.hpp"

MouseMotionManager::MouseMotionManager(InputManager* parent) :
	m_parent(parent) {}

void MouseMotionManager::process_motion_event(const SDL_MouseMotionEvent& event) {
	m_parent->get_controller(0).set_mouse_pos(Vector(event.x, event.y));
}