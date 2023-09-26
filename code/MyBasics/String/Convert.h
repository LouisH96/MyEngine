#pragma once
#include <string>

namespace MyEngine
{
	class Convert
	{
	public:
		static char ToChar(unsigned oneNumber);
		static std::string ToString(const std::wstring& wString);
	};
}
