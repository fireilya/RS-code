#ifndef _MT_RANDOM_H_
#define _MT_RANDOM_H_

#pragma once
#include "random.h"
#include <array>
#include <ctime>
class MTRandom32 final : public Random32
{
public:
	explicit MTRandom32() : MTRandom32(std::time(nullptr)) {}
	explicit MTRandom32(uint32_t seed) : MTRandom32(UINT32_MAX, seed) {}
	MTRandom32(uint32_t max, uint32_t seed) : MTRandom32(0, max, seed) {}
	MTRandom32(uint32_t min, uint32_t max, uint32_t seed) :
		Random32(min, max, seed) { init(); }

	virtual uint32_t next() noexcept override;
	virtual uint32_t generate(uint32_t min = 0, uint32_t max = UINT32_MAX) noexcept override;

	virtual void setSeed(uint32_t seed) noexcept override;

	uint32_t min() { return m_min; }
	uint32_t max() { return m_max; }
	uint32_t range() { return m_range; }
private:
	void init() noexcept;
	void twist() noexcept;
	uint32_t findNext() noexcept;

	const static uint32_t stateSize = 624;
	const static uint32_t recursionStep = 397;
	const static uint32_t feedbackCoef = 0x9908B0DF;
	const static uint32_t msbMask = 1 << 31;
	const static uint32_t initGenerationCoeff = 1812433253;
	
	uint32_t m_stateIndex = 0;
	std::array<uint32_t, stateSize> m_states;
};

#endif // !_MT_RANDOM_H_




