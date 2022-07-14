#include "Logger.h"

#include <iostream>
#include <locale>
#include <iomanip>

std::string MyEngine::Logging::Logger::GetLogLine(const std::string& logMessage)
{
	tm tm{};
	const time_t now = time(nullptr);
	localtime_s(&tm, &now);

	constexpr unsigned timeFormatLength = 11;
	std::string datetime(timeFormatLength + logMessage.size() + 1, 0);
	datetime[datetime.size() - 1] = '\n';
	std::strftime(&datetime[0], timeFormatLength + 1, "[%T] ", &tm);

	datetime.replace(timeFormatLength, logMessage.size(), logMessage);
	return datetime;
}

void MyEngine::Logging::Logger::Print(const std::string& logMessage)
{
	std::cout << GetLogLine(logMessage);
}
