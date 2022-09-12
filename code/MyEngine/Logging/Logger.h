#pragma once
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
