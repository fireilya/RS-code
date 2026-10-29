#include "mtrandom.h"
#include <vector>

void MTRandom32::init() noexcept
{
	m_states[0] = m_seed;

	for (size_t i = 1; i < stateSize; ++i)
		m_states[i] = initGenerationCoeff * (m_states[i - 1] ^ (m_states[i - 1] >> 30)) + i;
}

void MTRandom32::twist() noexcept
{
    for (size_t i = 0; i < stateSize; ++i)
    {
        uint32_t x = (m_states[i] & msbMask) + (m_states[(i + 1) % stateSize] & (~msbMask));
        uint32_t xA = x >> 1;
        if (x & 1) xA ^= feedbackCoef;
        m_states[i] = m_states[(i + recursionStep) % stateSize] ^ xA;
    }
}

uint32_t MTRandom32::findNext() noexcept
{
    if (m_stateIndex == stateSize)
    {
        twist();
        m_stateIndex = 0;
    }

    uint32_t out = m_states[m_stateIndex++];

    out ^= (out >> 11);
    out ^= (out << 7) & 0x9d2c5680;
    out ^= (out << 15) & 0xefc60000;
    out ^= (out >> 18);

    return out;
}

uint32_t MTRandom32::next() noexcept
{
    return m_min + findNext() % m_range;
}

uint32_t MTRandom32::generate(uint32_t min, uint32_t max) noexcept
{
    return min + next() % (max - min);
}

void MTRandom32::setSeed(uint32_t seed) noexcept
{
    m_seed = seed;
    init();
    m_stateIndex = 0;
}
