#ifndef HEADER_QBSKR_CONTROL_INPUT_MANAGER_HPP
#define HEADER_QBSKR_CONTROL_INPUT_MANAGER_HPP

#include "control/controller.hpp"

#include <SDL.h>
#include <vector>
#include <memory>

#include "util/currenton.hpp"

class KeyboardManager;
class KeyboardConfig;
class MouseButtonManager;
class MouseButtonConfig;
class MouseMotionManager;

class InputManager final: public Currenton<InputManager> {
public:
	~InputManager() override;

private:
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;

private:
	std::vector<std::unique_ptr<Controller>> m_controllers;

public:
	std::unique_ptr<KeyboardManager> keyboard_manager;
	std::unique_ptr<MouseButtonManager> mouse_button_manager;
	std::unique_ptr<MouseMotionManager> mouse_motion_manager;

public:
	InputManager(KeyboardConfig& keyboard_config, MouseButtonConfig& mouse_button_config);

	void process_event(const SDL_Event& event);

	void update();
	void reset();

	const Controller& get_controller(int player_id = 0) const;
	Controller& get_controller(int player_id = 0);

	int get_num_users() const { return static_cast<int>(m_controllers.size()); }

	// void push_user();
	// void pop_user();

};

#endif