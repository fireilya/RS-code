#include "exponential.h"
#include "mtrandom.h"
#include "uniform.h"
#include <cmath>

std::array<ziggurat::ZigguratRect, Exponential::zigguratNums> Exponential::ziggurats;
const double Exponential::initialX = 7.69711747013104972;
const double Exponential::rectArea = 3.9496598225815571993e-3;
bool Exponential::isInited = false;

Exponential::Exponential(double lambda, uint32_t seed) : m_lambda(lambda) 
{
	MTRandom32 seedGenerator(seed);
	if (!lambda) throw std::exception();

	if (!isInited)
	{
		init();
		isInited = true;
	}

	m_uniformGen = new Uniform(seedGenerator.next());
	m_uniformIntGen = new MTRandom32(seedGenerator.next());
}

double Exponential::next()
{
	return findNext() / m_lambda;
}

double Exponential::generate(double lambda)
{
	return findNext() / lambda;
}

Exponential::~Exponential()
{
	if (m_uniformGen) delete m_uniformGen;
	if (m_uniformIntGen) delete m_uniformIntGen;
}

void Exponential::init()
{
	ziggurats[0].height = std::exp(-initialX);
	for (size_t i = 1; i < zigguratNums; ++i)
	{
		ziggurats[i].width = -std::log(ziggurats[i - 1].height);
		ziggurats[i].height = ziggurats[i - 1].height + rectArea / ziggurats[i].width;
	}
	isInited = true;
}

double Exponential::findNext()
{
	for (size_t i = 0; i < 1e9; i++)
	{
		uint32_t zigguratIndex = m_uniformIntGen->generate(0, zigguratNums - 1);
		double x = m_uniformGen->generate(0, ziggurats[zigguratIndex].width);
		if (x < ziggurats[zigguratIndex + 1].width) return x;
		if (!zigguratIndex) return initialX + findNext();
		if (m_uniformGen->generate(ziggurats[zigguratIndex - 1].height, ziggurats[zigguratIndex].height) < std::exp(-x)) return x;
	}
	throw std::exception();
}
