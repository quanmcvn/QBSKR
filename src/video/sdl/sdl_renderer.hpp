#ifndef HEADER_QBSKR_VIDEO_SDL_SDL_RENDERER_hpp
#define HEADER_QBSKR_VIDEO_SDL_SDL_RENDERER_hpp

#include <SDL.h>

#include "video/sdl/sdl_painter.hpp"
#include "video/renderer.hpp"

class SDLVideoSystem;

/**
 * This class holds Painter and SDL_Renderer*
*/
class SDLRenderer final : public Renderer {
public:
	SDLRenderer(SDLVideoSystem& video_system, SDL_Renderer* renderer);
	~SDLRenderer();

private:
	SDLRenderer(const SDLRenderer&) = delete;
	SDLRenderer& operator=(const SDLRenderer&) = delete;

private:
	SDLVideoSystem& m_video_system;
	SDL_Renderer* m_renderer;
	SDLPainter m_painter;

public:
	virtual SDLPainter& get_painter() override { return m_painter; }

	virtual void start_draw() override;
	virtual void end_draw() override;

	SDL_Renderer* get_sdl_renderer() const { return m_renderer; }

	void present();
};

#endif