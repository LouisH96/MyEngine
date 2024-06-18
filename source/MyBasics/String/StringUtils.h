#pragma once
#include <string>

#include "DataStructures/List.h"

namespace MyEngine
{
	class StringUtils
	{
	public:
		static List<std::string> Split(const std::string& string, char delim);

		template<typename T>
		static bool BeginsWith(const std::basic_string<T>& target, const std::basic_string<T>& begin);

		template<typename T>
		static bool BeginsWith(const T* pTarget, const std::basic_string<T>& begin);
	};

	template<typename T>
	inline bool StringUtils::BeginsWith(const std::basic_string<T>& target, const std::basic_string<T>& begin)
	{
		if (target.size() < begin.size())
			return false;

		for (unsigned i = 0; i < begin.size(); i++)
			if (target[i] != begin[i])
				return false;
		return true;
	}

	template<typename T>
	inline bool StringUtils::BeginsWith(const T* pTarget, const std::basic_string<T>& begin)
	{
		for (unsigned i = 0; i < begin.size(); i++)
			if (pTarget[i] != begin[i])
				return false;
		return true;
	}
}
