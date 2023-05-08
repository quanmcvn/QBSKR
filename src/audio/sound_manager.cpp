#include "audio/sound_manager.hpp"

#include <SDL_mixer.h>
#include <filesystem>
#include <string>

#include "audio/sdl_mix_chunk.hpp"
#include "audio/sdl_mix_music.hpp"

SoundManager::SoundManager() :
	m_mix_chunks(),
	m_mix_musics(),
	m_music_volume(100.0f)
{
	Mix_OpenAudio(48000, AUDIO_S16SYS, 2, 2048);
}

SoundManager::~SoundManager()
{
	m_mix_chunks.clear();
	m_mix_musics.clear();
	Mix_CloseAudio();
}


void SoundManager::load_sound(const std::string& filename)
{
	if (!std::filesystem::exists(filename)) {
		std::ostringstream msg;
		msg << "File '" << filename << "' not exist";
		throw std::runtime_error(msg.str());
	}
	auto it = m_mix_chunks.find(filename);
	if (it != m_mix_chunks.end()) return;
	m_mix_chunks[filename] = std::move(std::make_unique<SDLMixChunk>(Mix_LoadWAV(filename.c_str())));
}

void SoundManager::play_sound(const std::string& filename, int loops)
{
	load_sound(filename);
	m_mix_chunks[filename]->play(loops);
}

void SoundManager::load_music(const std::string& filename)
{
	if (!std::filesystem::exists(filename)) {
		std::ostringstream msg;
		msg << "File '" << filename << "' not exist";
		throw std::runtime_error(msg.str());
	}
	auto it = m_mix_musics.find(filename);
	if (it != m_mix_musics.end()) return;
	m_mix_musics[filename] = std::move(std::make_unique<SDLMixMusic>(Mix_LoadMUS(filename.c_str())));
}

void SoundManager::play_music(const std::string& filename, int loops)
{
	load_music(filename);
	m_mix_musics[filename]->play(loops);
}

void SoundManager::set_sound_volume(int volume)
{
	m_sound_volume = volume;
	Mix_Volume(-1, static_cast<Uint8>(static_cast<float>(volume) / 100.0f * MIX_MAX_VOLUME));
}

int SoundManager::get_sound_volume() const { return m_sound_volume; }

void SoundManager::set_music_volume(int volume)
{
	m_music_volume = volume;
	Mix_VolumeMusic(static_cast<Uint8>(static_cast<float>(volume) / 100.0f * MIX_MAX_VOLUME));
}

int SoundManager::get_music_volume() const { return m_music_volume; }