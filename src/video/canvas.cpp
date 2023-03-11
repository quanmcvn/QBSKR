#include "video/canvas.hpp"

#include "video/drawing_context.hpp"
#include "video/painter.hpp"
#include "video/renderer.hpp"
#include "video/surface.hpp"

Canvas::~Canvas()
{}

Canvas::Canvas(DrawingContext& drawing_context, std::vector<std::unique_ptr<DrawingRequest>>& request_holder):
	m_drawing_context(drawing_context),
	m_request_holder(request_holder),
	m_requests()
{
	m_requests.reserve(100); // arbitrary choice
}

void Canvas::render(Renderer& renderer)
{
	std::stable_sort(m_requests.begin(), m_requests.end(), 
	                [](const DrawingRequest* lhs, const DrawingRequest* rhs) {
	                	return lhs->layer < rhs->layer;
	                });
	
	Painter& painter = renderer.get_painter();

	for (const auto& it : m_requests) {
		const DrawingRequest& request = *it;

		// to not renderer out of scope
		painter.set_clip_rect(request.viewport);

		switch (request.type) {
			case TEXTURE:
				painter.draw_texture(static_cast<const TextureRequest&>(request));
				break;
		}
	}
}

void Canvas::draw_surface(const SurfacePtr& surface, const Vector& position, int layer) 
{
	if (!surface) return;

	const auto& cliprect = m_drawing_context.get_cliprect();

	if (position.x > cliprect.get_right()) return;
	if (position.y > cliprect.get_bottom()) return;
	if (position.x + static_cast<float>(surface->get_width()) < cliprect.get_left()) return;
	if (position.y + static_cast<float>(surface->get_height()) < cliprect.get_top()) return;

	m_request_holder.emplace_back(std::make_unique<TextureRequest>());

	TextureRequest* request = static_cast<TextureRequest*>(m_request_holder.back().get());

	request->type = TEXTURE;
	request->layer = layer;
	request->viewport = m_drawing_context.get_viewport();

	assert(surface->get_texture());
	assert(surface->get_texture().get());

	request->texture = surface->get_texture().get();
	request->srcrects.emplace_back(Rectf(surface->get_region()));
	request->dstrects.emplace_back(Rectf(apply_translate(position) * scale(),
	                               Sizef(static_cast<float>(surface->get_width()) * scale(),
	                                     static_cast<float>(surface->get_height()) * scale())));

	m_requests.push_back(static_cast<DrawingRequest*>(request));
}


void Canvas::clear()
{
	m_requests.clear();
}

Vector Canvas::apply_translate(const Vector& pos) const
{
	Vector translation = m_drawing_context.get_translation();
	return (pos - translation) + Vector(static_cast<float>(m_drawing_context.get_viewport().left),
	                                    static_cast<float>(m_drawing_context.get_viewport().top));
}

float Canvas::scale() const
{
	return m_drawing_context.get_scale();
}