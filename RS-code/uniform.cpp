#include "uniform.h"
#include "mtrandom.h"
Uniform::Uniform(double min, double max, uint32_t seed) 
{
	if (min > max) throw std::exception();
	m_min = min;
	m_max = max;
	m_range = max - min;
	m_gen = new MTRandom32(seed);
}

double Uniform::next() noexcept
{
	return static_cast<double>(m_gen->next()) / m_gen->max() * m_range;
}

double Uniform::generate(double min, double max)
{
	if (min > max) throw std::exception();
	return static_cast<double>(m_gen->next()) / m_gen->max() * (max - min);
}
