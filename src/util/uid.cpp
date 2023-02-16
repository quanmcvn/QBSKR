#include "util/uid.hpp"

#include <ostream>

std::ostream& operator<<(std::ostream& os, const UID& uid) {
	return os << uid.m_value;
}

namespace std {

	size_t hash<UID>::operator()(const UID& uid) const {
		return static_cast<size_t>(uid.m_value);
	}

} // namespace std