#ifndef HEADER_QBSKR_AUDIO_SDL_MIX_MUSIC_HPP
#define HEADER_QBSKR_AUDIO_SDL_MIX_MUSIC_HPP

#include <SDL_mixer.h>
#include <memory>

class SDLMixMusic final {
public:
	~SDLMixMusic();

private:
	SDLMixMusic(const SDLMixMusic&) = delete;
	SDLMixMusic& operator=(const SDLMixMusic&) = delete;

private:
	Mix_Music* m_mix_music;

public:
	SDLMixMusic(Mix_Music* mix_music);

public:
	void play(int loops = -1);
};

using SDLMixMusicPtr = std::unique_ptr<SDLMixMusic>;

#endif