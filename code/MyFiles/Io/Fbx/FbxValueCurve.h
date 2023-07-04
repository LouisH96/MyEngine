#pragma once
#include <DataStructures/SortedArray.h>
#include <Io/Fbx/Wrapping/AnimationCurve.h>

#include "Logger/Logger.h"
#include "Math/Scalars.h"

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
				explicit FbxValueCurve(const T& defaultValue);
				explicit FbxValueCurve(const Wrapping::AnimationCurve& curve);

				T ValueAtTime(const int64_t& time) const;
				void ScaleValues(float scale);

			private:
				T m_DefaultValue;
				SortedArray<int64_t> m_Times;
				Array<T> m_Values;
			};

			template <typename T>
			FbxValueCurve<T>::FbxValueCurve(const T& defaultValue)
				: m_DefaultValue{ defaultValue }
			{
			}

			//Times & values should be sorted
			template <typename T>
			FbxValueCurve<T>::FbxValueCurve(const Wrapping::AnimationCurve& curve)
				: m_DefaultValue{}
				, m_Times{ curve.KeyTimes }
				, m_Values{ curve.KeyValueFloats }
			{
				if (m_Times.GetSize() != m_Values.GetSize())
					Logger::PrintError("Times and Values should have equal lengths");
				if (!curve.KeyTimes.IsSorted())
					Logger::PrintError("[FbxValueCurve] Times is expected to be sorted");
			}

			template <typename T>
			T FbxValueCurve<T>::ValueAtTime(const int64_t& time) const
			{
				if (m_Times.GetSize() == 0) return m_DefaultValue;

				int idxBefore, idxAfter;
				m_Times.GetSurroundingIndices(time, idxBefore, idxAfter);

				if (idxBefore == -1)
					return m_Values.First();
				if (idxAfter == -1)
					return m_Values.Last();

				const float alpha{ Float::Unlerp(time, m_Times[idxBefore], m_Times[idxAfter]) };
				return Float::Lerp(alpha, m_Values[idxBefore], m_Values[idxAfter]);
			}

			template <typename T>
			void FbxValueCurve<T>::ScaleValues(float scale)
			{
				for (int i = 0; i < m_Values.GetSize(); i++)
					m_Values[i] *= scale;
			}
		}
	}
}
