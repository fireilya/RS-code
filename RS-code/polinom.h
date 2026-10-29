#ifndef _POLINOM_H_
#define _POLINOM_H_

#pragma once

#include <vector>
#include <unordered_map>
#include <iostream>

template <typename T>
class Polinom;

template<typename T>
std::ostream& operator<<(std::ostream& out, const Polinom<T>& poly);

template <typename T>
class Polinom
{
public:
	explicit Polinom(const size_t initDegree = 0) noexcept : m_data(initDegree + 1) {};
	explicit Polinom(const std::vector<T>& data);
	explicit Polinom(const std::unordered_map<size_t, T>& coeffMap);

	Polinom<T> operator+ (const Polinom& right) const;
	Polinom<T> operator- (const Polinom& right) const;
	Polinom<T> operator* (const Polinom& right) const;
	Polinom<T> operator/ (const Polinom& right) const;
	Polinom<T> operator% (const Polinom& right) const;

	void operator+= (const Polinom& right);
	void operator-= (const Polinom& right);
	void operator*= (const Polinom& right);
	void operator/= (const Polinom& right);
	void operator%= (const Polinom& right);

	const T& operator[] (size_t degree) const { return m_data[degree]; }
	T& operator[] (size_t degree) { return m_data[degree]; }

	void normalize();
	T eval(T argValue) const;
	void pow(size_t pow);
	Polinom<T> powCopy(size_t pow) const;
	void scale(T scale);
	Polinom<T> scaleCopy(T scale) const;

	void setDegree(size_t degree);
	size_t degree() const noexcept { return m_data.empty() ? 0 : m_data.size() - 1; }
	const std::vector<T>& coeffs() { return m_data; }
	friend std::ostream& operator<< <T>(std::ostream& out, const Polinom<T>& poly);
private:
	std::vector<T> m_data;
};

#endif // !_POLINOM_H_

#pragma region PolinomRealization

template<typename T>
inline Polinom<T>::Polinom(const std::vector<T>& data) : m_data(data)
{
	if (m_data.empty()) m_data.emplace_back(T());
}

template<typename T>
inline Polinom<T>::Polinom(const std::unordered_map<size_t, T>& coeffMap)
{
	if (coeffMap.empty())  m_data.emplace_back(T());

	else
	{
		size_t degree = std::max_element(coeffMap.cbegin(), coeffMap.cend(),
			[](const std::pair<size_t, T>& left, const std::pair<size_t, T>& right) { return left.first < right.first; })->first;
		m_data = std::vector<T>(degree + 1);
		for (const auto& pair : coeffMap) m_data[pair.first] = pair.second;
	}
}

template<typename T>
inline Polinom<T> Polinom<T>::operator+(const Polinom& right) const
{
	Polinom<T> leftCopy = *this;
	Polinom<T> rightCopy = right;

	leftCopy.normalize();
	rightCopy.normalize();

	Polinom<T> answer = leftCopy;
	if (answer.degree() < rightCopy.degree()) answer.setDegree(rightCopy.degree());
	size_t minDegree = std::min(degree(), rightCopy.degree());
	for (size_t i = 0; i <= minDegree; ++i) answer.m_data[i] += rightCopy.m_data[i];

	return answer;
}
template<typename T>
inline Polinom<T> Polinom<T>::operator-(const Polinom& right) const
{
	Polinom<T> leftCopy = *this;
	Polinom<T> rightCopy = right;

	leftCopy.normalize();
	rightCopy.normalize();

	Polinom<T> answer = leftCopy;
	if (answer.degree() < rightCopy.degree()) answer.setDegree(rightCopy.degree());
	size_t minDegree = std::min(degree(), rightCopy.degree());
	for (size_t i = 0; i <= minDegree; ++i) answer.m_data[i] -= rightCopy.m_data[i];

	return answer;
}
template<typename T>
inline Polinom<T> Polinom<T>::operator*(const Polinom& right) const
{
	Polinom<T> leftCopy = *this;
	Polinom<T> rightCopy = right;

	leftCopy.normalize();
	rightCopy.normalize();

	Polinom<T> answer(leftCopy.degree() + rightCopy.degree());
	for (size_t i = 0; i < leftCopy.m_data.size(); i++)
	{
		for (size_t j = 0; j < rightCopy.m_data.size(); j++) answer.m_data[i + j] += leftCopy.m_data[i] * rightCopy.m_data[j];
	}
	answer.normalize();
	return answer;
}

template<typename T>
inline Polinom<T> Polinom<T>::operator/(const Polinom& right) const
{
	Polinom<T> leftCopy = *this;
	Polinom<T> rightCopy = right;

	leftCopy.normalize();
	rightCopy.normalize();

	Polinom<T> answer;
	if (rightCopy.degree() > leftCopy.degree()) return answer;
	
	answer.setDegree(leftCopy.degree() - rightCopy.degree());
	Polinom<T> divisibleBuf = leftCopy;

	while (divisibleBuf.degree() >= rightCopy.degree())
	{
		T currentQuotientCoeff = divisibleBuf.m_data[divisibleBuf.degree()] / rightCopy.m_data[rightCopy.degree()];
		size_t currentQuotientDegree = divisibleBuf.degree() - rightCopy.degree();
		answer.m_data[currentQuotientDegree] = currentQuotientCoeff;
		divisibleBuf -= rightCopy * Polinom<T>(std::unordered_map<size_t, T>({ {currentQuotientDegree, currentQuotientCoeff} }));
	}

	return answer;
}

template<typename T>
inline Polinom<T> Polinom<T>::operator%(const Polinom& right) const
{
	Polinom<T> leftCopy = *this;
	Polinom<T> rightCopy = right;

	leftCopy.normalize();
	rightCopy.normalize();

	if (rightCopy.degree() > leftCopy.degree()) return leftCopy;

	Polinom<T> answer = leftCopy;

	while (answer.degree() >= rightCopy.degree())
	{
		T currentQuotientCoeff = answer.m_data[answer.degree()] / rightCopy.m_data[rightCopy.degree()];
		size_t currentQuotientDegree = answer.degree() - rightCopy.degree();
		answer -= rightCopy * Polinom<T>(std::unordered_map<size_t, T>({ {currentQuotientDegree, currentQuotientCoeff} }));
	}

	return answer;
}

template<typename T>
inline void Polinom<T>::operator+=(const Polinom& right)
{
	Polinom<T> rightCopy = right;

	rightCopy.normalize();
	normalize();

	if (degree() < rightCopy.degree()) setDegree(rightCopy.degree());
	size_t minDegree = std::min(degree(), rightCopy.degree());
	for (size_t i = 0; i <= minDegree; i++) m_data[i] += rightCopy.m_data[i];

	normalize();
}

template<typename T>
inline void Polinom<T>::operator-=(const Polinom& right)
{
	Polinom<T> rightCopy = right;

	rightCopy.normalize();
	normalize();

	if (degree() < rightCopy.degree()) setDegree(rightCopy.degree());
	size_t minDegree = std::min(degree(), rightCopy.degree());
	for (size_t i = 0; i <= minDegree; i++) m_data[i] -= rightCopy.m_data[i];

	normalize();
}

template<typename T>
inline void Polinom<T>::operator*=(const Polinom& right)
{
	*this = *this * right;
}

template<typename T>
inline void Polinom<T>::operator/=(const Polinom& right)
{
	*this = *this / right;
}

template<typename T>
inline void Polinom<T>::operator%=(const Polinom& right)
{
	Polinom<T> rightCopy = right;

	rightCopy.normalize();
	normalize();

	if (degree() < rightCopy.degree()) return;

	while (degree() >= rightCopy.degree())
	{
		size_t currentQuotientCoeff = m_data[degree()] / rightCopy.m_data[rightCopy.degree()];
		size_t currentQuotientDegree = degree() - rightCopy.degree();
		*this -= rightCopy * Polinom<T>(std::unordered_map<size_t, T>({ {currentQuotientDegree, currentQuotientCoeff} }));
	}
}

template<typename T>
inline void Polinom<T>::normalize()
{
	m_data.erase((std::find_if(m_data.crbegin(), m_data.crend(), [](const auto& el) { return el; })).base(), m_data.end());
	if (m_data.empty()) m_data.emplace_back(T());
}

template<typename T>
inline T Polinom<T>::eval(T argValue) const
{
	T answer{}, argValueCopy = argValue;
	if (m_data.empty()) return answer;
	answer = m_data[0];
	for (size_t i = 1; i < m_data.size(); i++)
	{
		answer += m_data[i] * argValue;
		argValue *= argValueCopy;
	}
	return answer;
}

template<typename T>
inline void Polinom<T>::pow(size_t pow)
{
	size_t oldDegree = degree();
	setDegree(oldDegree + pow);
	for (size_t i = 0; i <= oldDegree; ++i) m_data[oldDegree + pow - i] = m_data[oldDegree - i];
	for (size_t i = 0; i < pow; ++i) m_data[i] = 0;
}

template<typename T>
inline Polinom<T> Polinom<T>::powCopy(size_t pow) const
{
	Polinom<T> answer(degree() + pow);
	for (size_t i = 0; i < degree() + 1; ++i) answer[i + pow] = m_data[i];
	return answer;
}

template<typename T>
inline void Polinom<T>::scale(T scale)
{
	for (auto& coeff : m_data) coeff *= scale;
}

template<typename T>
inline Polinom<T> Polinom<T>::scaleCopy(T scale) const
{
	Polinom<T> answer = *this;
	for (auto& coeff : answer.m_data) coeff *= scale;
	return answer;
}

template<typename T>
inline void Polinom<T>::setDegree(size_t newDegree)
{
	if (newDegree != m_data.size() - 1) m_data.resize(newDegree + 1);
}
#pragma endregion PolinomRealization

template<typename T>
std::ostream& operator<<(std::ostream& out, const Polinom<T>& poly)
{
	size_t i = poly.degree();
	while (true)
	{
		auto x = poly.m_data[i] != static_cast<T>(5);
		if (poly.m_data[i] != static_cast<T>(5) || !i) out << poly.m_data[i] << (!i ? "" : "*");
		if (!i) break;
		out << "x^" << i-- << " + ";
	}
	return out;
}

