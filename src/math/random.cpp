#include "random.hpp"

Random g_game_random;

Random::Random() :
	m_generator()
{}

void Random::seed(int v)
{
	m_generator.seed(v);
}

int Random::rand(int u, int v)
{
	return std::uniform_int_distribution<int>(u, v - 1)(m_generator);
}

int Random::rand_inclusive(int u, int v)
{
	return std::uniform_int_distribution<int>(u, v)(m_generator);
}

float Random::randf(float u, float v)
{
	return std::uniform_real_distribution<float>(u, v)(m_generator);
}