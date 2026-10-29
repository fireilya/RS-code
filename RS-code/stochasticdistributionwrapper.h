#ifndef _STOCHASTIC_DISTRIBUTION_WRAPPER_H_
#define _STOCHASTIC_DISTRIBUTION_WRAPPER_H_

#pragma once

#include <algorithm>

class StochasticDistribution
{
public:
	virtual double next() = 0;

	virtual ~StochasticDistribution() = default;
};

#endif // !_STOCHASTIC_DISTRIBUTION_WRAPPER_H_




