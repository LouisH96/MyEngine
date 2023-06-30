// ReSharper disable CppNonExplicitConvertingConstructor
#pragma once
#include "Array.h"
#include "List.h"

namespace MyEngine
{
	namespace Ds
	{
		template<typename Data>
		class PtrRangeConst
		{
		public:
			PtrRangeConst(const Array<Data>& source);
			PtrRangeConst(const List<Data>& source);

			Data const* pData;
			unsigned count;
		};

		template <typename Data>
		PtrRangeConst<Data>::PtrRangeConst(const Array<Data>& source)
			: pData{ source.GetData() }
			, count{ source.GetSizeU() }
		{
		}

		template <typename Data>
		PtrRangeConst<Data>::PtrRangeConst(const List<Data>& source)
			: pData{ source.GetData() }
			, count{ source.GetSizeU() }
		{
		}
	}
}
