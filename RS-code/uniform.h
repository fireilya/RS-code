#ifndef _UNIFORM_H_
#define _UNIFORM_H_

#pragma once

#include "stochasticdistributionwrapper.h"
#include <ctime>

class MTRandom32;

class Uniform final : public StochasticDistribution
{
public:
	explicit Uniform() : Uniform(std::time(nullptr)) {}
	explicit Uniform(uint32_t seed) : Uniform(1, seed) {}
	Uniform(double max, uint32_t seed) : Uniform(0, max, seed) {}
	Uniform(double min, double max, uint32_t seed);

	virtual double next() noexcept override;
	double generate(double min = 0, double max = 1.0);

	~Uniform() { if (m_gen) delete m_gen; }
private:
	double m_min, m_max, m_range;
	MTRandom32* m_gen = nullptr;
};


#endif // !_UNIFORM_H_



