#ifndef _RANDOM_H_
#define _RANDOM_H_

#pragma once

#include <algorithm>

class Random32
{
public:
	Random32(uint32_t min, uint32_t max, uint32_t seed) noexcept;

	virtual uint32_t next() = 0;
	virtual uint32_t generate(uint32_t min, uint32_t max) = 0;

	virtual void setSeed(uint32_t seed) noexcept { m_seed = seed; }
	uint32_t min() { return m_min; }
	uint32_t max() { return m_max; }
	uint32_t range() { return m_range; }

	virtual ~Random32() = default;
protected:
	uint32_t m_seed;
	uint32_t m_min, m_max, m_range;
};

#endif // !_RANDOM_H_




