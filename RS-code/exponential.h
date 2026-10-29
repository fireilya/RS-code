#ifndef _EXPONENTIAL_H_
#define _EXPONENTIAL_H_

#pragma once

#include "stochasticdistributionwrapper.h"
#include "infrastructure.h"
#include <array>
#include <ctime>

class MTRandom32;
class Uniform;

class Exponential final : public StochasticDistribution
{
public:
	explicit Exponential() : Exponential(std::time(nullptr)) {}
	explicit Exponential(uint32_t seed) : Exponential(1.0, seed) {}
	Exponential(double lambda, uint32_t seed);

	virtual double next() override;
	double generate(double lambda = 1.0);

	~Exponential();
private:
	void init();
	double findNext();

	static const uint16_t zigguratNums = 256;
	static std::array<ziggurat::ZigguratRect, zigguratNums> ziggurats;
	static const double initialX, rectArea;
	static bool isInited;

	MTRandom32* m_uniformIntGen = nullptr;
	Uniform* m_uniformGen = nullptr;
	double m_lambda;
};

#endif // !_EXPONENTIAL_H_




