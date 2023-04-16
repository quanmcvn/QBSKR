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
	std::stable_sort(
		m_requests.begin(), 
		m_requests.end(), 
		[] (const DrawingRequest* lhs, const DrawingRequest* rhs) {
			return lhs->layer < rhs->layer;
		}
	);
	
	Painter& painter = renderer.get_painter();

	for (const auto& it : m_requests) {
		const DrawingRequest& request = *it;

		// to not renderer out of scope
		painter.set_clip_rect(request.viewport);

		switch (request.type) {
			case TEXTURE:
				painter.draw_texture(static_cast<const TextureRequest&>(request));
				break;

			case FILLED_RECT:
				painter.draw_filled_rect(static_cast<const FilledRectRequest&>(request));
				break;

			case INVERSE_ELLIPSE:
				painter.draw_inverse_ellipse(static_cast<const InverseEllipseRequest&>(request));
				break;

			default:
				assert(false);
				break;
		}
	}
}

void Canvas::draw_surface(const SurfacePtr& surface, const Vector& position, int layer) 
{
	draw_surface(surface, position, 0.0f, Color(1.0f, 1.0f, 1.0f), layer);
}

void Canvas::draw_surface(const SurfacePtr& surface, const Vector& position, float angle, const Color& color, int layer)
{
	if (!surface) return;

	const auto& cliprect = m_drawing_context.get_cliprect();

	if (position.x > cliprect.get_right()) return;
	if (position.y > cliprect.get_bottom()) return;
	if (position.x + static_cast<float>(surface->get_width()) < cliprect.get_left()) return;
	if (position.y + static_cast<float>(surface->get_height()) < cliprect.get_top()) return;

	m_request_holder.emplace_back(static_cast<std::unique_ptr<DrawingRequest>>(std::make_unique<TextureRequest>()));

	TextureRequest* request = static_cast<TextureRequest*>(m_request_holder.back().get());

	request->type = TEXTURE;
	request->layer = layer;
	request->flip = m_drawing_context.get_flip() ^ surface->get_flip();
	request->alpha = m_drawing_context.get_alpha();
	request->viewport = m_drawing_context.get_viewport();

	request->texture = surface->get_texture().get();
	request->srcrects.emplace_back(Rectf(surface->get_region()));
	request->dstrects.emplace_back(Rectf(apply_translate(position) * scale(),
	                                     Sizef(static_cast<float>(surface->get_width()) * scale(),
	                                           static_cast<float>(surface->get_height()) * scale())));
	request->angles.push_back(angle);
	request->color = color;

	m_requests.push_back(static_cast<DrawingRequest*>(request));
}

void Canvas::draw_surface_scaled(const SurfacePtr& surface, const Rectf& dstrect, const Color& color, int layer)
{
	draw_surface_part(
		surface, 
		Rectf(0, 0, 
			static_cast<float>(surface->get_width()), 
			static_cast<float>(surface->get_height())
		),
		dstrect, color, layer
	);
}

void Canvas::draw_surface_part(const SurfacePtr& surface, const Rectf& srcrect, const Rectf& dstrect, const Color& color, int layer)
{
	if (!surface) return;

	m_request_holder.emplace_back(static_cast<std::unique_ptr<DrawingRequest>>(std::make_unique<TextureRequest>()));

	TextureRequest* request = static_cast<TextureRequest*>(m_request_holder.back().get());

	request->type = TEXTURE;
	request->layer = layer;
	request->flip = m_drawing_context.get_flip() ^ surface->get_flip();
	request->alpha = m_drawing_context.get_alpha();
	request->viewport = m_drawing_context.get_viewport();

	request->srcrects.emplace_back(srcrect);
	request->dstrects.emplace_back(apply_translate(dstrect.p1())*scale(), dstrect.get_size()*scale());
	request->angles.emplace_back(0.0f);
	request->texture = surface->get_texture().get();
	request->color = color;

	m_requests.push_back(static_cast<DrawingRequest*>(request));
}

void Canvas::draw_filled_rect(const Rectf& rect, const Color& color, int layer)
{
	m_request_holder.emplace_back(static_cast<std::unique_ptr<DrawingRequest>>(std::make_unique<FilledRectRequest>()));

	FilledRectRequest* request = static_cast<FilledRectRequest*>(m_request_holder.back().get());
	
	request->type = FILLED_RECT;
	request->layer = layer;

	request->flip = m_drawing_context.get_flip();
	request->alpha = m_drawing_context.get_alpha();
	request->viewport = m_drawing_context.get_viewport();

	request->rect = Rectf(apply_translate(rect.p1()) * scale(), rect.get_size() * scale());
	request->color = color;
	request->color.alpha = color.alpha * m_drawing_context.get_alpha();

	m_requests.push_back(static_cast<DrawingRequest*>(request));
}

void Canvas::draw_inverse_ellipse(const Vector& center_pos, const Sizef& size, const Color& color, int layer)
{
	m_request_holder.emplace_back(static_cast<std::unique_ptr<DrawingRequest>>(std::make_unique<InverseEllipseRequest>()));

	InverseEllipseRequest* request = static_cast<InverseEllipseRequest*>(m_request_holder.back().get());

	request->type = INVERSE_ELLIPSE;
	request->layer = layer;

	request->flip = m_drawing_context.get_flip();
	request->alpha = m_drawing_context.get_alpha();
	request->viewport = m_drawing_context.get_viewport();

	request->center_pos   = apply_translate(center_pos) * scale();
	request->color        = color;
	request->color.alpha  = color.alpha * m_drawing_context.get_alpha();
	request->size         = size * scale();

	m_requests.push_back(static_cast<DrawingRequest*>(request));
}

void Canvas::draw_text(const FontPtr& font, const std::string& text,
                       const Vector& position, FontAlignment alignment, int layer, const Color& color)
{
	font->draw_text(*this, text, position, alignment, layer, color);
}

void Canvas::clear()
{
	m_requests.clear();
}

DrawingContext& Canvas::get_drawing_context() { return m_drawing_context; }

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