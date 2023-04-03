#pragma once

#include <DataStructures/Array.h>
#undef max
#undef min

namespace MyEngine
{
	namespace Ds
	{
		class Algorithms
		{
		public:
			Algorithms() = delete;

			template<typename T>
			static T GetBiggest(const Ds::Array<T>& array);

			template<typename T>
			static void GetMinAndMax(const Array<T>& array, T& min, T& max);

			template<typename T>
			static void SortSmallToBig(Array<T>& array);
		};

		template <typename T>
		T Algorithms::GetBiggest(const Ds::Array<T>& a)
		{
			T biggest = a[0];
			for (int i = 1; i < a.GetSize(); i++)
				if (a[i] > biggest)
					biggest = a[i];
			return biggest;
		}

		template <typename T>
		void Algorithms::GetMinAndMax(const Array<T>& array, T& min, T& max)
		{
			min = std::numeric_limits<T>::max();
			max = std::numeric_limits<T>::min();
			for (int i = 0; i < array.GetSize(); i++)
			{
				if (array[i] < min) min = array[i];
				if (array[i] > max) max = array[i];
			}
		}

		template <typename T>
		void Algorithms::SortSmallToBig(Array<T>& array)
		{
			for(int iFirst = 0; iFirst < array.GetSize()-1; iFirst++)
			{
				T value{ array[iFirst] };
				int valueIdx{ iFirst };
				for(int iSecond = iFirst+1; iSecond < array.GetSize(); iSecond++)
				{
					if(array[iSecond] < value)
					{
						value = array[iSecond];
						valueIdx = iSecond;
					}
				}
				if (valueIdx == iFirst) continue;
				array[valueIdx] = array[iFirst];
				array[iFirst] = value;
			}
		}
	}
}