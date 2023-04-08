#ifndef HEADER_QBSKR_QBSKR_GLOBALS_HPP
#define HEADER_QBSKR_QBSKR_GLOBALS_HPP

// Managing globals are so confusing...
// Hope this works

#include <memory>

class Config;

// a little bit of trolling
#define SCREEN_WIDTH (VideoSystem::current()->get_viewport().get_screen_width())
// a little bit of trolling
#define SCREEN_HEIGHT (VideoSystem::current()->get_viewport().get_screen_height())

extern std::unique_ptr<Config> g_config;

extern float g_game_time;
extern float g_real_time;

#endif