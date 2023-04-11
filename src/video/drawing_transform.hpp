#ifndef HEADER_QBSKR_VIDEO_DRAWING_TRANSFORM_HPP
#define HEADER_QBSKR_VIDEO_DRAWING_TRANSFORM_HPP

#include "math/vector.hpp"
#include "video/flip.hpp"

/**
 *  Class to store tranform done to the drawing context
 *  Use exclusive in DrawingContext
*/
class DrawingTransform final {
public:
	Vector translation;
	Flip flip;
	float alpha;
	float scale;

	DrawingTransform() :
		translation(0.0f, 0.0f),
		flip(NO_FLIP),
		alpha(1.0f),
		scale(1.0f)
	{}
};

#endif