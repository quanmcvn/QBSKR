#ifndef HEADER_QBSKR_QBSKR_RESOURCES_HPP
#define HEADER_QBSKR_QBSKR_RESOURCES_HPP

#include <memory>

#include "video/font_ptr.hpp"

class MouseCursor;

class Resources final {
public:
	Resources();
	~Resources();

public:
	static std::unique_ptr<MouseCursor> mouse_cursor;
	// used for hud
	static FontPtr fixed_font;
	// used for menu
	static FontPtr normal_font;
	// used for label
	static FontPtr big_font;

public:
	void load();
	void unload();
};

#endif