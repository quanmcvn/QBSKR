#ifndef HEADER_QBSKR_UTIL_CURRENTON_HPP
#define HEADER_QBSKR_UTIL_CURRENTON_HPP

#include <cassert>

/**
 * Kind of like a singleton, but without handling the object construction itself
 */
template<class C>
class Currenton {
private:
	static Currenton<C>* s_current;

protected:
	Currenton()
	{
		// put inside if to make debug-able (put breakpoint in here)
		if (s_current != nullptr) {
			assert(s_current == nullptr);
		}
		s_current = this;
	}

	virtual ~Currenton()
	{
		if (s_current == this) {
			s_current = nullptr;
		}
	}

public:
	static C* current() { return static_cast<C*>(s_current); }
};

template<class C>
Currenton<C>* Currenton<C>::s_current = nullptr;

#endif