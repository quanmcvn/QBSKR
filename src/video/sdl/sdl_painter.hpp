#ifndef HEADER_QBSKR_VIDEO_SDL_SDL_PAINTER_HPP
#define HEADER_QBSKR_VIDEO_SDL_SDL_PAINTER_HPP

#include <optional>

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
	std::optional<SDL_Rect> m_cliprect;

public:
	SDLPainter(Renderer& renderer, SDL_Renderer* sdl_renderer);

	virtual void draw_texture(const TextureRequest& request) override;
	virtual void draw_filled_rect(const FilledRectRequest& request) override;

	virtual void set_clip_rect(const Rect& rect) override;
	virtual void clear_clip_rect() override;
};

#endif