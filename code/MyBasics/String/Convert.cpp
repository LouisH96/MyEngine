#include "Convert.h"

#include <codecvt>

using namespace MyEngine;

char Convert::ToChar(unsigned oneNumber)
{
	return static_cast<char>('0' + oneNumber);
}

std::string Convert::ToString(const std::wstring& wString)
{
	using Converter = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<Converter, wchar_t> converter;
	return converter.to_bytes(wString);
}
