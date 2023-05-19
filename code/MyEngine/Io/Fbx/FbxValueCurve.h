#pragma once
#include "DataStructures/SortedArray.h"
#include "Math/Float.h"
#include <Io/Fbx/Wrapping/AnimationCurve.h>

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			template<typename T>
			class FbxValueCurve
			{
			public:
				FbxValueCurve() = default;
				explicit FbxValueCurve(const Wrapping::AnimationCurve& curve);

				T ValueAtTime(const int64_t& time) const;

			private:
				SortedArray<int64_t> m_Times;
				SortedArray<T> m_Values;
			};

			//Times & values should be sorted
			template <typename T>
			FbxValueCurve<T>::FbxValueCurve(const Wrapping::AnimationCurve& curve)
				: m_Times{ SortedArray<int64_t>{curve.KeyTimes} }
				, m_Values{ SortedArray<float>{curve.KeyValueFloats} }
			{
				if (m_Times.GetSize() != m_Values.GetSize())
					Logger::PrintError("Times and Values should have equal lengths");
			}

			template <typename T>
			T FbxValueCurve<T>::ValueAtTime(const int64_t& time) const
			{
				int idxBefore, idxAfter;
				m_Times.GetSurroundingIndices(time, idxBefore, idxAfter);

				if (idxBefore == -1 || idxAfter == -1)
				{
					Logger::PrintError("Time is out of range");
					return 0;
				}
				const float alpha{ Float::Unlerp(time, m_Times[idxBefore], m_Times[idxAfter]) };
				return Float::Lerp(alpha, m_Values[idxBefore], m_Values[idxAfter]);
			}
		}
	}
}

