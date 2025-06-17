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

		static std::string ToString(const std::string& string);
		static std::string ToString(const std::wstring& wString);
		static std::string ToString(char c);
		static std::string ToString(const Quaternion& quat);

		template<typename T>
		static std::string ToString(const Vector2<T>& vector);
		template<typename T>
		static std::string ToString(const Vector3<T>& vector);
		template<typename T>
		static std::string ToString(const Vector4<T>& vector);

		template<typename T>
		static std::string ToString(const Array<T>& array);

		template<typename T>
		static std::string ToString(const List<T>& list);

		template<typename T>
		static std::string ToString(T value);

		static std::wstring ToWString(const std::string& string);
	};

	template<typename T>
	std::string Convert::ToString(const Vector2<T>& vector)
	{
		std::stringstream ss{};
		ss << "(" << ToString(vector.x)
			<< ", " << ToString(vector.y)
			<< ")";
		return ss.str();
	}

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
	std::string Convert::ToString(const Vector4<T>& vector)
	{
		std::stringstream ss{};
		ss << "(" << ToString(vector.x)
			<< ", " << ToString(vector.y)
			<< ", " << ToString(vector.z)
			<< ", " << ToString(vector.w)
			<< ")";
		return ss.str();
	}

	template<typename T>
	inline std::string Convert::ToString(const Array<T>& array)
	{
		std::stringstream ss{};
		ss << "Array[" << array.GetSize() << "]" << std::endl;
		for (unsigned i{ 0 }; i < array.GetSize(); ++i)
		{
			ss << " [" << i << "] ";
			ss << ToString(array[i]) << std::endl;;
		}
		return ss.str();
	}

	template<typename T>
	inline std::string Convert::ToString(const List<T>& list)
	{
		std::stringstream ss{};
		ss << "List[" << list.GetSize() << "," << list.GetCapacity() << "]" << std::endl;
		for (unsigned i{ 0 }; i < list.GetSize(); ++i)
		{
			ss << " [" << i << "] ";
			ss << ToString(list[i]) << std::endl;;
		}
		return ss.str();
	}

	template <typename T>
	std::string Convert::ToString(T value)
	{
		return std::to_string(value);
	}
}
