#include "video/compositor.hpp"

#include "video/drawing_context.hpp"
#include "video/painter.hpp"
#include "video/renderer.hpp"
#include "video/video_system.hpp"

Compositor::~Compositor()
{
	m_drawing_contexts.clear();
	m_request_holder.clear();
}

Compositor::Compositor(VideoSystem& video_system) :
	m_video_system(video_system)
{}

DrawingContext& Compositor::make_context()
{
	m_drawing_contexts.emplace_back(std::make_unique<DrawingContext>(m_video_system, m_request_holder));
	return *m_drawing_contexts.back();
}

void Compositor::render()
{
	auto& renderer = m_video_system.get_renderer();

	renderer.start_draw();

	auto& painter = renderer.get_painter();

	for (auto& drawing_context : m_drawing_contexts) {
		painter.set_clip_rect(drawing_context->get_viewport());
		drawing_context->get_canvas().render(renderer);
		painter.clear_clip_rect();
	}

	renderer.end_draw();

	// clean up
	for (auto& drawing_context : m_drawing_contexts) {
		drawing_context->clear();
	}
	m_video_system.present();
}