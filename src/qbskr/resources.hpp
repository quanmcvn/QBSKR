#ifndef HEADER_QBSKR_QBSKR_RESOURCES_HPP
#define HEADER_QBSKR_QBSKR_RESOURCES_HPP

#include <memory>
#include <string>

#include "video/font_ptr.hpp"

class Resources final {
public:
	Resources();
	~Resources();

public:
	// used for hud
	static FontPtr fixed_font;
	// used for menu
	static FontPtr normal_font;

public:
	void load();
	void unload();
};

#endif