#pragma once
#include "DataStructures/Array.h"

namespace MyEngine
{
	template<typename T>
	class ArrayAdder
	{
	public:
		ArrayAdder(Array<T>& array, unsigned startIdx = 0);

		void Add(const T& value);
		void Add(const T& value1, const T& value2, const T& value3);

	private:
		T* m_pCurrent;
	};

	template <typename T>
	ArrayAdder<T>::ArrayAdder(Array<T>& array, unsigned startIdx)
		: m_pCurrent{ &array.GetData()[startIdx] }
	{

	}

	template <typename T>
	void ArrayAdder<T>::Add(const T& value)
	{
		*m_pCurrent++ = value;
	}

	template <typename T>
	void ArrayAdder<T>::Add(const T& value1, const T& value2, const T& value3)
	{
		*m_pCurrent++ = value1;
		*m_pCurrent++ = value2;
		*m_pCurrent++ = value3;
	}
}
