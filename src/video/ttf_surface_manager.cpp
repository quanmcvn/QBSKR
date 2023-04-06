#include "video/ttf_surface_manager.hpp"

#include "qbskr/globals.hpp"
#include "video/ttf_font.hpp"

TTFSurfaceManager::TTFSurfaceManager() :
	m_cache(),
	m_cache_support()
{}

TTFSurfaceManager::CacheEntry::CacheEntry() :
	ttf_surface(),
	last_access()
{}

TTFSurfaceManager::CacheEntry::CacheEntry(const TTFSurfacePtr& ttf_surface_) :
	ttf_surface(ttf_surface_),
	last_access(g_game_time)
{}

bool TTFSurfaceManager::CacheEntry::operator<(const CacheEntry& other) const
{
	if (last_access != other.last_access) return last_access < other.last_access;
	return std::less<void*>()(static_cast<void*>(ttf_surface.get()), static_cast<void*>(other.ttf_surface.get()));
}

TTFSurfacePtr TTFSurfaceManager::get_surface(const TTFFont& font, const std::string& text)
{
	auto key = Key(font.get_ttf_font(), text);
	auto it = m_cache.find(key);
	if (it == m_cache.end()) {
		cache_cleanup_step();
		TTFSurfacePtr ttf_surface = TTFSurface::create(font, text);
		m_cache[key] = CacheEntry(ttf_surface);
		m_cache_support[m_cache[key]] = key;
		return ttf_surface;
	} else {
		m_cache_support.erase(it->second);
		it->second.last_access = g_game_time;
		m_cache_support[it->second] = it->first;
		return it->second.ttf_surface;
	}
}

void TTFSurfaceManager::cache_cleanup_step()
{
	static const float magic_number = 10.0f;
	while (!m_cache_support.empty() && g_game_time - m_cache_support.begin()->first.last_access > magic_number) {
		m_cache.erase(m_cache_support.begin()->second);
		m_cache_support.erase(m_cache_support.begin());
	}
}