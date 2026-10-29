#include "infrastructure.h"
#include <fstream>

fwork::Bytes fwork::readFile(const std::string& fileName)
{
	fwork::Bytes answer;
	std::ifstream file(fileName, std::ios_base::binary);
	if (file.is_open())
	{
		while (!file.eof()) answer.emplace_back(file.get());
	}
	file.close();
	return answer;
}

void fwork::writeFile(const std::string& fileName, const Bytes& data)
{
	std::ofstream file(fileName, std::ios_base::binary);
	if (file.is_open())
	{
		for (const auto& byte : data) file << byte;
	}
	file.close();
}

fwork::Bytes fwork::stringToBytes(const std::string& data)
{
	std::vector<uint8_t> answer;
	for (const auto& sym : data) answer.emplace_back(sym);
	return answer;
}

std::string fwork::bytesToString(const Bytes& data)
{
	std::string answer;
	for (const auto& byte : data) answer.push_back(byte);
	return answer;
}

std::pair<fwork::Bytes, fwork::Bytes> fwork::readBMP(const std::string& fileName)
{
	const uint8_t BMPHeaderSize = 0x36;
	Bytes header(BMPHeaderSize), data;
	std::ifstream file(fileName, std::ios_base::binary);
	if (file.is_open())
	{
		for (size_t i = 0; i < BMPHeaderSize && !file.eof(); i++) header[i] = file.get();
		while (!file.eof()) data.emplace_back(file.get());
	}
	file.close();
	return std::make_pair(header, data);
}

void fwork::writeBMP(const std::string& fileName, const std::pair<Bytes, Bytes> data)
{
	std::ofstream file(fileName, std::ios_base::binary);
	if (file.is_open())
	{
		for (const auto& byte : data.first) file << byte;
		for (const auto& byte : data.second) file << byte;
	}
	file.close();
}
