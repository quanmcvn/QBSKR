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
	// random float in [u, v)
	float randf(float u, float v);
};

extern Random g_game_random;

#endif