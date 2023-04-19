#include "util/string_util.hpp"

std::string StringUtil::join_strings(const std::vector<std::string>& strings, char delim)
{
	bool ok_to_add_delim = false;
	std::string ret;
	for (const auto& string : strings) {
		if (ok_to_add_delim) ret += delim;
		ret += string;
		ok_to_add_delim = true;
	}
	return ret;
}