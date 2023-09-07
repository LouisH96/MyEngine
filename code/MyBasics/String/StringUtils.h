#pragma once
#include <string>

#include "DataStructures/List.h"

namespace MyEngine
{
	class StringUtils
	{
	public:
		static List<std::string> Split(const std::string& string, char delim);
		static bool BeginsWith(const std::string& target, const std::string& begin);
	};
}
