#pragma once
#include "Array.h"
#include "Math/Scalar.h"

namespace MyEngine
{
	//collects for example a top3 of values inserted
	template<typename Data>
	class PeakCollection
	{
	public:
		explicit PeakCollection(unsigned amount, const Data& init = Scalar<Data>::MIN);

		void TryAdd(const Data& value);
		const Array<Data>& GetData() const { return m_Data; }

	private:
		Array<Data> m_Data;
	};

	template <typename Data>
	PeakCollection<Data>::PeakCollection(unsigned amount, const Data& init)
		: m_Data{ amount, init }
	{
	}

	template <typename Data>
	void PeakCollection<Data>::TryAdd(const Data& value)
	{
		if (value <= m_Data[0])
			return;
		for (unsigned i = 1; i < m_Data.GetSize(); i++)
			if (value < m_Data[i])
			{
				//shift previous values
				m_Data.InsertRevert(i - 1, value);
				return;
			}
		m_Data.InsertRevert(m_Data.GetSize() - 1, value);
	}
}
