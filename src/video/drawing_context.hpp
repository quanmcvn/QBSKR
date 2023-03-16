#ifndef HEADER_QBSKR_VIDEO_DRAWING_CONTEXT_HPP
#define HEADER_QBSKR_VIDEO_DRAWING_CONTEXT_HPP

#include <memory>
#include <vector>

#include "math/rect.hpp"
#include "math/rectf.hpp"
#include "video/canvas.hpp"
#include "video/drawing_transform.hpp"

class VideoSystem;
struct DrawingRequest;

/**
 * This class holds Canvas(es) and transform stack
*/
class DrawingContext {
public:
	~DrawingContext();

private:
	DrawingContext(const DrawingContext&) = delete;
	DrawingContext& operator=(const DrawingContext&) = delete;

private:
	VideoSystem& m_video_system;
	// holds the memory of all the drawing requests
	// it is shared between everything (Canvas(es), DrawingContext(s))
	std::vector<std::unique_ptr<DrawingRequest>>& m_request_holder;

	Rect m_viewport;
	std::vector<DrawingTransform> m_transform_stack;

	Canvas m_canvas;

public:
	DrawingContext(VideoSystem& video_system, std::vector<std::unique_ptr<DrawingRequest>>& request_holder);

private:
	DrawingTransform& transform();
	const DrawingTransform& transform() const;

public:
	void push_transform();
	void pop_transform();

	const Vector& get_translation() const;
	void set_translation(const Vector& new_translation);
	
	float get_scale() const;
	void set_scale(float new_scale);
	void scale(float scale);
	
	Flip get_flip() const;
	void set_flip(Flip flip);
	
	float get_alpha() const;
	void set_alpha(float alpha);

	const Rect get_viewport() const;
	void set_viewport(const Rect& viewport);

	int get_width() const;
	int get_height() const;
	Vector get_size() const;
	Rectf get_rect() const;

	Rectf get_cliprect() const;
	Canvas& get_canvas();

	void clear();
};

#endif