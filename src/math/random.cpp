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
	return rand_inclusive(u, v - 1);
}

int Random::rand_inclusive(int u, int v)
{
	return std::uniform_int_distribution<int>(u, v)(m_generator);
}

float Random::randf(float u, float v)
{
	return std::uniform_real_distribution<float>(u, v)(m_generator);
}

float Random::randf(float v)
{
	return randf(0.0, v);
}

bool Random::test_lucky(float chance) { return randf(0.0f, 1.0f) < chance; }
bool Random::test_lucky_percent(float chance_percent) { return test_lucky(chance_percent / 100.f); }