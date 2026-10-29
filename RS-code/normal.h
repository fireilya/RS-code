#ifndef _NORMAL_H_
#define _NORMAL_H_

#pragma once

#include "stochasticdistributionwrapper.h"
#include "infrastructure.h"
#include <array>
#include <ctime>

class MTRandom32;
class Uniform;
class Exponential;

class Normal : public StochasticDistribution
{
public:
	Normal() : Normal(std::time(nullptr)) {}
	Normal(uint32_t seed) : Normal(0.0, 1.0, seed) {}
	Normal(double m, double sigma, uint32_t seed);

	virtual double next() noexcept override;

	~Normal();
private:
	void init();
	double findNext();

	static const double initialX;
	static const double rectArea;
	static const uint16_t zigguratNum = 256;
	static std::array<ziggurat::ZigguratRect, zigguratNum> ziggurats;
	static bool isInited;

	MTRandom32* m_uniformIntGen;
	Uniform* m_uniformGen;
	Exponential* m_exponentialGen;
	double m_M, m_sigma;
};

#endif // !_NORMAL_H_




