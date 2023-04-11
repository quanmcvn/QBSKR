#ifndef HEADER_QBSKR_VIDEO_TTF_SURFACE_HPP
#define HEADER_QBSKR_VIDEO_TTF_SURFACE_HPP

#include <memory>
#include <string>

#include "math/vector.hpp"
#include "video/surface_ptr.hpp"

class TTFFont;
class TTFSurface;

using TTFSurfacePtr = std::shared_ptr<TTFSurface>;

/**
 *  Class for rendered text
*/
class TTFSurface final {
private:
	TTFSurface(const TTFSurface&) = delete;
	TTFSurface& operator=(const TTFSurface&) = delete;

private:
	SurfacePtr m_surface;
	Vector m_offset;

public:
	TTFSurface(const SurfacePtr& surface, const Vector& offset);

public:
	static TTFSurfacePtr create(const TTFFont& font, const std::string& text);

public:
	SurfacePtr get_surface() const;
	Vector get_offset() const;

	int get_width() const;
	int get_height() const;
};

#endif