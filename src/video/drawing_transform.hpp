#ifndef HEADER_QBSKR_VIDEO_DRAWING_TRANSFORM_HPP
#define HEADER_QBSKR_VIDEO_DRAWING_TRANSFORM_HPP

#include "math/vector.hpp"
#include "video/flip.hpp"

class DrawingTransform final {
public:
	Vector translation;
	Flip flip;
	float scale;

	DrawingTransform() : 
		translation(0.0f, 0.0f),
		flip(NO_FLIP),
		scale(1.0f)
	{}
};

#endif