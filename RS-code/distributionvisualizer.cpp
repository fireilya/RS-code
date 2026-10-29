#include "distributionvisualizer.h"
#include "stochasticdistributionwrapper.h"

void DistributionVisualizer::visualizeDistribution(std::unique_ptr<StochasticDistribution> gen, size_t samplesCount) const
{
	std::vector<double> distribution(samplesCount);
	for (size_t i = 0; i < samplesCount; ++i) distribution[i] = gen->next();
	visualizeData(distribution);
}

void DistributionVisualizer::visualizeGist(const std::vector<size_t>& gist, size_t padding) const
{
	size_t min = *(std::min_element(gist.cbegin(), gist.cend()));
	size_t max = *(std::max_element(gist.cbegin(), gist.cend()));
	double step = static_cast<double>(max) / m_visualizationHeight;
	Interval<double> currentInterval(max - step, max + 1);

	for (size_t i = 0; i < m_visualizationHeight; i++)
	{
		for (size_t j = 0; j < padding; ++j) std::cout << ' ';
		for (const auto& column : gist)
			std::cout << (currentInterval.isInclude(column) ? '|' : ' ');
		if (i != m_visualizationHeight - 1) std::cout << std::endl;
		currentInterval.setStart(currentInterval.start() - step);
	}
	std::cout << std::endl;
}
