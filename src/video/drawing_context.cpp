#include "video/drawing_context.hpp"

#include "video/video_system.hpp"

DrawingContext::DrawingContext(VideoSystem& video_system, std::vector<std::unique_ptr<DrawingRequest>>& request_holder) :
	m_video_system(video_system),
	m_request_holder(request_holder),
	m_viewport(0, 0, m_video_system.get_viewport().get_screen_width(), m_video_system.get_viewport().get_screen_height()),
	m_transform_stack(1),
	m_canvas(*this, request_holder)
{}

DrawingContext::~DrawingContext()
{
	clear();
}

DrawingTransform& DrawingContext::transform() 
{
	assert(!m_transform_stack.empty());
	return m_transform_stack.back();
}

const DrawingTransform& DrawingContext::transform() const
{
	assert(!m_transform_stack.empty());
	return m_transform_stack.back();
}

void DrawingContext::push_transform()
{
	m_transform_stack.push_back(transform());
}

void DrawingContext::pop_transform()
{
	m_transform_stack.pop_back();
	assert(!m_transform_stack.empty());
}

const Vector& DrawingContext::get_translation() const { return transform().translation; }
void DrawingContext::set_translation(const Vector& new_translation) { transform().translation = new_translation; }

float DrawingContext::get_scale() const { return transform().scale; }
void DrawingContext::set_scale(float new_scale) { transform().scale = new_scale; }
void DrawingContext::scale(float scale) { transform().scale *= scale; }

Flip DrawingContext::get_flip() const { return transform().flip; }
void DrawingContext::set_flip(Flip flip) { transform().flip = flip; }

float DrawingContext::get_alpha() const { return transform().alpha; }
void DrawingContext::set_alpha(float alpha) { transform().alpha = alpha; }

const Rect DrawingContext::get_viewport() const { return m_viewport; }
void DrawingContext::set_viewport(const Rect& viewport) { m_viewport = viewport; }

int DrawingContext::get_width() const { return static_cast<int>(static_cast<float>(m_viewport.get_width()) / transform().scale); }
int DrawingContext::get_height() const { return static_cast<int>(static_cast<float>(m_viewport.get_height()) / transform().scale); }
Vector DrawingContext::get_size() const { return Vector(static_cast<float>(get_width()), static_cast<float>(get_height())) * transform().scale; }
Rectf DrawingContext::get_rect() const { return Rectf(Vector(0, 0), get_size()); }

Rectf DrawingContext::get_cliprect() const
{
	return Rectf(get_translation().x, get_translation().y,
	             get_translation().x + static_cast<float>(m_viewport.get_width()) / transform().scale,
	             get_translation().y + static_cast<float>(m_viewport.get_height()) / transform().scale);
}

Canvas& DrawingContext::get_canvas() { return m_canvas; }

void DrawingContext::clear()
{
	m_canvas.clear();
}