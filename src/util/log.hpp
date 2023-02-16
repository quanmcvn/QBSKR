#ifndef HEADER_QBSKR_UTIL_LOG_HPP
#define HEADER_QBSKR_UTIL_LOG_HPP

#include <iostream>

enum LogLevel { LOG_NONE, LOG_FATAL, LOG_WARNING, LOG_INFO, LOG_DEBUG };
extern LogLevel g_log_level;

std::ostream& log_debug_f(const char* file, int line);
/**
 * usage: log_debug << "things";
*/
#define log_debug if (g_log_level >= LOG_DEBUG) log_debug_f(__FILE__, __LINE__)

std::ostream& log_info_f(const char* file, int line);
/**
 * usage: log_info << "things";
*/
#define log_info if (g_log_level >= LOG_INFO) log_info_f(__FILE__, __LINE__)

std::ostream& log_warning_f(const char* file, int line);
/**
 * usage: log_warning << "things";
*/
#define log_warning if (g_log_level >= LOG_WARNING) log_warning_f(__FILE__, __LINE__)

std::ostream& log_fatal_f(const char* file, int line);
/**
 * usage: log_fatal << "things";
*/
#define log_fatal if (g_log_level >= LOG_FATAL) log_fatal_f(__FILE__, __LINE__)

std::ostream& get_logging_instance();

#endif