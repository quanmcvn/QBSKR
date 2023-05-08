#ifndef HEADER_QBSKR_AUDIO_SOUND_MANAGER_HPP
#define HEADER_QBSKR_AUDIO_SOUND_MANAGER_HPP

#include <map>
#include <string>

#include "audio/sdl_mix_chunk.hpp"
#include "audio/sdl_mix_music.hpp"
#include "util/currenton.hpp"

/**
 *  An inferior version of TextureManager (by myself)
 *  No extend support aka have to write the whole code again if I want to add more lib
 *  Simple caching sound
 *  The whole design is bad
*/
class SoundManager final : public Currenton<SoundManager> {
public:
	SoundManager();
	~SoundManager() override;

private:
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;

private:
	std::map<std::string, SDLMixChunkPtr> m_mix_chunks;
	std::map<std::string, SDLMixMusicPtr> m_mix_musics;

	int m_sound_volume;
	int m_music_volume;

public:
	void load_sound(const std::string& filename);
	void play_sound(const std::string& filename, int loops = 0);

	void load_music(const std::string& filename);
	void play_music(const std::string& filename, int loops = -1);

	// between 0-100
	void set_sound_volume(int volume);
	int get_sound_volume() const;

	// between 0-100
	void set_music_volume(int volume);
	int get_music_volume() const;
};

#endif