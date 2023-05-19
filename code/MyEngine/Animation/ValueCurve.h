#pragma once
#include "DataStructures/SortedArray.h"
#include "Math/Float.h"

namespace MyEngine
{
	namespace Animation
	{
		template<typename T>
		class ValueCurve
		{
		public:
			ValueCurve() = default;
			ValueCurve(const SortedArray<int64_t>& times, const SortedArray<T>& values);

			T ValueAtTime(const int64_t& time) const;

		private:
			SortedArray<int64_t> m_Times;
			SortedArray<T> m_Values;
		};

		//Times & values should be sorted
		template <typename T>
		ValueCurve<T>::ValueCurve(const SortedArray<int64_t>& times, const SortedArray<T>& values)
			: m_Times{ times }
			, m_Values{ values }
		{
			if (times.GetSize() != m_Values.GetSize())
				Logger::PrintError("Times and Values should have equal lengths");
		}

		template <typename T>
		T ValueCurve<T>::ValueAtTime(const int64_t& time) const
		{
			int idxBefore, idxAfter;
			m_Times.GetSurroundingIndices(time, idxBefore, idxAfter);

			if(idxBefore == -1 || idxAfter == -1)
			{
				Logger::PrintError("Time is out of range");
				return 0;
			}
			const float alpha{ Float::Unlerp(time, m_Times[idxBefore], m_Times[idxAfter]) };
			return Float::Lerp(alpha, m_Values[idxBefore], m_Values[idxAfter]);
		}
	}
}

