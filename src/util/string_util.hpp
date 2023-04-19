#ifndef HEADER_QBSKR_UTIL_STRING_UTIL_HPP
#define HEADER_QBSKR_UTIL_STRING_UTIL_HPP

#include <string>
#include <vector>

namespace StringUtil {
	std::string join_strings(const std::vector<std::string>& strings, char delim);
}

#endif