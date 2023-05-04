#pragma once
#include <DirectXMath.h>
#include <sstream>
#include <string>
#include <bitset>
#include <vector>

#include "DataStructures/Array.h"
#include "ToString.h"

namespace MyEngine
{
	namespace Debug
	{
		class Logger
		{
		public:
			static std::string GetLogLine(const std::string& logMessage); //returns: [13:20:13]: MyMessage/n
			static void Print(const std::string& logMessage); //print to console: [13:20:13]: MyMessage/n

			static void Print(const std::string& prefix, float value);
			static void Print(float x, float y, float z);
			static void Print(const DirectX::XMFLOAT3& vector);
			static void PrintXYZ(const DirectX::XMVECTOR& vector);
			static void PrintXYZ(const float* x);
			static void PrintXYZ(const Math::Float3& vector);
			static void PrintXYZ(const std::string& prefix, const Math::Float3& vector);
			static void PrintXY(const Math::Int2& vector);
			static void PrintXY(const std::string& prefix, const Math::Int2& vector);

			static void PrintError(const std::string& message);
			static void PrintWarning(const std::string& message);

			static std::string ToString(float x, float y, float z);
			static std::string ToString(const DirectX::XMFLOAT3& vector);
			static std::string ToStringXYZ(const DirectX::XMVECTOR& vector);
			static std::string ToStringXYZ(const float* x);
			static std::string ToStringXYZ(const Math::Float3& vector);
			static std::string ToStringXY(const Math::Int2& vector);

			template <typename T>
			static void ToStringPrint(const T& value);

			template<typename T>
			static void Print(const std::string& description, const Ds::Array<T>& array);

			template<typename T>
			static void PrintBinary(const std::string& description, const Ds::Array<T>& array);

			template<typename T>
			static void Print(const std::string& description, const std::vector<T>& vector);

			template<typename T>
			static void Print(const std::string& description, const T& value);

			template<typename T>
			static void PrintToString(const std::string& description, const T& value);
		};

		template <typename T>
		void Logger::ToStringPrint(const T& value)
		{
			Print(std::to_string(value));
		}

		template <typename T>
		void Logger::Print(const std::string& description, const Ds::Array<T>& array)
		{
			std::stringstream ss{};
			ss << description << "[" << array.GetSize() << "]\n";
			for (int i = 0; i < array.GetSize(); i++)
				ss << " [" << i << "] " << std::to_string(array[i]) << std::endl;
			Print(ss.str());
		}

		template <typename T>
		void Logger::PrintBinary(const std::string& description, const Ds::Array<T>& array)
		{
			std::stringstream ss{};
			ss << description << "[" << array.GetSize() << "]\n";
			for (int i = 0; i < array.GetSize(); i++)
				ss << " [" << i << "] " << std::bitset<8 * sizeof(T)>(array[i]) << std::endl;
			Print(ss.str());
		}

		template <typename T>
		void Logger::Print(const std::string& description, const std::vector<T>& vector)
		{
			std::stringstream ss{};
			ss << description << "[" << vector.GetSize() << "]\n";
			for (int i = 0; i < vector.GetSize(); i++)
				ss << " [" << i << "] " << std::to_string(vector[i]) << std::endl;
			Print(ss.str());
		}

		template <typename T>
		void Logger::Print(const std::string& description, const T& value)
		{
			std::stringstream ss{};
			ss << description << ": " << ToString::Convert(value) << std::endl;
			Print(ss.str());
		}

		template <typename T>
		void Logger::PrintToString(const std::string& description, const T& value)
		{
			std::stringstream ss{};
			ss << description << ": " << ToString::Convert(value) << std::endl;
			Print(ss.str());
		}
	}
}
