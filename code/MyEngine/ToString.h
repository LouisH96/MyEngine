#pragma once
#include "Math/Float3.h"
#include "Math/Value2.h"

namespace MyEngine
{
	namespace Debug
	{
		class ToString
		{
		public:
			template<typename T>
			static std::string Convert(const T& value);

			template<typename T>
			static std::string Convert(const Math::Value2<T>& value);
		};

		template <>
		inline std::string ToString::Convert(const int& value)
		{
			return std::to_string(value);
		}

		template<>
		inline std::string ToString::Convert(const Math::Float3& value)
		{
			std::stringstream ss{};
			ss << '[';
			ss << value.x << ", ";
			ss << value.y << ", ";
			ss << value.z << ']';
			return ss.str();
		}

		template <typename T>
		std::string ToString::Convert(const Math::Value2<T>& value)
		{
			std::stringstream ss{};
			ss << "[";
			ss << value.x << ", ";
			ss << value.y << ']';
			return ss.str();
		}
	}
}
