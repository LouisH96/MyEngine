#pragma once

namespace MyEngine
{
template<typename T, unsigned TSize>
class StackArray
{
public:
	static constexpr unsigned Size{ TSize };
	
	StackArray() = default;
	StackArray(const T& initValue);

	StackArray(const StackArray& other) noexcept;
	StackArray(StackArray&& other) noexcept;

	StackArray& operator=(const StackArray& other);
	StackArray& operator=(StackArray&& other);

	const T& operator[](unsigned index) const { return m_Data[index]; }
	T& operator[](unsigned index) { return m_Data[index]; }

private:
	T m_Data[TSize]{};
};
template<typename T, unsigned TSize>
inline StackArray<T, TSize>::StackArray(const T& initValue)
{
	for (unsigned i{ 0 }; i < TSize; ++i)
		m_Data[i] = initValue;
}
template<typename T, unsigned TSize>
inline StackArray<T, TSize>::StackArray(const StackArray& other) noexcept
{
	std::copy(other.m_Data, &other.m_Data[TSize], m_Data);
}
template<typename T, unsigned TSize>
inline StackArray<T, TSize>::StackArray(StackArray&& other) noexcept
{
	std::move(other.m_Data, &other.m_Data[TSize], m_Data);
}
template<typename T, unsigned TSize>
inline StackArray<T, TSize>& StackArray<T, TSize>::operator=(const StackArray& other)
{
	if (this == &other) return *this;
	std::copy(other.m_Data, &other.m_Data[TSize], m_Data);
	return *this;
}
template<typename T, unsigned TSize>
inline StackArray<T, TSize>& StackArray<T, TSize>::operator=(StackArray&& other)
{
	if (this == &other) return *this;
	std::move(other.m_Data, &other.m_Data[TSize], m_Data);
	return *this;
}
}