#include "util/timer.hpp"

#include <cmath>

Timer::Timer() :
	m_period(0),
	m_cycle_start(),
	m_cyclic()
{}

void Timer::start(float period, bool cyclic)
{
	m_period = period;
	m_cycle_start = g_game_time;
	m_cyclic = cyclic;
}

void Timer::start_true(float period, bool cyclic)
{
	m_period = period;
	m_cycle_start = g_game_time - m_period;
	m_cyclic = cyclic;
}

bool Timer::check()
{
	if (m_period == 0) return false;

	if (g_game_time - m_cycle_start >= m_period) {
		if (m_cyclic) {
			m_cycle_start = g_game_time - std::fmod(g_game_time - m_cycle_start, m_period);
		} else {
			m_period = 0;
		}
		return true;
	}

	return false;
}

float Timer::get_period() const { return m_period; }
float Timer::get_timeleft() const { return m_period - (g_game_time - m_cycle_start); }
float Timer::get_timegone() const { return g_game_time - m_cycle_start; }
bool Timer::started() const { return m_period != 0; }
bool Timer::ended() const { return get_timeleft() <= 0; }