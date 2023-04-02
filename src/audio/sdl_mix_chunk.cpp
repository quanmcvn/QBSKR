#include "audio/sdl_mix_chunk.hpp"

#include <SDL_mixer.h>

SDLMixChunk::~SDLMixChunk()
{
	Mix_FreeChunk(m_mix_chunk);
}

SDLMixChunk::SDLMixChunk(Mix_Chunk* mix_chunk) :
	m_mix_chunk(mix_chunk)
{}

int SDLMixChunk::get_size() const { return m_mix_chunk->alen; }

void SDLMixChunk::set_volume(float volume) { Mix_VolumeChunk(m_mix_chunk, static_cast<Uint8>(volume / 100.0f * MIX_MAX_VOLUME)); }
float SDLMixChunk::get_volume() const { return static_cast<float>(Mix_VolumeChunk(m_mix_chunk, -1)) / MIX_MAX_VOLUME * 100.0f; }

void SDLMixChunk::play(int channel, int loops)
{
	Mix_PlayChannel(channel, m_mix_chunk, loops);
}