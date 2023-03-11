#include "video/sdl/sdl_renderer.hpp"

#include "video/sdl/sdl_video_system.hpp"

SDLRenderer::SDLRenderer(SDLVideoSystem& video_system, SDL_Renderer* renderer) :
	m_video_system(video_system),
	m_renderer(renderer),
	m_painter(*this, m_renderer)
{}

SDLRenderer::~SDLRenderer()
{}

void SDLRenderer::start_draw()
{
	const Rect& viewport = m_video_system.get_viewport().get_rect();
	const Vector& scale = m_video_system.get_viewport().get_scale();

	SDL_Rect sdl_viewport = viewport.to_sdl();

	// scale down to 1 because SDL_RenderSetViewport() respects scale
	SDL_RenderSetScale(m_renderer, 1.0f, 1.0f);
	SDL_RenderSetViewport(m_renderer, &sdl_viewport);
	SDL_RenderSetScale(m_renderer, scale.x, scale.y);

	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderClear(m_renderer);
}

void SDLRenderer::end_draw()
{}

void SDLRenderer::present() 
{
	SDL_RenderPresent(m_renderer);
}