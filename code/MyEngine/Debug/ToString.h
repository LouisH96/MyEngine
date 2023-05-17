#pragma once
#include "Math/Float3.h"
#include "Math/Int2.h"
#include "Math/Vector2.h"

namespace MyEngine
{
	namespace Debug
	{
		class ToString
		{
		public:

			template<typename T>
			static std::string Convert(const Math::Vector2<T>& value);

			template<typename T>
			static std::string Convert(const Math::Vector3<T>& value);

			template<typename T>
			static std::string Convert(const Ds::Array<T>& value);

			template<typename T>
			static std::string Convert(const T& value);
		};

		template <>
		inline std::string ToString::Convert(const int& value)
		{
			return std::to_string(value);
		}

		template<>
		inline std::string ToString::Convert(const Math::Int2& value)
		{
			std::stringstream ss{};
			ss << "[";
			ss << value.x << ", ";
			ss << value.y << ']';
			return ss.str();
		}

		template <typename T>
		std::string ToString::Convert(const Math::Vector2<T>& value)
		{
			std::stringstream ss{};
			ss << "[";
			ss << value.x << ", ";
			ss << value.y << ']';
			return ss.str();
		}

		template <typename T>
		std::string ToString::Convert(const Math::Vector3<T>& value)
		{
			std::stringstream ss{};
			ss << '[';
			ss << value.x << ", ";
			ss << value.y << ", ";
			ss << value.z << ']';
			return ss.str();
		}

		template <typename T>
		std::string ToString::Convert(const Ds::Array<T>& value)
		{
			std::stringstream ss{};
			ss << "Array[" << value.GetSize() << "]\n";
			for(int i = 0; i < value.GetSize(); i++)
			{
				ss << "[" << i << "] ";
				ss << Convert(value[i]) << std::endl;
			}
			return ss.str();
		}

		template<typename T>
		inline std::string ToString::Convert(const T& value)
		{
			return std::to_string(value);
		}
	}
}
