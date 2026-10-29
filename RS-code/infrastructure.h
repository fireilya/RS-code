#ifndef _INFRASTRUCTURE_H_
#define _INFRASTRUCTURE_H_

#pragma once

#include <vector>
#include <string>

namespace ziggurat
{
	struct ZigguratRect
	{
		double height, width;
	};
}

namespace fwork
{
	using Bytes = std::vector<uint8_t>;
	Bytes readFile(const std::string& fileName);
	void writeFile(const std::string& fileName, const Bytes& data);
	Bytes stringToBytes(const std::string& data);
	std::string bytesToString(const Bytes& data);
	std::pair<Bytes, Bytes> readBMP(const std::string& fileName);
	void writeBMP(const std::string& fileName, const std::pair<Bytes, Bytes> data);
}


#endif // !_INFRASTRUCTURE_H_




