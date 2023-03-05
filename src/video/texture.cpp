#include "video/texture.hpp"

#include "video/texture_manager.hpp"

Texture::Texture() :
	m_cache_key()
{}

Texture::~Texture()
{
	if (TextureManager::current() && m_cache_key) {
		// free cached texture
		TextureManager::current()->free_cache_entry(*m_cache_key);
	}
}