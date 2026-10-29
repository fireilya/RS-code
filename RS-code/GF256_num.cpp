#include "GF256_num.h"
#include <iostream>

bool GF256Num::isInited = false;
std::array<uint8_t, GF256Num::fieldSize> GF256Num::logTable;
std::array<uint8_t, GF256Num::fieldSize> GF256Num::powTable;
const GF256Num GF256Num::primitive(2);

void GF256Num::inverse() noexcept
{
	m_value = powTable[255 - logTable[m_value]];
}

GF256Num GF256Num::inverseCopy() const noexcept
{
	return GF256Num(powTable[255 - logTable[m_value]]);
}

GF256Num GF256Num::powCopy(const uint8_t pow) const noexcept
{
	GF256Num answer, answerCopy;
	answer = answerCopy = *this;
	if (answer.m_value == primitive.m_value) return powTable[pow];
	for (size_t i = 1; i < pow; ++i) answer *= answerCopy;
	return answer;
}

void GF256Num::init()
{
	uint16_t formingPolinom = 0x11d;
	uint16_t currentDegree = 1;

	for (uint16_t i = 0; i < fieldSize; i++)
	{
		powTable[i] = static_cast<uint8_t>(currentDegree);
		logTable[currentDegree] = static_cast<uint8_t>(i);
		currentDegree <<= 1;
		if (currentDegree & 0x100) currentDegree ^= formingPolinom;
	}

	isInited = true;
}

GF256Num::GF256Num(uint8_t num) noexcept : m_value(num)
{
	if (isInited) return;
	init();
}

GF256Num GF256Num::operator+(const GF256Num& right) const noexcept
{
	return GF256Num(m_value ^ right.m_value);
}

GF256Num GF256Num::operator-(const GF256Num& right) const noexcept
{
	return *this + right;
}

GF256Num GF256Num::operator*(const GF256Num& right) const noexcept
{
	uint8_t answer = 0;
	if (m_value && right.m_value)
		answer = powTable[(logTable[m_value] + logTable[right.m_value]) % 255];
	return GF256Num(answer);
}

GF256Num GF256Num::operator/(const GF256Num& right) const
{
	if (!right.m_value) throw std::exception();

	int16_t degree = 0;
	if (m_value)
	{
		degree = logTable[m_value] - logTable[right.m_value];
		if (degree < 0) degree += 255;
	}
	return GF256Num(powTable[static_cast<uint8_t>(degree)]);
}

void GF256Num::operator+=(const GF256Num& right) noexcept
{
	m_value ^= right.m_value;
}

void GF256Num::operator-=(const GF256Num& right) noexcept
{
	m_value ^= right.m_value;
}

void GF256Num::operator*=(const GF256Num& right) noexcept
{
	if (m_value && right.m_value)
		m_value = powTable[(logTable[m_value] + logTable[right.m_value]) % 255];
	else m_value = 0;
}

void GF256Num::operator/=(const GF256Num& right)
{
	if (!right.m_value) throw std::exception();
	if (m_value)
	{
		int16_t degree = logTable[m_value] - logTable[right.m_value];
		if (degree < 0) degree = 255 - degree;
		m_value = powTable[static_cast<uint8_t>(degree)];
	}

	else m_value = 0;
}

std::ostream& operator<<(std::ostream& out, const GF256Num& num)
{
	out << static_cast<uint16_t>(num.value());
	return out;
}