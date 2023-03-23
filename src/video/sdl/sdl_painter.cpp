#include "video/sdl/sdl_painter.hpp"

#include <math.h>

#include "util/log.hpp"
#include "video/drawing_request.hpp"
#include "video/renderer.hpp"
#include "video/sdl/sdl_texture.hpp"

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