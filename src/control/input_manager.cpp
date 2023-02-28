#include "control/input_manager.hpp"

#include "control/keyboard_manager.hpp"
#include "control/mouse_button_manager.hpp"
#include "control/mouse_motion_manager.hpp"
#include "util/log.hpp"

InputManager::InputManager(KeyboardConfig& keyboard_config,
                           MouseButtonConfig& mouse_button_config) :
	m_controllers(),
	keyboard_manager(new KeyboardManager(this, keyboard_config)),
	mouse_button_manager(new MouseButtonManager(this, mouse_button_config)),
	mouse_motion_manager(new MouseMotionManager(this))
{
	m_controllers.push_back(std::make_unique<Controller>());
}

InputManager::~InputManager() {}

const Controller& InputManager::get_controller(int player_id) const
{
	return *m_controllers[player_id];
}

Controller& InputManager::get_controller(int player_id)
{
	return *m_controllers[player_id];
}

void InputManager::update()
{
	for (auto& controller : m_controllers) {
		controller->update();
	}
}

void InputManager::reset()
{
	for (auto& controller : m_controllers) {
		controller->reset();
	}
}

void InputManager::process_event(const SDL_Event& event)
{
	switch (event.type) {
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			keyboard_manager->process_key_event(event.key);
			break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			mouse_button_manager->process_button_event(event.button);
			break;

		case SDL_MOUSEMOTION:
			mouse_motion_manager->process_motion_event(event.motion);
			break;

		default:
			break;
	}
}
