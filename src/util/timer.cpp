#include "util/timer.hpp"

#include <cmath>

Timer::Timer() :
	m_period(0),
	m_cycle_start()
{}

void Timer::start(float period)
{
	m_period = period;
	m_cycle_start = g_game_time;
}

void Timer::start_true(float period)
{
	m_period = period;
	m_cycle_start = g_game_time - m_period;
}

bool Timer::check()
{
	if (m_period == 0) return false;

	if (g_game_time - m_cycle_start >= m_period) {
		m_cycle_start = g_game_time - std::fmod(g_game_time - m_cycle_start, m_period);
		return true;
	}

	return false;
}

float Timer::get_period() const { return m_period; }