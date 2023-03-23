#ifndef HEADER_QBSKR_VIDEO_LAYER_HPP
#define HEADER_QBSKR_VIDEO_LAYER_HPP

// some constants for predefined layer values
enum {
	// background
	LAYER_BACKGROUND = -100,
	// solid tilemaps
	LAYER_TILES = 0,

	// projectiles
	LAYER_PROJECTILES = 100,

	// normal objects
	LAYER_OBJECTS = 200,

	// weapons
	LAYER_WEAPONS = 300,

	LAYER_FOREGROUND = 400
};

#endif