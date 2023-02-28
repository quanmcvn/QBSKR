#include "control/mouse_button_manager.hpp"

#include "control/input_manager.hpp"

MouseButtonManager::MouseButtonManager(InputManager* parent,
                                       MouseButtonConfig& mouse_button_config) :
	m_parent(parent),
	m_mouse_button_config(mouse_button_config)
{}

void MouseButtonManager::process_button_event(const SDL_MouseButtonEvent& event)
{
	auto button_mapping = m_mouse_button_config.m_mouse_button_map.find(event.button);

	if (button_mapping == m_mouse_button_config.m_mouse_button_map.end()) return;

	auto control = button_mapping->second;
	bool value = (event.type == SDL_MOUSEBUTTONDOWN);

	if (control.player >= m_parent->get_num_users()) return;

	m_parent->get_controller(control.player).set_control(control.control, value);
}