#include "util/uid_generator.hpp"

#include "util/log.hpp"

uint8_t UIDGenerator::s_magic_counter = 1;

UIDGenerator::UIDGenerator() :
	m_magic(s_magic_counter++),
	m_id_counter() {

	if (s_magic_counter == 0) {
		s_magic_counter = 1;
	}
}

UID UIDGenerator::next() {
	m_id_counter += 1;

	if (m_id_counter > 0xff'ffff) {
		log_warning << "UIDGenerator overflow!!!" << std::endl;
		m_id_counter = 0;
	}

	return UID((m_magic << 24) | m_id_counter);
}