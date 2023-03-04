#ifndef HEADER_QBSKR_CONTROL_CONTROLLER_HPP
#define HEADER_QBSKR_CONTROL_CONTROLLER_HPP

#include <iosfwd>
#include <optional>

#include "math/vector.hpp"

// Note that this is not the control key but the action
enum class Control {
	LEFT = 0,
	RIGHT,
	UP,
	DOWN,

	ATTACK,

	CONTROLCOUNT
};

std::ostream& operator<<(std::ostream& os, Control control);

std::string control_to_string(Control control);
std::optional<Control> control_from_string(const std::string& text);

class Controller {
public:
	Controller();
	virtual ~Controller();

private:
	Controller(const Controller&) = delete;
	Controller& operator=(const Controller&) = delete;

public:
	virtual void update();

	void set_control(Control control, bool value);

	void set_mouse_pos(Vector mouse_pos);
	
	// returns true if the control is pressed down
	bool hold(Control control) const;

	// returns true if the control has just been pressed down this frame
	bool pressed(Control control) const;

	// returns true if the control has just been released this frame
	bool released(Control control) const;

	Vector get_mouse_pos() const;

	void reset();

protected:
	// current control status
	bool m_controls[static_cast<int>(Control::CONTROLCOUNT)];

	// control status at last frame
	bool m_old_controls[static_cast<int>(Control::CONTROLCOUNT)];

	/** 
	 * This is really bad, I hard-designed (?) the mouse cursor to be the only one
	 * but oh well
	*/

	Vector m_mouse_pos;
};

#endif