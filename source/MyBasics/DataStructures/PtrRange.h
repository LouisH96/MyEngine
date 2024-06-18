// ReSharper disable CppNonExplicitConvertingConstructor
#pragma once
#include "Array.h"
#include "List.h"

namespace MyEngine
{
	template<typename Data>
	class PtrRange
	{
	public:
		PtrRange(const Array<Data>& source);
		PtrRange(const List<Data>& source);

		Data* pData;
		unsigned count;
	};

	template <typename Data>
	PtrRange<Data>::PtrRange(const Array<Data>& source)
		: pData{ source.GetData() }
		, count{ source.GetSizeU() }
	{
	}

	template <typename Data>
	PtrRange<Data>::PtrRange(const List<Data>& source)
		: pData{ source.GetData() }
		, count{ source.GetSizeU() }
	{
	}
}