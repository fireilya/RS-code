#include "destructor.h"
#include "mtrandom.h"
#include "uniform.h"

std::vector<uint8_t> Destructor::destruct(const std::vector<uint8_t>& data, double destructionCoeff, uint32_t seed)
{
    std::vector<uint8_t> answer(data);
    MTRandom32 uniformIntGen(255, seed);
    Uniform uniformGen(uniformIntGen.next());

    for (auto& byte : answer)
        if (uniformGen.next() < destructionCoeff) byte ^= static_cast<uint8_t>(uniformIntGen.next());

    return answer;
}
