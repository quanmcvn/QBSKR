#include "audio/sdl_mix_music.hpp"

#include <SDL_mixer.h>

SDLMixMusic::~SDLMixMusic()
{
	Mix_FreeMusic(m_mix_music);
}

SDLMixMusic::SDLMixMusic(Mix_Music* mix_music) :
	m_mix_music(mix_music)
{}

void SDLMixMusic::play(int loops)
{
	Mix_PlayMusic(m_mix_music, loops);
}