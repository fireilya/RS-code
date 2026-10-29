#ifndef _DESTRUCTOR_H_
#define _DESTRUCTOR_H_

#pragma once

#include <vector>
namespace Destructor
{
	std::vector<uint8_t> destruct(const std::vector<uint8_t>& data, double destructionCoeff, uint32_t seed = 42);
};

#endif // !_DESTRUCTOR_H_




