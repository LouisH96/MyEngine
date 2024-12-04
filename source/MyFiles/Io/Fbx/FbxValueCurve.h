#pragma once
#include "DataStructures/SortedArray.h"
#include "Io/Fbx/Wrapping/AnimationCurve.h"
#include "Logger/Logger.h"
#include "Math/Scalars.h"
#include "String/Convert.h"

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

				T ValueAtTime(const uint64_t& time) const;
				void ScaleValues(float scale);

				void Print() const;

				unsigned GetNrValues() const { return m_Times.GetSize(); }
				const SortedArray<uint64_t>& GetTimes() const { return m_Times; }
				const Array<T>& GetValues() const { return m_Values; }

			private:
				T m_DefaultValue{};
				SortedArray<uint64_t> m_Times;
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
					Logger::PrintError(
						"[FbxValueCurve] Times(" + Convert::ToString(m_Times.GetSize()) +
						") and Values(" + Convert::ToString(m_Values.GetSize()) +
						") should have equal lengths");
				if (!curve.KeyTimes.IsSorted())
					Logger::PrintError("[FbxValueCurve] Times is expected to be sorted");
			}

			template <typename T>
			T FbxValueCurve<T>::ValueAtTime(const uint64_t& time) const
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
				for (unsigned i = 0; i < m_Values.GetSize(); i++)
					m_Values[i] *= scale;
				m_DefaultValue *= scale;
			}

			template <typename T>
			void FbxValueCurve<T>::Print() const
			{
				std::cout << "NrEntries: " << m_Times.GetSize() << std::endl;
				for (unsigned i = 0; i < m_Times.GetSize(); i++)
					std::cout << Convert::ToString(m_Times[i]) << ": " << Convert::ToString(m_Values[i]) << std::endl;
			}
		}
	}
}
