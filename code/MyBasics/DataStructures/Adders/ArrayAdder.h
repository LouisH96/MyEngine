#pragma once
#include "DataStructures/Array.h"

namespace MyEngine
{
	template<typename T>
	class ArrayAdder
	{
	public:
		ArrayAdder(Array<T>& array, unsigned startIdx);

		void Add(const T& value);

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
}
