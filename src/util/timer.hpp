#ifndef HEADER_QBSKR_UTIL_TIMER_HPP
#define HEADER_QBSKR_UTIL_TIMER_HPP

#include "qbskr/globals.hpp"

class Timer final {
public:
	Timer();

private:
	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;

private:
	float m_period;
	float m_cycle_start;
	bool m_cyclic;

public:
	// start the timer with the given period (in seconds)
	void start(float period, bool cyclic);
	// start the timer with the given period (in seconds), but true even if check() right way 
	// (basically start at time = -inf) (but still be overriden by period == 0 then false)
	void start_true(float period, bool cyclic);
	// returns true if a period (or more) passed
	bool check();
	float get_timeleft() const;
	float get_timegone() const;
	float get_period() const;
	// normalize time gone in range [0, 1]
	// if timegone > period then return 1
	float get_timegone_normalized() const;

	bool started() const;
	bool ended() const;
};

#endif