#ifndef HEADER_QBSKR_VIDEO_SDL_SDL_VIDEO_SYSTEM_HPP
#define HEADER_QBSKR_VIDEO_SDL_SDL_VIDEO_SYSTEM_HPP

#include <memory>
#include <SDL.h>

#include "video/sdl/sdlbased_video_system.hpp"
#include "video/viewport.hpp"

class SDLRenderer;
class TextureManager;

/**
 * The SDL Video System
 * Renderer(s) included
*/ 
class SDLVideoSystem final : public SDLBasedVideoSystem {
public:
	SDLVideoSystem();
	~SDLVideoSystem() override;

private:
	SDLVideoSystem(const SDLVideoSystem&) = delete;
	SDLVideoSystem& operator=(const SDLVideoSystem&) = delete;

private:
	// the SDL_Renderer* tied with m_sdl_window
	std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> m_sdl_renderer;
	Viewport m_viewport;
	std::unique_ptr<SDLRenderer> m_renderer;
	std::unique_ptr<TextureManager> m_texture_manager;

public:
	virtual Renderer& get_renderer() const override;

	virtual TexturePtr new_texture(const SDL_Surface& image) override;

	virtual const Viewport& get_viewport() const override;
	virtual void apply_config() override;
	virtual void present() override;

private:
	void create_window();
};

#endif