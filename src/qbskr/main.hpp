#ifndef HEADER_QBSKR_QBSKR_MAIN_HPP
#define HEADER_QBSKR_QBSKR_MAIN_HPP

#include "object/player.hpp"

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture(std::string path);

class Main final {
public:
	int run(int argc, char** argv);
};

#endif
