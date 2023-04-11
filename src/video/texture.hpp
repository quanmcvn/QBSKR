#ifndef HEADER_QBSKR_VIDEO_TEXTURE_HPP
#define HEADER_QBSKR_VIDEO_TEXTURE_HPP

#include <string>
#include <tuple>
#include <optional>

#include "math/rect.hpp"

/**
 * Abstract class for texture
 * Stores the texture width and height
*/
class Texture {
	friend class TextureManager;

public:
	virtual ~Texture();

private:
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

protected:
	Texture();

public:
	/** filename, left, top, right, bottom */
	using Key = std::tuple<std::string, Rect>;

public:
	virtual int get_texture_width() const = 0;
	virtual int get_texture_height() const = 0;

	virtual int get_image_width() const = 0;
	virtual int get_image_height() const = 0;

private:
	std::optional<Key> m_cache_key;
};

#endif