#ifndef HEADER_QBSKR_VIDEO_RENDERER_HPP
#define HEADER_QBSKR_VIDEO_RENDERER_HPP

#include "video/texture_ptr.hpp"

class Painter;

/**
 * Generic renderer class
 * Renderer is interface (?) (kind of)
*/
class Renderer {
public:
	virtual ~Renderer() {}

	virtual void start_draw() = 0;
	virtual void end_draw() = 0;

	virtual Painter& get_painter() = 0;
};

#endif