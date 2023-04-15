#include "video/sdl/sdl_painter.hpp"

#include <math.h>

#include "math/interpolate.hpp"
#include "util/log.hpp"
#include "video/sdl/sdl_texture.hpp"
#include "video/drawing_request.hpp"
#include "video/renderer.hpp"
#include "video/video_system.hpp"

namespace {

	// the reason it belongs here but not in Rectf is because it's only used in this specific situation
	// the Rect::to_sdl() is 1-1 mapping but this is not
	SDL_Rect to_sdl_rect(const Rectf& rectf)
	{
		SDL_Rect sdl_rect;

		// floorf is because of int(-0.99f) = int(-0.01f) = int(0.01f) = int(0.99f) = 0
		// => a jump in coord at 0
		sdl_rect.x = static_cast<int>(floorf(rectf.get_left()));
		sdl_rect.y = static_cast<int>(floorf(rectf.get_top()));

		sdl_rect.w = static_cast<int>(roundf(rectf.get_width()));
		sdl_rect.h = static_cast<int>(roundf(rectf.get_height()));

		return sdl_rect;
	}
};

SDLPainter::SDLPainter(Renderer& renderer, SDL_Renderer* sdl_renderer) :
	m_renderer(renderer),
	m_sdl_renderer(sdl_renderer),
	m_cliprect()
{}

void SDLPainter::draw_texture(const TextureRequest& request) 
{
	const auto& texture = static_cast<const SDLTexture&>(*request.texture);

	assert(request.srcrects.size() == request.dstrects.size());

	for (size_t i = 0; i < request.srcrects.size(); ++i) {
		const SDL_Rect& src_rect = to_sdl_rect(request.srcrects[i]);
		const SDL_Rect& dst_rect = to_sdl_rect(request.dstrects[i]);

		Uint8 r = static_cast<Uint8>(request.color.red * 255);
		Uint8 g = static_cast<Uint8>(request.color.green * 255);
		Uint8 b = static_cast<Uint8>(request.color.blue * 255);
		Uint8 a = static_cast<Uint8>(request.alpha * request.color.alpha * 255);

		SDL_SetTextureColorMod(texture.get_texture(), r, g, b);
		SDL_SetTextureAlphaMod(texture.get_texture(), a);

		SDL_RendererFlip flip = SDL_FLIP_NONE;
		if (request.flip & HORIZONTAL_FLIP) flip = static_cast<SDL_RendererFlip>(flip | SDL_FLIP_HORIZONTAL);
		if (request.flip & VERTICAL_FLIP) flip = static_cast<SDL_RendererFlip>(flip | SDL_FLIP_VERTICAL);

		SDL_RenderCopyEx(m_sdl_renderer, texture.get_texture(),
		                 &src_rect, &dst_rect,
		                 request.angles[i], nullptr, flip);
	}
}

void SDLPainter::draw_filled_rect(const FilledRectRequest& request)
{
	SDL_Rect rect = to_sdl_rect(request.rect);

	Uint8 r = static_cast<Uint8>(request.color.red * 255);
	Uint8 g = static_cast<Uint8>(request.color.green * 255);
	Uint8 b = static_cast<Uint8>(request.color.blue * 255);
	Uint8 a = static_cast<Uint8>(request.alpha * request.color.alpha * 255);

	SDL_SetRenderDrawBlendMode(m_sdl_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(m_sdl_renderer, r, g, b, a);
	SDL_RenderFillRect(m_sdl_renderer, &rect);
}

void SDLPainter::draw_inverse_ellipse(const InverseEllipseRequest& request)
{
	float center_x = request.center_pos.x;
	float width = request.size.width;
	float height = request.size.height;

	int top = static_cast<int>(request.center_pos.y - (height / 2));

	const Viewport& viewport = VideoSystem::current()->get_viewport();

	const int MAX_SLICES = 256;
	int slices = std::min(static_cast<int>(height), MAX_SLICES);
	std::vector<SDL_Rect> rects(2 * (slices + 1));

	// drawing the core of the inverse ellipse
	// draw from top to bottom
	// filling everything but the ellipse inside
	for (int i = 0; i < slices; ++ i) {
		int xoff = static_cast<int>(interpolate::half_ellipse_vertical_ease(
			static_cast<float>(i) / static_cast<float>(slices - 1),
			0, width / 2
		));

		SDL_Rect& left  = rects[2 * i + 0];
		SDL_Rect& right = rects[2 * i + 1];

		left.x = 0;
		left.y = top + (i * static_cast<int>(height) / slices);
		left.w = static_cast<int>(center_x) - xoff;
		left.h = top + ((i + 1) * static_cast<int>(height) / slices) - left.y;

		right.x = static_cast<int>(center_x) + xoff;
		right.y = left.y;
		right.w = viewport.get_screen_width() - right.x;
		right.h = left.h;
	}

	// filling the top and bottom
	SDL_Rect& top_rect    = rects[2 * slices + 0];
	SDL_Rect& bottom_rect = rects[2 * slices + 1];

	top_rect.x = 0;
	top_rect.y = 0;
	top_rect.w = viewport.get_screen_width();
	top_rect.h = top;

	bottom_rect.x = 0;
	bottom_rect.y = top + static_cast<int>(height);
	bottom_rect.w = viewport.get_screen_width();
	bottom_rect.h = viewport.get_screen_height() - bottom_rect.y;

	Uint8 r = static_cast<Uint8>(request.color.red * 255);
	Uint8 g = static_cast<Uint8>(request.color.green * 255);
	Uint8 b = static_cast<Uint8>(request.color.blue * 255);
	Uint8 a = static_cast<Uint8>(request.color.alpha * 255);

	SDL_SetRenderDrawColor(m_sdl_renderer, r, g, b, a);
	// using &rects[0] should work here
	SDL_RenderFillRects(m_sdl_renderer, &rects[0], 2 * (slices + 1));
}

void SDLPainter::set_clip_rect(const Rect& rect)
{
	m_cliprect = rect.to_sdl();

	if (SDL_RenderSetClipRect(m_sdl_renderer, &*m_cliprect) < 0) {
		log_warning << "SDLPainter::set_clip_rect(): SDL_RenderSetClipRect() failed: " << SDL_GetError() << std::endl;
	}
}

void SDLPainter::clear_clip_rect()
{
	m_cliprect.reset();

	if (SDL_RenderSetClipRect(m_sdl_renderer, nullptr) < 0) {
		log_warning << "SDLPainter::clear_clip_rect(): SDL_RenderSetClipRect() failed: " << SDL_GetError() << std::endl;
	}
}