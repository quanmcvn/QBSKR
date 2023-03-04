#include "control/controller.hpp"

#include <ostream>

namespace {

	const char* g_control_names[] = {
		"left",
		"right",
		"up",
		"down",
		"attack",
		nullptr
	};

} // namespace

std::ostream& operator<<(std::ostream& os, Control control)
{
	return os << g_control_names[static_cast<int>(control)];
}

std::string control_to_string(Control control)
{
	return g_control_names[static_cast<int>(control)];
}

std::optional<Control> control_from_string(const std::string& text)
{
	for (int i = 0; g_control_names[i] != nullptr; ++i) {
		if (text == g_control_names[i]) {
			return static_cast<Control>(i);
		}
	}

	return std::nullopt;
}

Controller::Controller()
{
	reset();
}

Controller::~Controller() {}

void Controller::reset()
{
	for (int i = 0; i < static_cast<int>(Control::CONTROLCOUNT); ++i) {
		m_controls[i] = false;
		m_old_controls[i] = false;
	}
}

void Controller::set_control(Control control, bool value)
{
	m_controls[static_cast<int>(control)] = value;
}

void Controller::set_mouse_pos(Vector mouse_pos)
{
	m_mouse_pos = mouse_pos;
}

Vector Controller::get_mouse_pos() const
{
	return m_mouse_pos;
}

bool Controller::hold(Control control) const
{
	return m_controls[static_cast<int>(control)];
}

bool Controller::pressed(Control control) const
{
	return !m_old_controls[static_cast<int>(control)] && m_controls[static_cast<int>(control)];
}

bool Controller::released(Control control) const
{
	return m_old_controls[static_cast<int>(control)] && !m_controls[static_cast<int>(control)];
}

void Controller::update()
{
	for (int i = 0; i < static_cast<int>(Control::CONTROLCOUNT); ++i) {
		m_old_controls[i] = m_controls[i];
	}
}
