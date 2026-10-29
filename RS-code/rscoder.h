#ifndef _RS_CODER_H_
#define _RS_CODER_H_

#pragma once

#include "polinom.h"
#include "GF256_num.h"
#include <vector>

class RSCoder
{
public:
	using Bytes = std::vector<uint8_t>;

	Bytes encode(const Bytes& data, const double correctionRatio);
	Bytes decode(const Bytes& data);
	Bytes decodeWithoutRecover(const Bytes& data);

private:
	Polinom<GF256Num> createGeneratorPolinom(const uint8_t correctionSymbolsNum);
	Polinom<GF256Num> findSyndroms(const Polinom<GF256Num>& block, const uint8_t correctionSymbolsNum);
	Polinom<GF256Num> findErrLocator(const Polinom<GF256Num>& syndroms, const uint8_t correctionSymbolsNum);
	std::vector<uint8_t> findErrPositions(const Polinom<GF256Num>& errLocator);
	Polinom<GF256Num> findErrDelta(
		const Polinom<GF256Num>& syndroms,
		const Polinom<GF256Num>& errLocator,
		const std::vector<uint8_t>& errPositions,
		const uint8_t correctionSymbolsNum);

	Polinom<GF256Num> findFormalDerivative(const Polinom<GF256Num>& polinom);

	const uint8_t blockSize = 255;
};

#endif // !_RS_CODER_H_




