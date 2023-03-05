#include "video/sdl/sdl_renderer.hpp"

SDLRenderer::SDLRenderer(SDL_Renderer* renderer) :
	m_renderer(renderer),
	m_painter(*this, m_renderer)
{}

SDLRenderer::~SDLRenderer()
{}

void SDLRenderer::start_draw()
{
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderClear(m_renderer);
}

void SDLRenderer::end_draw()
{}

void SDLRenderer::present() 
{
	SDL_RenderPresent(m_renderer);
}