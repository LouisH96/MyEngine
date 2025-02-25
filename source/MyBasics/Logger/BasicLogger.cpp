#include "BasicLogger.h"

#include <iostream>
#include <Windows.h>

using namespace MyEngine;

std::chrono::high_resolution_clock::time_point BasicLogger::m_BeginDay{};
char BasicLogger::m_Timestamp[TIMESTAMP_LENGTH]{};

void BasicLogger::Init()
{
	//---| Time |---
	using namespace std::chrono;

	//prepare convert
	const system_clock::time_point sysClockNow{ system_clock::now() };
	const high_resolution_clock::time_point highClockNow{
		high_resolution_clock::now() };

	const system_clock::duration sysToHigh{
		duration_cast<system_clock::duration>(highClockNow.time_since_epoch())
		- sysClockNow.time_since_epoch() };

	//time_t
	const std::time_t currentTime{
		system_clock::to_time_t(sysClockNow) };

	//tm
	tm tmStruct{};
	localtime_s(&tmStruct, &currentTime);
	tmStruct.tm_hour = 0;
	tmStruct.tm_min = 0;
	tmStruct.tm_sec = 0;

	//time_t
	const std::time_t midnightT{ std::mktime(&tmStruct) };

	//time_point
	const system_clock::time_point midnight{
		system_clock::from_time_t(midnightT) };

	//convert to high
	const high_resolution_clock::time_point midnightHigh{
		duration_cast<high_resolution_clock::duration>(
			(midnight + sysToHigh).time_since_epoch()) };

	m_BeginDay = midnightHigh;

	//---| Timestamp |---
	m_Timestamp[0] = '[';
	m_Timestamp[TIMESTAMP_LENGTH - 3] = ']';
	m_Timestamp[TIMESTAMP_LENGTH - 2] = ' ';
	m_Timestamp[TIMESTAMP_LENGTH - 1] = '\0';

	m_Timestamp[TIMESTAMP_BEGIN_HOURS + 2] = ':';
	m_Timestamp[TIMESTAMP_BEGIN_MINUTES + 2] = ':';
	m_Timestamp[TIMESTAMP_BEGIN_SECONDS + 2] = ':';

	//---| Console Text Color |---
	SetColorWhite();
}

void BasicLogger::Print(const std::string& msg)
{
	UpdateTimestamp();
	std::cout << m_Timestamp << msg << std::endl;
}

void BasicLogger::Print(const std::string& prefix, const std::string& msg)
{
	UpdateTimestamp();
	std::cout << m_Timestamp << prefix << ": " << msg << std::endl;
}

void BasicLogger::BeginPrint()
{
	UpdateTimestamp();
	std::cout << m_Timestamp;
}

void BasicLogger::BeginPrint(const std::string& msg)
{
	UpdateTimestamp();
	std::cout << m_Timestamp << msg;
}

void BasicLogger::ContinuePrint(const std::string& msg)
{
	std::cout << msg;
}

void BasicLogger::EndPrint()
{
	std::cout << std::endl;
}

void BasicLogger::EndPrint(const std::string& msg)
{
	std::cout << msg << std::endl;
}

void BasicLogger::Warning(const std::string& prefix, const std::string& msg)
{
	SetColorYellow();
	Print(prefix, msg);
	SetColorWhite();
}

void BasicLogger::Warning(const std::string& msg)
{
	SetColorYellow();
	Print(msg);
	SetColorWhite();
}

void BasicLogger::BeginWarning()
{
	SetColorYellow();
	BeginPrint();
}

void BasicLogger::BeginWarning(const std::string& msg)
{
	SetColorYellow();
	BeginPrint(msg);
}

void BasicLogger::ContinueWarning(const std::string& msg)
{
	ContinuePrint(msg);
}

void BasicLogger::EndWarning()
{
	EndPrint();
	SetColorWhite();
}

void BasicLogger::EndWarning(const std::string& msg)
{
	EndPrint(msg);
	SetColorWhite();
}

void BasicLogger::Error(const std::string& prefix, const std::string& msg)
{
	SetColorRed();
	Print(prefix, msg);
	SetColorWhite();
}

void BasicLogger::Error(const std::string& msg)
{
	SetColorRed();
	Print(msg);
	SetColorWhite();
}

void BasicLogger::BeginError()
{
	SetColorRed();
	BeginPrint();
}

void BasicLogger::BeginError(const std::string& msg)
{
	SetColorRed();
	BeginPrint(msg);
}

void BasicLogger::ContinueError(const std::string& msg)
{
	ContinuePrint(msg);
}

void BasicLogger::EndError()
{
	EndPrint();
	SetColorWhite();
}

void BasicLogger::EndError(const std::string& msg)
{
	EndPrint(msg);
	SetColorWhite();
}

void BasicLogger::UpdateTimestamp()
{
	using namespace std::chrono;

	Duration time{ Clock::now() - m_BeginDay };
	long long total;
	long long display;

	UpdateTimestamp<hours, 2>(time, total, display, TIMESTAMP_BEGIN_HOURS);
	UpdateTimestamp<minutes, 2>(time, total, display, TIMESTAMP_BEGIN_MINUTES);
	UpdateTimestamp<seconds, 2>(time, total, display, TIMESTAMP_BEGIN_SECONDS);
	UpdateTimestamp<milliseconds, 3>(time, total, display, TIMESTAMP_BEGIN_MILLIS);
}

void BasicLogger::SetColorYellow()
{
	SetColor(14);
}

void BasicLogger::SetColorRed()
{
	SetColor(12);
}

void BasicLogger::SetColorWhite()
{
	SetColor(7);
}

void BasicLogger::SetColor(short attribute)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attribute);
}

