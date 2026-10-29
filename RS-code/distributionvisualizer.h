#ifndef _DISTRIBUTION_VISUALIZER_H_
#define _DISTRIBUTION_VISUALIZER_H_

#pragma once

#include <vector>
#include <list>
#include <algorithm>
#include <memory>
#include "stochasticdistributionwrapper.h"
#include "interval.h"
#include "segment.h"
#include <string>
#include <iostream>

class DistributionVisualizer
{
public:
	explicit DistributionVisualizer(
		uint8_t visualizationLength = 20, 
		uint8_t visualizationHeight = 10, 
		uint32_t genSeed = 42) :
		m_visualizationLength(visualizationLength), 
		m_visualizationHeight(visualizationHeight), 
		m_genSeed(genSeed) {}

	void visualizeDistribution(std::unique_ptr<StochasticDistribution> gen, size_t samplesCount = 1e5) const;
private:

	template <typename T>
	void visualizeData(const std::vector<T>& data) const;

	template <typename T>
	std::vector<std::unique_ptr<Gap<T>>> buildIntervals(T start, T end, size_t count) const;

	void visualizeGist(const std::vector<size_t>& gist, size_t padding = 0) const;

	uint8_t m_visualizationLength;
	uint8_t m_visualizationHeight;
	uint32_t m_genSeed;
};

#endif // !_DISTRIBUTION_VISUALIZER_H_

template<typename T>
inline void DistributionVisualizer::visualizeData(const std::vector<T>& data) const
{
	T min = *(std::min_element(data.cbegin(), data.cend()));
	T max = *(std::max_element(data.cbegin(), data.cend()));
	std::vector<std::unique_ptr<Gap<T>>> gistIntervals = buildIntervals(min, max, m_visualizationLength);
	std::vector<size_t> gist(gistIntervals.size());

	for (const auto& el : data)
	{
		for (size_t i = 0; i < m_visualizationLength; ++i)
		{
			if (gistIntervals[i]->isInclude(el))
			{
				++gist[i];
				continue;
			}
		}
	}

	std::string minStr = std::to_string(static_cast<int>(min));
	visualizeGist(gist, minStr.size());
	std::cout << minStr;
	for (size_t i = 0; i < m_visualizationLength; ++i) std::cout << '_';
	std::cout << std::to_string(static_cast<int>(std::ceil(max))) << std::endl;
}

template<typename T>
inline std::vector<std::unique_ptr<Gap<T>>> DistributionVisualizer::
buildIntervals(T start, T end, size_t count) const
{
	std::vector<std::unique_ptr<Gap<T>>> answer;
	T step = static_cast<double>(end - start) / count;

	T currentIntervalStart = start;
	for (size_t i = 0; i < count; ++i)
	{
		if (i == count - 1) answer.emplace_back(std::make_unique<Segment<T>>(currentIntervalStart, currentIntervalStart + step));
		else answer.emplace_back(std::make_unique<Interval<T>>(currentIntervalStart, currentIntervalStart + step));
		currentIntervalStart += step;
	}
	return answer;
}
