#pragma once
#include <sstream>
#include <string>

#include "Math/Vectors.h"

namespace MyEngine
{
	class Quaternion;

	class Convert
	{
	public:
		static char ToChar(unsigned oneNumber);

		static std::string ToString(const std::wstring& wString);
		static std::string ToString(char c);
		static std::string ToString(const Quaternion& quat);

		template<typename T>
		static std::string ToString(const Vector3<T>& vector);

		template<typename T>
		static std::string ToString(T value);
	};

	template <typename T>
	std::string Convert::ToString(const Vector3<T>& vector)
	{
		std::stringstream ss{};
		ss << "(" << ToString(vector.x)
			<< ", " << ToString(vector.y)
			<< ", " << ToString(vector.z) << ")";
		return ss.str();
	}

	template <typename T>
	std::string Convert::ToString(T value)
	{
		return std::to_string(value);
	}
}
