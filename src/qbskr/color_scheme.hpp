#ifndef HEADER_QBSKR_QBSKR_COLOR_SCHEME_HPP
#define HEADER_QBSKR_QBSKR_COLOR_SCHEME_HPP

#include "video/color.hpp"

class ColorScheme final {
public:
	class Menu final {
	public:
		static Color front_color;
		static Color horizontal_line_color;
		static Color default_color;
		static Color active_color;
		static Color label_color;
	};

	class Text final {
	public:
		static Color normal_color;
	};

	class HUD final {
	public:
		static Color damage_color;
	};
};

#endif