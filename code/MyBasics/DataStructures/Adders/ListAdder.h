#pragma once
#include "DataStructures/List.h"

namespace MyEngine
{
	template<typename D>
	class ListAdder
	{
	public:
		using DataType = D;

		explicit ListAdder(List<D>& list);

		void Add(const D& value);
		void Add(const D& value1, const D& value2, const D& value3);

	private:
		List<D>* m_pList;
	};

	template <typename D>
	ListAdder<D>::ListAdder(List<D>& list)
		: m_pList{ &list }
	{
	}

	template <typename D>
	void ListAdder<D>::Add(const D& value)
	{
		m_pList->Add(value);
	}

	template <typename D>
	void ListAdder<D>::Add(const D& value1, const D& value2, const D& value3)
	{
		m_pList->Add(value1, value2, value3);
	}
}
