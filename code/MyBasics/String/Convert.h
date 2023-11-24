#pragma once
#include <string>

namespace MyEngine
{
	class Convert
	{
	public:
		static char ToChar(unsigned oneNumber);

		static std::string ToString(const std::wstring& wString);
		static std::string ToString(char c);

		template<typename T>
		static std::string ToString(T value);
	};

	template <typename T>
	std::string Convert::ToString(T value)
	{
		return std::to_string(value);
	}
}
