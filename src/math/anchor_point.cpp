#include "math/anchor_point.hpp"

#include <stdexcept>
#include <sstream>

#include "math/rectf.hpp"
#include "util/log.hpp"

std::vector<std::string> get_anchor_names()
{
	return {
	  "Top Left",
	  "Top",
	  "Top Right",
	  "Left",
	  "Middle",
	  "Right",
	  "Bottom Left",
	  "Bottom",
	  "Bottom Right",
	};
}

std::string anchor_point_to_string(AnchorPoint point)
{
	try {
		return get_anchor_names().at(point);
	}
	catch (const std::out_of_range&) {
		throw std::runtime_error("Invalid anchor point");
	}
}

AnchorPoint string_to_anchor_point(const std::string& str)
{
	const std::vector<std::string> anchor_keys = get_anchor_names();
	for (size_t i = 0; i < anchor_keys.size(); i++)
		if (anchor_keys.at(i) == str)
			return static_cast<AnchorPoint>(i);

	std::ostringstream msg;
	msg << "Unknown anchor '" << str << "'";
	throw std::runtime_error(msg.str());
}

Vector get_anchor_pos(const Rectf& rect, AnchorPoint point)
{
	Vector result(0.0f, 0.0f);

	switch (point % 3) {
		case 0: // left
			result.x = rect.get_left();
			break;
		case 1: // middle
			result.x = rect.get_left() + rect.get_width() / 2.0f;
			break;
		case 2: // right
			result.x = rect.get_right();
			break;
		default:
			log_warning << "Invalid anchor point found" << std::endl;
			result.x = rect.get_left();
			break;
	}

	switch (point / 3) {
		case 0: // top
			result.y = rect.get_top();
			break;
		case 1: // middle
			result.y = rect.get_top() + rect.get_height() / 2.0f;
			break;
		case 2: // bottom
			result.y = rect.get_bottom();
			break;
		default:
			log_warning << "Invalid anchor point found" << std::endl;
			result.y = rect.get_top();
			break;
	}

	return result;
}

Vector get_anchor_pos(const Rectf& destrect, float width, float height, AnchorPoint point)
{
	Vector result(0.0f, 0.0f);

	switch (point % 3) {
		case 0: // left
			result.x = destrect.get_left();
			break;
		case 1: // middle
			result.x = destrect.get_middle().x - width / 2.0f;
			break;
		case 2: // right
			result.x = destrect.get_right() - width;
			break;
		default:
			log_warning << "Invalid anchor point found" << std::endl;
			result.x = destrect.get_left();
			break;
	}

	switch (point / 3) {
		case 0: // top
			result.y = destrect.get_top();
			break;
		case 1: // middle
			result.y = destrect.get_middle().y - height / 2.0f;
			break;
		case 2: // bottom
			result.y = destrect.get_bottom() - height;
			break;
		default:
			log_warning << "Invalid anchor point found" << std::endl;
			result.y = destrect.get_top();
			break;
	}

	return result;
}