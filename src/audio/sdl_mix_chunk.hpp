#ifndef HEADER_QBSKR_AUDIO_SDL_MIX_CHUNK_HPP
#define HEADER_QBSKR_AUDIO_SDL_MIX_CHUNK_HPP

#include <memory>

struct Mix_Chunk;

class SDLMixChunk final {
public:
	~SDLMixChunk();

private:
	SDLMixChunk(const SDLMixChunk&) = delete;
	SDLMixChunk& operator=(const SDLMixChunk&) = delete;

private:
	Mix_Chunk* m_mix_chunk;

public:
	SDLMixChunk(Mix_Chunk* mix_chunk);

public:
	// guess I can use Mix_Chunk::alen ?
	// hope it works
	int get_size() const;

	// between 0-100
	void set_volume(float volume);
	float get_volume() const;

	void play(int channel = -1, int loops = 0);
};

using SDLMixChunkPtr = std::unique_ptr<SDLMixChunk>;

#endif