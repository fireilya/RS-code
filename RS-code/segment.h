#ifndef _SEGMENT_H_
#define _SEGMENT_H_

#pragma once

#include "gap.h"
#pragma once

template <typename T>
class Segment : public Gap<T>
{
public:
	constexpr Segment(T start = {}, T end = {}) : Gap<T>(start, end) {}
	virtual bool isInclude(T num) const noexcept override { return num >= this->m_start && num <= this->m_end; }
};

#endif // !_SEGMENT_H_
