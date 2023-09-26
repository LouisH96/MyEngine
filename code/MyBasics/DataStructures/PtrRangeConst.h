// ReSharper disable CppNonExplicitConvertingConstructor
#pragma once
#include "Array.h"
#include "List.h"

namespace MyEngine
{
	template<typename Target, typename Data>
	const Target& ptr_range_const_getter(const Data& data)
	{
		return data;
	}

	template<typename Target, typename Data = Target, const Target& (Getter)(const Data&) = ptr_range_const_getter>
	class PtrRangeConst
	{
	public:
		typedef const Target& (GetterType)(const Data&);
		PtrRangeConst(const Array<Data>& source);
		PtrRangeConst(const List<Data>& source);
		PtrRangeConst(const Data* pData, unsigned count);

		Data const* pData;
		unsigned count;

		const Target& operator[](unsigned idx) const;
		const Target& First() const;
		const Target& Last() const;
		const Target* End() const;
	};

	template <typename Target, typename Data, const Target& (Getter)(const Data&)>
	PtrRangeConst<Target, Data, Getter>::PtrRangeConst(const Array<Data>& source)
		: pData{ source.GetData() }
		, count{ source.GetSize() }
	{
	}

	template <typename Target, typename Data, const Target& (Getter)(const Data&)>
	PtrRangeConst<Target, Data, Getter>::PtrRangeConst(const List<Data>& source)
		: pData{ source.GetData() }
		, count{ source.GetSizeU() }
	{
	}

	template <typename Target, typename Data, const Target& (Getter)(const Data&)>
	PtrRangeConst<Target, Data, Getter>::PtrRangeConst(const Data* pData, unsigned count)
		: pData{ pData }
		, count{ count }
	{
	}

	template <typename Target, typename Data, const Target& (Getter)(const Data&)>
	const Target& PtrRangeConst<Target, Data, Getter>::operator[](unsigned idx) const
	{
		return Getter(pData[idx]);
	}

	template <typename Target, typename Data, const Target& (Getter)(const Data&)>
	const Target& PtrRangeConst<Target, Data, Getter>::First() const
	{
		return (*this)[0];
	}

	template <typename Target, typename Data, const Target& (Getter)(const Data&)>
	const Target& PtrRangeConst<Target, Data, Getter>::Last() const
	{
		return (*this)[count - 1];
	}

	template <typename Target, typename Data, const Target&(* Getter)(const Data&)>
	const Target* PtrRangeConst<Target, Data, Getter>::End() const
	{
		return pData[count];
	}
}
