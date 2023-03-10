#ifndef HEADER_QBSKR_VIDEO_SDL_SDL_SURFACE_PTR_HPP
#define HEADER_QBSKR_VIDEO_SDL_SDL_SURFACE_PTR_HPP

#include <SDL.h>

/**
 * Wrapper class for SDL_Surface*
*/
class SDLSurfacePtr final {
private:
	SDL_Surface* m_surface;

public:
	SDLSurfacePtr() :
		m_surface(nullptr)
	{}

	~SDLSurfacePtr()
	{
		SDL_FreeSurface(m_surface);
	}

private:
	SDLSurfacePtr(const SDLSurfacePtr&) = delete;
	SDLSurfacePtr& operator=(const SDLSurfacePtr&) = delete;

public:
	SDLSurfacePtr(SDLSurfacePtr&& other) noexcept :
		m_surface(other.m_surface)
	{
		other.m_surface = nullptr;
	}

	SDLSurfacePtr& operator=(SDLSurfacePtr&& other) noexcept
	{
		if (this != &other) {
			m_surface = other.m_surface;
			other.m_surface = nullptr;
		}
		return *this;
	}

public:
	explicit SDLSurfacePtr(SDL_Surface* surface) :
		m_surface(surface)
	{}

	SDL_Surface& operator*()
	{
		return *m_surface;
	}

	const SDL_Surface& operator*() const
	{
		return *m_surface;
	}

	SDL_Surface* operator->()
	{
		return m_surface;
	}

	const SDL_Surface* operator->() const
	{
		return m_surface;
	}

	void reset(SDL_Surface* surface)
	{
		SDL_FreeSurface(m_surface);
		m_surface = surface;
	}

	SDL_Surface* get() const
	{
		return m_surface;
	}

	/**
	 * implicit cast to bool
	 * to use if (sdlsurfaceptr) / if (!sdlsurfaceptr)
	*/
	operator bool() {
		return m_surface;
	}
};

#endif