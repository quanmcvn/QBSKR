#ifndef HEADER_QBSKR_VIDEO_PAINTER_HPP
#define HEADER_QBSKR_VIDEO_PAINTER_HPP

#include "math/rect.hpp"

struct DrawingRequest;
struct TextureRequest;
struct FilledRectRequest;

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
	virtual void draw_filled_rect(const FilledRectRequest& request) = 0;

	virtual void set_clip_rect(const Rect& rect) = 0;
	virtual void clear_clip_rect() = 0;
};

#endif