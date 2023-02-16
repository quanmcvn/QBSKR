#ifndef HEADER_QBSKR_UTIL_UID_HPP
#define HEADER_QBSKR_UTIL_UID_HPP

#include <assert.h>
#include <stdint.h>
#include <functional>
#include <iosfwd>

class UID;

namespace std {
	template<>
	struct hash<UID> {
		size_t operator()(const UID& uid) const;
	};
} // namespace std

class UID {
	friend class UIDGenerator;
	friend std::ostream& operator<<(std::ostream& os, const UID& uid);
	friend size_t std::hash<UID>::operator()(const UID&) const;
protected:
	uint32_t m_value;

private:
	explicit UID(uint32_t value) :
		m_value(value) {
		assert(m_value != 0);
	}

public:
	UID() : m_value(0) {}
	UID(const UID& other) = default;
	UID& operator=(const UID& other) = default;

	inline operator bool() const {
		return m_value != 0;
	}

	inline bool operator<(const UID& other) const {
		return m_value < other.m_value;
	}

	inline bool operator==(const UID& other) const {
		return m_value == other.m_value;
	}

	inline bool operator!=(const UID& other) const {
		return m_value != other.m_value;
	}
};

std::ostream& operator<<(std::ostream& os, const UID& uid);

#endif