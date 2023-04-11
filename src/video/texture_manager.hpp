#ifndef HEADER_QBSKR_VIDEO_TEXTURE_MANAGER_HPP
#define HEADER_QBSKR_VIDEO_TEXTURE_MANAGER_HPP

#include <map>
#include <memory>
#include <ostream>
#include <set>
#include <string>
#include <vector>

#include "math/rect.hpp"
#include "util/currenton.hpp"
#include "video/sdl/sdl_surface_ptr.hpp"
#include "video/texture_ptr.hpp"
#include "video/texture.hpp"

struct SDL_Surface;
/**
 * This class stores images and provides way to get them
 * create image through VideoSystem
*/
class TextureManager final : public Currenton<TextureManager> {
public:
	friend class Texture;

public:
	TextureManager();
	~TextureManager() override;

private:
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

public:
	TexturePtr get(const std::string& filename);
	TexturePtr get(const std::string& filename, const std::optional<Rect>& rect);

	// void debug_print(std::ostream& os) const;

private:
	const SDL_Surface& get_surface(const std::string& filename);

	// to free cached textures
	void free_cache_entry(const Texture::Key& key);

	// on failure a dummy texture is returned and no exception is thrown
	TexturePtr create_image_texture(const std::string& filename) noexcept;
	// on failure a dummy texture is returned and no exception is thrown
	TexturePtr create_image_texture(const std::string& filename, const Rect& rect) noexcept;

	// throw exception on error
	TexturePtr create_image_texture_raw(const std::string& filename);
	// throw exception on error
	TexturePtr create_image_texture_raw(const std::string& filename, const Rect& rect);

	// create dummy texture in case of failure of create_image_texture_raw()
	TexturePtr create_dummy_texture();

private:
	// to cache
	std::map<Texture::Key, std::weak_ptr<Texture>> m_image_textures;
	// to cache
	std::map<std::string, SDLSurfacePtr> m_surfaces;
};

#endif