#ifndef HEADER_QBSKR_VIDEO_TTF_SURFACE_MANAGER_HPP
#define HEADER_QBSKR_VIDEO_TTF_SURFACE_MANAGER_HPP

#include <map>
#include <string>

#include "util/currenton.hpp"
#include "video/ttf_surface.hpp"

class TTFFont;

/**
 * Class to cache loaded text surface
 * It's different from TextureManager it's only cache recently used text
 * Use a quite stupid algorithm (let the std::map do the heavy lifting tho)
*/
class TTFSurfaceManager final : public Currenton<TTFSurfaceManager> {
public:
	TTFSurfaceManager();

private:
	TTFSurfaceManager(const TTFSurfaceManager&) = delete;
	TTFSurfaceManager& operator=(const TTFSurfaceManager&) = delete;

private:
	struct CacheEntry {
		CacheEntry();
		CacheEntry(const TTFSurfacePtr& ttf_surface_);

		// to make CacheEntry key-able (english where?)
		bool operator<(const CacheEntry& other) const;

		TTFSurfacePtr ttf_surface;
		float last_access;
	};

private:
	// could have been TTF_Font* instead of void*
	using Key = std::tuple<void*, std::string>;
	std::map<Key, CacheEntry> m_cache;
	std::map<CacheEntry, Key> m_cache_support;

public:
	// add in if not exist in cache (similar to std::map::operator[])
	TTFSurfacePtr get_surface(const TTFFont& font, const std::string& text);

private:
	void cache_cleanup_step();
};

#endif