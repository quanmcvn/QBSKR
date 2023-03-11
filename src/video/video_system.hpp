#ifndef HEADER_QBSKR_VIDEO_VIDEO_SYSTEM_HPP
#define HEADER_QBSKR_VIDEO_VIDEO_SYSTEM_HPP

#include <SDL.h>

#include "math/size.hpp"
#include "util/currenton.hpp"
#include "video/texture_ptr.hpp"
#include "video/viewport.hpp"

class Renderer;

/** 
 * How all of this (suppose) to work:
 *   - VideoSystem is an abstract (?) (kind of) class (Java OOP?)
 *   - Renderer, Painter are interfaces (?) (kind of)
 *   - SDLBasedVideoSystem is the class holding SDL_Window (to separate SDL_Window)
 *   - SDLVideoSystem is the class holding Renderer(s), implements VideoSystem
 *   - SDLRenderer/SDLPainter implement Renderer/Painter
 *   - Texture is abstract, TexturePtr is... abstract (not!)
 *   - SDLTexture is a wrapper around SDL_Texture*, implements Texture
 *   - SDLSurfacePtr is a wrapper around SDL_Surface*
 *     (What is this design...) 
 *     (I have good reasons: SDLTexture implements Texture so its name is fixed, 
 *     but SDLSurfacePtr exists on its own)
 *   - SDLSurfaceCreator is the class full of static functions to create SDLSurfacePtr
 *   - TextureManager is the class to get/cache textures by itself, but create textures through VideoSystem
*/

/**
 * Generic (?) video system class
*/
class VideoSystem : public Currenton<VideoSystem> {
public:
	enum VideoType {
		VIDEO_SDL,
		VIDEO_DEBUG
	};

	static std::unique_ptr<VideoSystem> create(VideoSystem::VideoType video_system);

public:
	VideoSystem();
	~VideoSystem() override;

	virtual Renderer& get_renderer() const = 0;

	virtual TexturePtr new_texture(const SDL_Surface& image) = 0;

	virtual const Viewport& get_viewport() const = 0;
	virtual void apply_config() = 0;
	// swap (flip (?)) the backbuffer to frontbuffer
	virtual void present() = 0; 
};

#endif