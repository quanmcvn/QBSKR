#ifndef HEADER_QBSKR_VIDEO_PAINTER_HPP
#define HEADER_QBSKR_VIDEO_PAINTER_HPP

#include "math/rect.hpp"

struct DrawingRequest;
struct TextureRequest;

/**
 * Generic painter class
 * This class only draw things when given request
*/
class Painter {
public:
	Painter() {}
	virtual ~Painter() {}

public:
	virtual void draw_texture(const TextureRequest& request) = 0;
};

#endif