#ifndef HEADER_QBSKR_VIDEO_SDL_SDL_PAINTER_HPP
#define HEADER_QBSKR_VIDEO_SDL_SDL_PAINTER_HPP

#include "video/painter.hpp"

class Renderer;
struct TextureRequest;
struct SDL_Renderer;

class SDLPainter : public Painter {
private:
	SDLPainter(const SDLPainter&) = delete;
	SDLPainter& operator=(const SDLPainter&) = delete;

private:
	Renderer& m_renderer;
	SDL_Renderer* m_sdl_renderer;

public:
	SDLPainter(Renderer& renderer, SDL_Renderer* sdl_renderer);

	virtual void draw_texture(const TextureRequest& request) override;
};

#endif