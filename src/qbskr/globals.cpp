#include "qbskr/globals.hpp"

#include "qbskr/gameconfig.hpp"

std::unique_ptr<Config> g_config = std::make_unique<Config>(std::string("gameconfig.txt"));

float g_game_time = 0;