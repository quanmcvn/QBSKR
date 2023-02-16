#ifndef HEADER_QBSKR_UTIL_UNIQUE_NAME_HPP
#define HEADER_QBSKR_UTIL_UNIQUE_NAME_HPP

#include <string>

/**
 *  This function makes unique name based on prefix given and a ptr
*/
std::string make_unique_name(const std::string& prefix, void* ptr);

#endif