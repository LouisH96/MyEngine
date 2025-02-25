#pragma once

#include "BasicLogger.h"

#include <DataStructures\Array.h>
#include <String\Convert.h>

#include <string>

namespace MyEngine
{
class Logger
{
public:
	//---| Print |---
	static void Print(const std::string& msg);

	template<typename T>
	static void Print(const std::string& prefix, const T& value);

	static void BeginPrint();
	static void BeginPrint(const std::string& msg);

	static void ContinuePrint(const std::string& msg);

	static void EndPrint();
	static void EndPrint(const std::string& msg);

	//---| Warning |---
	static void Warning(const std::string& msg);
	template<typename T>
	static void Warning(const std::string& prefix, const T& value);

	static void BeginWarning();
	static void BeginWarning(const std::string& msg);

	static void ContinueWarning(const std::string& msg);

	static void EndWarning();
	static void EndWarning(const std::string& msg);

	//---| Error |--
	static void Error(const std::string& msg);
	template<typename T>
	static void Error(const std::string& prefix, const T& value);
	static void BeginError();
	static void BeginError(const std::string& msg);

	static void ContinueError(const std::string& msg);

	static void EndError();
	static void EndError(const std::string& msg);

private:
	Logger() = delete;
	~Logger() = delete;
};

template<typename T>
inline void Logger::Print(const std::string& prefix, const T& value)
{
	BasicLogger::Print(prefix, Convert::ToString(value));
}
template<typename T>
inline void Logger::Warning(const std::string& prefix, const T& value)
{
	BasicLogger::Warning(prefix, Convert::ToString(value));
}
template<typename T>
inline void Logger::Error(const std::string& prefix, const T& value)
{
	BasicLogger::Error(prefix, Convert::ToString(value));
}
}
