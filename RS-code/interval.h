#ifndef _INTERVAL_H_
#define _INTERVAL_H_

#pragma once

#include "gap.h"
#pragma once

template <typename T>
class Interval : public Gap<T>
{
public:
	constexpr explicit Interval(T start = {}, T end = {}) : Gap<T>(start, end) {}
	virtual bool isInclude(T num) const noexcept override { return num >= this->m_start && num < this->m_end; }
};

#endif // !_INTERVAL_H_




