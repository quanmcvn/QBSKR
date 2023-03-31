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

public:
	UID();

public:
	UID(const UID& other) = default;
	UID& operator=(const UID& other) = default;

protected:
	uint32_t m_value;

private:
	explicit UID(uint32_t value);

public:
	operator bool() const;
	bool operator<(const UID& other) const;
	bool operator==(const UID& other) const;
	bool operator!=(const UID& other) const;

public:
	friend std::ostream& operator<<(std::ostream& os, const UID& uid);
	friend size_t std::hash<UID>::operator()(const UID&) const;
};

std::ostream& operator<<(std::ostream& os, const UID& uid);

#endif