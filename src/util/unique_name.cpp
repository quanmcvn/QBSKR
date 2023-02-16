#include "util/unique_name.hpp"

#include <sstream>
#include <stdint.h>
#include <time.h>

std::string make_unique_name(const std::string& prefix, void* ptr) {
	std::ostringstream out;
	out << "gen" << " " << time(nullptr) << " " << prefix << " " << reinterpret_cast<uintptr_t>(ptr);
	return out.str();
}