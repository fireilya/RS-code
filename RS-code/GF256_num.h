#ifndef _GF256_NUM_H_
#define _GF256_NUM_H_

#pragma once

#include <array>
#include <compare>
#include <iostream>

class GF256Num
{
public:
	GF256Num(uint8_t num = 0) noexcept;

	GF256Num operator+(const GF256Num& right) const noexcept;
	GF256Num operator-(const GF256Num& right) const noexcept;
	GF256Num operator*(const GF256Num& right) const noexcept;
	GF256Num operator/(const GF256Num& right) const;

	void operator+=(const GF256Num& right) noexcept;
	void operator-=(const GF256Num& right) noexcept;
	void operator*=(const GF256Num& right) noexcept;
	void operator/=(const GF256Num& right);

	GF256Num operator-() const noexcept { return *this; }

	bool operator==(const GF256Num& right) const noexcept { return m_value == right.m_value; }
	bool operator<(const GF256Num& right) const noexcept { return m_value < right.m_value; }
	bool operator<=(const GF256Num& right) const noexcept { return m_value < right.m_value || m_value == right.m_value; }
	bool operator>(const GF256Num& right) const noexcept { return !(m_value <= right.m_value); }
	bool operator>=(const GF256Num& right) const noexcept { return !(m_value < right.m_value); }
	bool operator!=(const GF256Num& right) const noexcept { return !(m_value == right.m_value); }

	explicit operator bool() const noexcept { return m_value; }
	//operator uint8_t() const noexcept{ return m_value; }

	void inverse() noexcept;
	GF256Num inverseCopy() const noexcept;
	GF256Num powCopy(const uint8_t pow) const noexcept;
	GF256Num logCopy() const noexcept { return logTable[m_value]; }

	uint8_t value() const noexcept { return m_value; }

	static const GF256Num primitive;
private:
	void init();
	static const uint16_t fieldSize = 256;
	static std::array<uint8_t, fieldSize> powTable;
	static std::array<uint8_t, fieldSize> logTable;
	static bool isInited;
	uint8_t m_value;
};

std::ostream& operator<<(std::ostream& out, const GF256Num& num);

#endif // !_GF256_NUM_H_


