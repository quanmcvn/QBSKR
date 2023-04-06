#include "audio/sound_manager.hpp"

#include <SDL_mixer.h>
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
	auto it = m_mix_chunks.find(filename);
	if (it != m_mix_chunks.end()) return;
	// no SDL_Mixer error checking here
	// idk why but Mix_LoadWAV() always return non NULL
	m_mix_chunks[filename] = std::move(std::make_unique<SDLMixChunk>(Mix_LoadWAV(filename.c_str())));
}

void SoundManager::play_sound(const std::string& filename, int loops)
{
	load_sound(filename);
	m_mix_chunks[filename]->play(loops);
}

void SoundManager::load_music(const std::string& filename)
{
	auto it = m_mix_musics.find(filename);
	if (it != m_mix_musics.end()) return;
	m_mix_musics[filename] = std::move(std::make_unique<SDLMixMusic>(Mix_LoadMUS(filename.c_str())));
}

void SoundManager::play_music(const std::string& filename, int loops)
{
	load_sound(filename);
	m_mix_musics[filename]->play(loops);
}

void SoundManager::set_volume_music(float volume)
{
	m_music_volume = volume;
	Mix_VolumeMusic(static_cast<Uint8>(volume / 100.0f * MIX_MAX_VOLUME));
}

float SoundManager::get_volume_music() const { return m_music_volume; }