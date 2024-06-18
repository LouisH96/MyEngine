#pragma once
#include <vector>

#include "Array.h"

namespace MyEngine
{
	class DsUtils
	{
	public:
		template<typename T>
		static Array<T> ToArray(const std::vector<T>& vector);
	};

	template <typename T>
	Array<T> DsUtils::ToArray(const std::vector<T>& vector)
	{
		Array<T> a{ static_cast<int>(vector.size()) };
		for (int i = 0; i < vector.size(); i++)
			a[i] = vector[i];
		return a;
	}
}
