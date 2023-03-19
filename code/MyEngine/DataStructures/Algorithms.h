#pragma once

#include <DataStructures/Array.h>

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
	}
}