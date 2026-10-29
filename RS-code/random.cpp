#include "random.h"

Random32::Random32(uint32_t min, uint32_t max, uint32_t seed) noexcept
{
	if (min > max) throw std::exception();
	m_min = min;
	m_max = max;
	m_range = max - min;
	m_seed = seed;
}
