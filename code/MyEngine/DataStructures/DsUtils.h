#pragma once
#include <vector>
#include <DataStructures/Array.h>

namespace MyEngine
{
	namespace Ds
	{
		class DsUtils
		{
		public:
			template<typename T>
			static Ds::Array<T> ToArray(const std::vector<T>& vector);
		};

		template <typename T>
		Ds::Array<T> DsUtils::ToArray(const std::vector<T>& vector)
		{
			Ds::Array<T> a{ static_cast<int>(vector.size()) };
			for (int i = 0; i < vector.size(); i++)
				a[i] = vector[i];
			return a;
		}
	}
}
