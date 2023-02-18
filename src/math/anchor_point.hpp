#ifndef HEADER_QBMG_MATH_ANCHOR_POINT_HPP
#define HEADER_QBMG_MATH_ANCHOR_POINT_HPP

#include <string>
#include <vector>

#include "math/vector.hpp"

class Rectf;

enum AnchorPoint {
	ANCHOR_TOP_LEFT,
	ANCHOR_TOP,
	ANCHOR_TOP_RIGHT,
	ANCHOR_LEFT,
	ANCHOR_MIDDLE,
	ANCHOR_RIGHT,
	ANCHOR_BOTTOM_LEFT,
	ANCHOR_BOTTOM,
	ANCHOR_BOTTOM_RIGHT,
	ANCHOR_LAST = ANCHOR_BOTTOM_RIGHT
};

const std::vector<std::string> g_anchor_keys = {
    "topleft", "top", "topright", "left", "middle", "right", "bottomleft",
    "bottom", "bottomright"
};

std::vector<std::string> get_anchor_names();
std::string anchor_point_to_string(AnchorPoint point);
AnchorPoint string_to_anchor_point(const std::string& str);
Vector get_anchor_pos(const Rectf& rect, AnchorPoint point);
Vector get_anchor_pos(const Rectf& destrect, float width, float height, 
                      AnchorPoint point);

#endif