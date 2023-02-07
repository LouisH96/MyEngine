#pragma once
#include <DirectXMath.h>
#include <string>

namespace MyEngine
{
	namespace Math
	{
		union Int2;
	}
}

namespace MyEngine
{
	namespace Logging
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
			static void PrintXY(const Math::Int2& vector);
			static void PrintXY(const std::string& prefix, const Math::Int2& vector);

			static void PrintError(const std::string& message);

			static std::string ToString(float x, float y, float z);
			static std::string ToString(const DirectX::XMFLOAT3& vector);
			static std::string ToStringXYZ(const DirectX::XMVECTOR& vector);
			static std::string ToStringXYZ(const float* x);
			static std::string ToStringXY(const Math::Int2& vector);

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
