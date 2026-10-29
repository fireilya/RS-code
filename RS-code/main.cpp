#include "rscoder.h"
#include "infrastructure.h"
#include "destructor.h"
#include "GF256_num.h"
#include "polinom.h"
#include "distributionvisualizer.h"
#include "uniform.h"
#include "normal.h"
#include "exponential.h"

const std::string g_testFolder = "A:/buf/";

void distributionTest(size_t visualizationLength, size_t visualizationHeight)
{
	const uint32_t seed = 42;
	DistributionVisualizer dv(visualizationLength, visualizationHeight);
	std::cout << "--------------------------" << std::endl;
	std::cout << "Uniform distribution:" << std::endl;
	std::cout << "--------------------------" << std::endl << std::endl;
	dv.visualizeDistribution(std::make_unique<Uniform>(seed));
	std::cout << std::endl << "--------------------------" << std::endl;
	std::cout << "Exponential distribution:" << std::endl;
	std::cout << "--------------------------" << std::endl << std::endl;;
	dv.visualizeDistribution(std::make_unique<Exponential>(seed));
	std::cout << std::endl << "--------------------------" << std::endl;
	std::cout << "Normal distribution:" << std::endl;
	std::cout << "--------------------------" << std::endl << std::endl;
	dv.visualizeDistribution(std::make_unique<Normal>(seed));
}

void imageTest()
{
	RSCoder coder;
	std::pair<fwork::Bytes, fwork::Bytes> bmp = fwork::readBMP(g_testFolder + "test_bmp.bmp");
	fwork::Bytes encodedBMPData = coder.encode(bmp.second, 0.8);
	bmp.second = Destructor::destruct(encodedBMPData, 0.2);
	bmp.second[0] = encodedBMPData[0];
	fwork::writeBMP(
		g_testFolder + "bmp_destructed.bmp", 
		std::make_pair(bmp.first, coder.decodeWithoutRecover(bmp.second)));

	bmp.second = coder.decode(bmp.second);
	fwork::writeBMP(g_testFolder + "bmp_decoded.bmp", bmp);
}

void textTest()
{
	RSCoder coder;
	fwork::Bytes data = fwork::readFile(g_testFolder + "test.txt");
	data.erase(data.cend() - 1);
	fwork::Bytes encodedData = coder.encode(data, 0.8);
	fwork::writeFile(g_testFolder + "test_encoded.txt", encodedData);
	fwork::Bytes destructedData = Destructor::destruct(encodedData, 0.25);
	destructedData[0] = encodedData[0];
	fwork::writeFile(g_testFolder + "test_destructed.txt", coder.decodeWithoutRecover(destructedData));
	fwork::writeFile(g_testFolder + "test_decoded.txt", coder.decode(destructedData));
}

int main()
{
	//distributionTest(50, 25);
	textTest();
	imageTest();
	return 0;
}