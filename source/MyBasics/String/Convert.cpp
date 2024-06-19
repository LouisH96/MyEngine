#include "Convert.h"

#include <codecvt>
#include "Math/Quaternion.h"

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

std::string Convert::ToString(char c)
{
	return { c };
}

std::string Convert::ToString(const Quaternion& quat)
{
	std::stringstream ss{};
	ss << "(" << ToString(quat.Xyz.x)
		<< ", " << ToString(quat.Xyz.y)
		<< "," << ToString(quat.Xyz.z)
		<< ", " << ToString(quat.W) << ")";
	return ss.str();
}

std::wstring Convert::ToWString(const std::string& string)
{
	return std::wstring{ string.begin(), string.end() };
}
