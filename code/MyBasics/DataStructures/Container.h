#pragma once
#include <DataStructures\List.h>

namespace MyEngine
{
	//---| Main Class |---
	template<typename Source>
	class Container
	{
		public:
			using Data = typename Source::DataType;

			Container(Source& source);

			void Add(const Data& value);
			void Add(const Data& value1, const Data& value2);
			void Add(const Data& value1, const Data& value2, const Data& value3);

			void EnsureIncrease(unsigned increase);

			Data& Get(unsigned index);
			Data& GetPrevious(unsigned timesBack = 1);
			unsigned GetSize() const;

		private:
			Source* m_pSource;
	};

	//---| Specialized Classes |---
	//todo: raw pointer

	//---| Main Implemenation |---
	template<typename Source>
	inline Container<Source>::Container(Source& source)
		: m_pSource{ &source }
	{
	}

	template<typename Source>
	inline void Container<Source>::Add(const Data& value)
	{
		m_pSource->Add(value);
	}

	template<typename Source>
	inline void Container<Source>::Add(const Data& value1, const Data& value2)
	{
		m_pSource->Add(value1, value2);
	}

	template<typename Source>
	inline void Container<Source>::Add(const Data& value1, const Data& value2, const Data& value3)
	{
		m_pSource->Add(value1, value2, value3);
	}

	template<typename Source>
	inline void Container<Source>::EnsureIncrease(unsigned increase)
	{
		m_pSource->EnsureIncrease(increase);
	}

	template<typename Source>
	inline unsigned Container<Source>::GetSize() const
	{
		return m_pSource->GetSize();
	}
}