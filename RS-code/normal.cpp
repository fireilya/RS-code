#include "normal.h"
#include "mtrandom.h"
#include "uniform.h"
#include "exponential.h"
#include <cmath>

const double Normal::initialX = 3.6541528853610088;
const double Normal::rectArea = 4.92867323399e-3;
std::array<ziggurat::ZigguratRect, Normal::zigguratNum> Normal::ziggurats;
bool Normal::isInited = false;

Normal::Normal(double m, double sigma, uint32_t seed) :
	m_M(m), m_sigma(sigma)
{
	if (!isInited)
	{
		init();
		isInited = true;
	}

	MTRandom32 seedGenerator(seed);
	m_uniformIntGen = new MTRandom32(seedGenerator.next());
	m_uniformGen = new Uniform(seedGenerator.next());
	m_exponentialGen = new Exponential(seedGenerator.next());
}

double Normal::next() noexcept
{
	return m_M + findNext() * m_sigma;
}

Normal::~Normal()
{
	if (m_uniformGen) delete m_uniformGen;
	if (m_uniformIntGen) delete m_uniformIntGen;
	if (m_exponentialGen) delete m_exponentialGen;
}

void Normal::init()
{
	ziggurats[0].height = std::exp(-0.5 * initialX * initialX);
	ziggurats[0].width = rectArea / ziggurats[0].height;
	for (size_t i = 1; i < zigguratNum; ++i)
	{
		ziggurats[i].width = std::sqrt(-2 * std::log(ziggurats[i - 1].height));
		ziggurats[i].height = ziggurats[i - 1].height + rectArea / ziggurats[i].width;
	}
}

double Normal::findNext()
{
	for (size_t i = 0; i < 1e9; ++i)
	{
		uint32_t zigguratIndex = m_uniformIntGen->generate(0, zigguratNum - 1);
		bool isNegativeReturn = m_uniformGen->generate() < 0.5;
		double x = m_uniformGen->generate(0, ziggurats[zigguratIndex].width);
		if (x < ziggurats[zigguratIndex + 1].width) return isNegativeReturn ? -x : x;
		if (!zigguratIndex)
		{
			static double z = -1;
			double y;

			if (z > 0)
			{
				x = m_exponentialGen->generate(initialX);
				z -= 0.5 * x * x;
			}

			if (z <= 0)
			{
				do
				{
					x = m_exponentialGen->generate(initialX);
					y = m_exponentialGen->generate();
					z = y - 0.5 * x * x;
				} while (z <= 0);
			}

			x += initialX;
			return isNegativeReturn ? -x : x;
		}

		if (m_uniformGen->generate(ziggurats[zigguratIndex - 1].height, ziggurats[zigguratIndex].height) < std::exp(-0.5 * x * x))
			return isNegativeReturn ? -x : x;
	}
	throw std::exception();
}
