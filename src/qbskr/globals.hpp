#ifndef HEADER_QBSKR_QBSKR_GLOBALS_HPP
#define HEADER_QBSKR_QBSKR_GLOBALS_HPP

// Managing globals are so confusing...
// Hope this works

#include <memory>

class Config;

extern std::unique_ptr<Config> g_config;

#endif