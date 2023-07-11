#pragma once

#include <sstream>
#include <string>

#include "DataStructures/Array.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"

namespace MyEngine
{
	class ToString
	{
	public:
		template<typename T>
		static std::string Convert(const Vector2<T>& value);

		template<typename T>
		static std::string Convert(const Vector3<T>& value);

		template<typename T>
		static std::string Convert(const Array<T>& value);

		template<typename T>
		static std::string Convert(const T& value);
	};

	template <typename T>
	std::string ToString::Convert(const Vector2<T>& value)
	{
		std::stringstream ss{};
		ss << "[";
		ss << value.x << ", ";
		ss << value.y << ']';
		return ss.str();
	}

	template <typename T>
	std::string ToString::Convert(const Vector3<T>& value)
	{
		std::stringstream ss{};
		ss << '[';
		ss << value.x << ", ";
		ss << value.y << ", ";
		ss << value.z << ']';
		return ss.str();
	}

	template <typename T>
	std::string ToString::Convert(const Array<T>& value)
	{
		std::stringstream ss{};
		ss << "Array[" << value.GetSize() << "]\n";
		for (int i = 0; i < value.GetSize(); i++)
		{
			ss << "[" << i << "] ";
			ss << Convert(value[i]) << std::endl;
		}
		return ss.str();
	}

	template<>
	inline std::string ToString::Convert(const bool& value)
	{
		return value ? "true" : "false";
	}

	template<>
	inline std::string ToString::Convert(const std::string& string)
	{
		return string;
	}

	template<>
	inline std::string ToString::Convert(const char& c)
	{
		return { c };
	}

	template<typename T>
	inline std::string ToString::Convert(const T& value)
	{
		return std::to_string(value);
	}
}
