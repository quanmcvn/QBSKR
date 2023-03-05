#include "video/texture_manager.hpp"

#include <assert.h>
#include <sstream>

#include "util/log.hpp"
#include "video/sdl/sdl_surface_creator.hpp"
#include "video/texture_manager.hpp"
#include "video/video_system.hpp"

TextureManager::TextureManager() :
	m_image_textures(),
	m_surfaces()
{}

TextureManager::~TextureManager() 
{
	for (const auto& texture : m_image_textures) {
		if (!texture.second.expired()) {
			log_warning << "Texture '" << std::get<0>(texture.first) << "' not freed" << std::endl;
		}
	}
	m_image_textures.clear();
	m_surfaces.clear();
}

TexturePtr TextureManager::get(const std::string& filename)
{
	Texture::Key key(filename, Rect(0, 0, 0, 0));
	auto it = m_image_textures.find(key);

	TexturePtr texture;
	if (it != m_image_textures.end()) {
		texture = it->second.lock();
	}

	if (!texture) {
		texture = create_image_texture(filename);
		texture->m_cache_key = key;
		m_image_textures[key] = texture;
	}

	return texture;
}

void TextureManager::free_cache_entry(const Texture::Key& key)
{
	auto it = m_image_textures.find(key);
	if (it == m_image_textures.end()) {
		log_warning << "No cache entry for '" << std::get<0>(key) << "'" << std::endl;
	} else {
		assert(it->second.expired());
		m_image_textures.erase(it);
	}
}
TexturePtr TextureManager::create_image_texture(const std::string& filename) noexcept
{
	try {
		return create_image_texture_raw(filename);
	} catch (const std::exception& err) {
		log_warning << "Couldn't load texture '" << filename << "' (now using dummy texture): " << err.what() << std::endl;
		return create_dummy_texture();
	}
	return TexturePtr();
}

TexturePtr TextureManager::create_image_texture_raw(const std::string& filename)
{
	SDLSurfacePtr image = SDLSurfaceCreator::from_file(filename);
	if (!image) {
		std::ostringstream msg;
		msg << "Couldn't load image '" << filename << "' :" << SDL_GetError();
		throw std::runtime_error(msg.str());
	} else {
		TexturePtr texture = VideoSystem::current()->new_texture(*image);
		image.reset(nullptr);
		return texture;
	}
}

TexturePtr TextureManager::create_dummy_texture()
{
	// this (the relative filepath) is bad and I don't have a way to fix it
	// need more tools
	const std::string dummy_texture_filename = "../data/images/missing.png";

	// this function will be called when create_image_texture() fail
	// try load placeholder file
	try {
		TexturePtr dummy_texture = create_image_texture_raw(dummy_texture_filename);
		return dummy_texture;
	}
	catch (const std::exception& err) {
		// if load placeholder fails, try using empty surface
		// if that also fails, give up
		SDLSurfacePtr image(SDL_CreateRGBSurface(0, 256, 256, 8, 0 ,0, 0, 0));
		if (!image) {
			throw;
		} else {
			log_warning << "Couldn't load texture '" << dummy_texture_filename << "' (now using empty one): " << err.what() << std::endl;
			TexturePtr texture = VideoSystem::current()->new_texture(*image);
			return texture;
		}
	}
}

