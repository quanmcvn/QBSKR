#ifndef HEADER_QBSKR_VIDEO_CANVAS_HPP
#define HEADER_QBSKR_VIDEO_CANVAS_HPP

#include <vector>
#include <memory>

#include "video/drawing_request.hpp"
#include "video/font_ptr.hpp"
#include "video/font.hpp"
#include "video/layer.hpp"
#include "video/surface_ptr.hpp"

class DrawingContext;
class Renderer;
struct DrawingRequest;

/**
 *  This class passes DrawingRequests to Painter
 *  Use user-defined layer to make draw more flexible
*/
class Canvas final {
public:
	~Canvas();

private:
	Canvas(const Canvas&) = delete;
	Canvas& operator=(const Canvas&) = delete;

public:
	Canvas(DrawingContext& drawing_context, std::vector<std::unique_ptr<DrawingRequest>>& request_holder);

private:
	DrawingContext& m_drawing_context;
	// sholds the memory of all the drawing requests
	// it is shared between everything (Canvas(es), DrawingContext(s))
	std::vector<std::unique_ptr<DrawingRequest>>& m_request_holder;
	// the actual requests that are going to be passed to Painter
	std::vector<DrawingRequest*> m_requests;

public:
	void render(Renderer& renderer);

	void draw_surface(const SurfacePtr& surface, const Vector& position, int layer);
	void draw_surface(const SurfacePtr& surface, const Vector& position, float angle, const Color& color, int layer);

	void draw_surface_scaled(const SurfacePtr& surface, const Rectf& dstrect, const Color& color, int layer);
	void draw_surface_part(const SurfacePtr& surface, const Rectf& srcrect, const Rectf& dstrect, const Color& color, int layer);

	void draw_filled_rect(const Rectf& rect, const Color& color, int layer);
	void draw_inverse_ellipse(const Vector& center_pos, const Sizef& size, const Color& color, int layer);
	void draw_text(const FontPtr& font, const std::string& text,
	               const Vector& position, FontAlignment alignment, int layer, const Color& color = Color(1.0f, 1.0f, 1.0f, 1.0f));
	void clear();

	DrawingContext& get_drawing_context();

private:
	Vector apply_translate(const Vector& pos) const;
	float scale() const;
};

#endif