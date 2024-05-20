#pragma once

namespace MyEngine
{
	template<typename T>
	class IncrementRefAdder
	{
	public:
		using DataType = T;

		IncrementRefAdder(T*& pData);

		void Add(const T& value);
		void Add(const T& value1, const T& value2, const T& value3);

	private:
		T*& m_pData;
	};

	template <typename T>
	IncrementRefAdder<T>::IncrementRefAdder(T*& pData)
		: m_pData{ pData }
	{
	}

	template <typename T>
	void IncrementRefAdder<T>::Add(const T& value)
	{
		*m_pData++ = value;
	}

	template <typename T>
	void IncrementRefAdder<T>::Add(const T& value1, const T& value2, const T& value3)
	{
		*m_pData++ = value1;
		*m_pData++ = value2;
		*m_pData++ = value3;
	}
}
