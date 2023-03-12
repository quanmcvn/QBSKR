#ifndef HEADER_QBSKR_VIDEO_SURFACE_HPP
#define HEADER_QBSKR_VIDEO_SURFACE_HPP

#include <string>
#include <optional>

#include "math/rect.hpp"
#include "video/flip.hpp"
#include "video/surface_ptr.hpp"
#include "video/texture_ptr.hpp"

/**
 * This class is a rectangular image. It basically holds a TexturePtr and 
 * some information that specify a rectangular area of the texture
*/
class Surface final {
public:
	~Surface();

private:
	Surface& operator=(const Surface&) = delete;

public:
	static SurfacePtr from_texture(const TexturePtr& texture);
	static SurfacePtr from_file(const std::string& filename, const std::optional<Rect>& rect = std::nullopt);

public:
	Surface(const TexturePtr& texture, const Flip flip, const std::string& filename = "");
	Surface(const TexturePtr& texture, const Rect& region, const Flip flip, const std::string& filename = "");

private:
	const TexturePtr m_texture;
	const Rect m_region;
	// note that the Surface itself IS NOT flipped, this is used to produce the DrawingRequest::flip
	const Flip m_flip;
	const std::string m_filename;

public:
	// clone the surface and flip it
	SurfacePtr clone_flip(const Flip flip = NO_FLIP) const;

	TexturePtr get_texture() const;
	Rect get_region() const;
	int get_width() const;
	int get_height() const;
	Flip get_flip() const;
};

#endif