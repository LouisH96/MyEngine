#pragma once
#include <DirectXMath.h>
#include <string>

namespace MyEngine
{
	namespace Logging
	{
		class Logger
		{
		public:
			static std::string GetLogLine(const std::string& logMessage); //returns: [13:20:13]: MyMessage/n
			static void Print(const std::string& logMessage); //print to console: [13:20:13]: MyMessage/n

			static void Print(float x, float y, float z);
			static void Print(const DirectX::XMFLOAT3& vector);
			static void PrintXYZ(const DirectX::XMVECTOR& vector);
			static void PrintXYZ(const float* x);

			static std::string ToString(float x, float y, float z);
			static std::string ToString(const DirectX::XMFLOAT3& vector);
			static std::string ToStringXYZ(const DirectX::XMVECTOR& vector);
			static std::string ToStringXYZ(const float* x);

			template <typename T>
			static void ToStringPrint(const T& value);
		};

		template <typename T>
		void Logger::ToStringPrint(const T& value)
		{
			Print(std::to_string(value));
		}
	}
}
