#ifndef HEADER_QBSKR_VIDEO_FLIP_HPP
#define HEADER_QBSKR_VIDEO_FLIP_HPP

enum {
	NO_FLIP         = 0b0000,
	VERTICAL_FLIP   = 0b0010,
	HORIZONTAL_FLIP = 0b0100
};

typedef unsigned int Flip;

#endif