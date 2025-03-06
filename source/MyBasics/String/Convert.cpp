#include "Convert.h"

#include "Math/Quaternion.h"

#include <cstdlib>

using namespace MyEngine;

char Convert::ToChar(unsigned oneNumber)
{
	return static_cast<char>('0' + oneNumber);
}

std::string Convert::ToString(const std::wstring& wString)
{
	//https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/wcstombs-s-wcstombs-s-l?view=msvc-170
	const size_t bufferLength{ wString.length() + 1 };
	char* pBuffer{ new char[bufferLength] };

	size_t convertedSize{}; //in bytes
	wcstombs_s(
		&convertedSize,
		pBuffer, bufferLength,
		wString.c_str(), bufferLength - 1);

	const std::string result{ pBuffer };
	delete[] pBuffer;
	return result;
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
