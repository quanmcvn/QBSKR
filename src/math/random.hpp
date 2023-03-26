#ifndef HEADER_QBSKR_MATH_RANDOM_HPP
#define HEADER_QBSKR_MATH_RANDOM_HPP

#include <random>

class Random {
public:
	Random();

private:
	Random(const Random&) = delete;
	Random& operator=(const Random&) = delete;

private:
	std::mt19937 m_generator;

public:
	// seed generator
	void seed(int v);

	// random int in [u, v)
	int rand(int u, int v);
	// random int in [u, v]
	int rand_inclusive(int u, int v);
	// random float in [0, v)
	float randf(float v);
	// random float in [u, v)
	float randf(float u, float v);
	// return true if lucky
	// eg. test_lucky(.3) will return true in 30%
	bool test_lucky(float chance);
	// return true if is lucky (percent)
	// eg. test_lucky_percent(30) will return true in 30%
	bool test_lucky_percent(float chance_percent);
};

extern Random g_game_random;

#endif