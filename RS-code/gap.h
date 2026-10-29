#ifndef _GAP_H_
#define _GAP_H_

#pragma once
template <typename T>
class Gap
{
public:
	constexpr explicit Gap(T start = {}, T end = {}) : m_start(start), m_end(end) {}
	virtual bool isInclude(T num) const noexcept = 0;

	T start() const noexcept { return m_start; }
	void setStart(T start) noexcept { m_start = start; }
	T end() const noexcept { return m_end; }
	void setEnd(T end) noexcept{ m_end = end; }
	T length() const noexcept { return m_end - m_start; }

	virtual ~Gap() = default;
protected:
	T m_start, m_end;
};

#endif // !_GAP_H_




