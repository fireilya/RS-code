#include "rscoder.h"

RSCoder::Bytes RSCoder::encode(const Bytes& data, const double correctionRatio)
{
    Bytes answer;
    uint8_t infoSymbolsNum = static_cast<uint8_t>((static_cast<double>(blockSize) / (2 * correctionRatio + 1)));
    if (infoSymbolsNum % 2 == 0) --infoSymbolsNum;
    uint8_t correctionSymbolsNum = blockSize - infoSymbolsNum;
    answer.emplace_back(correctionSymbolsNum);
    if (!correctionSymbolsNum)
    {
        for (const auto& byte : data) answer.emplace_back(byte);
        return answer;
    }

    Polinom<GF256Num> generator = createGeneratorPolinom(blockSize - infoSymbolsNum);

    for (size_t i = 0; i < data.size();)
    {
        Polinom<GF256Num> block(blockSize - 1);
        for (size_t j = correctionSymbolsNum; j < blockSize && i < data.size(); ++j, ++i) block[j] = data[i];
        block.normalize();
        block -= block % generator;
        block.setDegree(blockSize - 1);
        for (const auto& coef : block.coeffs()) answer.emplace_back(coef.value());
    }
    answer.erase((std::find_if(answer.crbegin(), answer.crend(), [](const auto& el) { return el; })).base(), answer.end());
    return answer;
}

RSCoder::Bytes RSCoder::decode(const Bytes& data)
{
    Bytes answer;
    if (data.empty()) return answer;
    uint8_t correctionSymbolsNum = data[0];
    if (!correctionSymbolsNum)
    {
        for (size_t i = 1; i < data.size(); ++i) answer.emplace_back(data[i]);
        return answer;
    }

    for (size_t i = 1; i < data.size();)
    {
        Polinom<GF256Num> block(blockSize - 1);
        for (size_t j = 0; j < blockSize && i < data.size(); ++j, ++i) block[j] = data[i];
        Polinom<GF256Num> syndrom = findSyndroms(block, correctionSymbolsNum);
        const auto& syndromCoeffs = syndrom.coeffs();
        if (std::find_if(syndromCoeffs.cbegin(), syndromCoeffs.cend(), [](const auto& el) { return el; }) == syndromCoeffs.cend())
        {
            for (size_t j = correctionSymbolsNum; j < blockSize; ++j) answer.emplace_back(block[j].value());
            continue;
        }

        Polinom<GF256Num> errLocator = findErrLocator(syndrom, correctionSymbolsNum);
        block -= findErrDelta(syndrom, errLocator, findErrPositions(errLocator), correctionSymbolsNum);
        block.setDegree(blockSize - 1);
        for (size_t j = correctionSymbolsNum; j < blockSize; ++j) answer.emplace_back(block[j].value());
    }
    answer.erase((std::find_if(answer.crbegin(), answer.crend(), [](const auto& el) { return el; })).base(), answer.end());
    return answer;
}

RSCoder::Bytes RSCoder::decodeWithoutRecover(const Bytes& data)
{
    Bytes answer;
    if (data.empty()) return answer;
    uint8_t correctionSymbolsNum = data[0];
    if (!correctionSymbolsNum)
    {
        for (size_t i = 1; i < data.size(); ++i) answer.emplace_back(data[i]);
        return answer;
    }

    for (size_t i = 1; i < data.size();)
    {
        i += correctionSymbolsNum;
        for (size_t j = correctionSymbolsNum; j < blockSize && i < data.size(); ++j, ++i) answer.emplace_back(data[i]);
    }

    return answer;
}

Polinom<GF256Num> RSCoder::createGeneratorPolinom(const uint8_t correctionSymbolsNum)
{
    GF256Num currentRoot = GF256Num::primitive;
    GF256Num currentRootCopy = currentRoot;
    Polinom<GF256Num> answer(std::vector<GF256Num>{1});
    for (size_t i = 0; i < correctionSymbolsNum; i++)
    {
        answer *= Polinom<GF256Num>(std::vector<GF256Num>{currentRoot, -static_cast<GF256Num>(1)});
        currentRoot *= currentRootCopy;
    }
    return answer;
}

Polinom<GF256Num> RSCoder::findSyndroms(const Polinom<GF256Num>& block, const uint8_t correctionSymbolsNum)
{
    GF256Num currentRoot = GF256Num::primitive;
    GF256Num currentRootCopy = currentRoot;
    Polinom<GF256Num> answer(correctionSymbolsNum - 1);
    for (size_t i = 0; i < correctionSymbolsNum; i++)
    {
        answer[i] = block.eval(currentRoot);
        currentRoot *= currentRootCopy;
    }
    return answer;
}

Polinom<GF256Num> RSCoder::findErrLocator(const Polinom<GF256Num>& syndroms, const uint8_t correctionSymbolsNum)
{
    Polinom<GF256Num> errLocator(std::vector<GF256Num>{1});
    Polinom<GF256Num> oldErrLocator = errLocator;

    for (size_t i = 0; i < correctionSymbolsNum; ++i)
    {
        uint8_t currentSyndromIndex = i;
        GF256Num delta = syndroms[currentSyndromIndex];
        for (size_t j = 1; j <= errLocator.degree(); j++) delta += errLocator[j] * syndroms[currentSyndromIndex - j];
        
        oldErrLocator.pow(1);
        if (delta)
        {
            if (oldErrLocator.degree() > errLocator.degree())
            {
                Polinom<GF256Num> newErrLocator = oldErrLocator.scaleCopy(delta);
                oldErrLocator = errLocator.scaleCopy(delta.inverseCopy());
                errLocator = newErrLocator;
            }
            errLocator -= oldErrLocator.scaleCopy(delta);
        }
    }

    return errLocator;
}

std::vector<uint8_t> RSCoder::findErrPositions(const Polinom<GF256Num>& errLocator)
{
    GF256Num currentRoot = GF256Num::primitive;
    GF256Num currentRootCopy = currentRoot;
    std::vector<uint8_t> answer;
    if (!errLocator.eval(GF256Num(1))) answer.emplace_back(0);
    for (size_t i = 1; i < 255; i++)
    {
        if (!errLocator.eval(currentRoot.inverseCopy())) answer.emplace_back(i);
        currentRoot *= currentRootCopy;
    }
    return answer;
}

Polinom<GF256Num> RSCoder::findErrDelta(
    const Polinom<GF256Num>& syndroms, 
    const Polinom<GF256Num>& errLocator,
    const std::vector<uint8_t>& errPositions, 
    const uint8_t correctionSymbolsNum)
{
    Polinom<GF256Num> errs(correctionSymbolsNum - 1);
    const auto productCoeffs = (syndroms * errLocator).coeffs();
    for (size_t i = 0; i < correctionSymbolsNum; ++i) errs[i] = productCoeffs[i];
    Polinom<GF256Num> errLocatorDer = findFormalDerivative(errLocator);
    Polinom<GF256Num> errDelta(blockSize);

    for (const auto& errPos : errPositions)
    {
        GF256Num buf = GF256Num::primitive.powCopy(errPos).inverseCopy();
        auto x = errLocator.eval(buf);
        errDelta[errPos] = errs.eval(buf) / errLocatorDer.eval(buf);
    }

    return errDelta;
}

Polinom<GF256Num> RSCoder::findFormalDerivative(const Polinom<GF256Num>& polinom)
{
    Polinom<GF256Num> answer(polinom.degree());
    for (size_t i = 1; i < polinom.degree() + 1; i += 2) answer[i - 1] = polinom[i];
    answer.normalize();
    return answer;
}
