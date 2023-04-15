#ifndef HEADER_QBSKR_VIDEO_PAINTER_HPP
#define HEADER_QBSKR_VIDEO_PAINTER_HPP

#include "math/rect.hpp"

struct DrawingRequest;
struct TextureRequest;
struct FilledRectRequest;
struct InverseEllipseRequest;

/**
 * Generic painter class
 * This class only draw things when given request
 * Painter is interface (?) (kind of)
*/
class Painter {
public:
	Painter() {}
	virtual ~Painter() {}

public:
	virtual void draw_texture(const TextureRequest& request) = 0;
	virtual void draw_filled_rect(const FilledRectRequest& request) = 0;
	virtual void draw_inverse_ellipse(const InverseEllipseRequest& request) = 0;

	virtual void set_clip_rect(const Rect& rect) = 0;
	virtual void clear_clip_rect() = 0;
};

#endif