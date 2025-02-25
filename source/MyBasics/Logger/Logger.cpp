#include "Logger.h"

using namespace MyEngine;

void Logger::Print(const std::string& msg)
{
	BasicLogger::Print(msg);
}

void Logger::BeginPrint()
{
	BasicLogger::BeginPrint();
}

void Logger::BeginPrint(const std::string& msg)
{
	BasicLogger::BeginPrint(msg);
}

void Logger::ContinuePrint(const std::string& msg)
{
	BasicLogger::ContinuePrint(msg);
}

void Logger::EndPrint()
{
	BasicLogger::EndPrint();
}

void Logger::EndPrint(const std::string& msg)
{
	BasicLogger::EndPrint(msg);
}

void Logger::Warning(const std::string& msg)
{
	BasicLogger::Warning(msg);
}

void Logger::BeginWarning()
{
	BasicLogger::BeginWarning();
}

void Logger::BeginWarning(const std::string& msg)
{
	BasicLogger::BeginWarning(msg);
}

void Logger::ContinueWarning(const std::string& msg)
{
	BasicLogger::ContinueWarning(msg);
}

void Logger::EndWarning()
{
	BasicLogger::EndWarning();
}

void Logger::EndWarning(const std::string& msg)
{
	BasicLogger::EndWarning(msg);
}

void Logger::Error(const std::string& msg)
{
	BasicLogger::Error(msg);
}

void Logger::BeginError()
{
	BasicLogger::BeginError();
}

void Logger::BeginError(const std::string& msg)
{
	BasicLogger::BeginError(msg);
}

void Logger::ContinueError(const std::string& msg)
{
	BasicLogger::ContinueError(msg);
}

void Logger::EndError()
{
	BasicLogger::EndError();
}

void Logger::EndError(const std::string& msg)
{
	BasicLogger::EndError(msg);
}
