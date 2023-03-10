#ifndef HEADER_QBSKR_VIDEO_SDL_SDL_TEXTURE_HPP
#define HEADER_QBSKR_VIDEO_SDL_SDL_TEXTURE_HPP

#include "video/texture.hpp"

#include <SDL.h>

struct SDL_Texture;

/**
 * Wrapper class for SDL_Texture*
*/
class SDLTexture final : public Texture {
public:
	~SDLTexture() override;

private:
	SDLTexture(const SDLTexture&) = delete;
	SDLTexture& operator=(const SDLTexture&) = delete;

private:
	SDL_Texture* m_texture;
	int m_width;
	int m_height;

public:
	SDLTexture(SDL_Texture* texture, int width, int height);
	SDLTexture(const SDL_Surface& image);

public:
	virtual int get_texture_width() const override { return m_width; }
	virtual int get_texture_height() const override { return m_height; }

	virtual int get_image_width() const override { return m_width; }
	virtual int get_image_height() const override { return m_height; }

	SDL_Texture* get_texture() const { return m_texture; }
};

#endif