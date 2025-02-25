#pragma once

#include <chrono>
#include <string>

namespace MyEngine
{
class BasicLogger
{
public:
	static void Init();

	//---| Print |---
	static void Print(const std::string& msg);
	static void Print(const std::string& prefix, const std::string& msg);

	static void BeginPrint();
	static void BeginPrint(const std::string& msg);

	static void ContinuePrint(const std::string& msg);

	static void EndPrint();
	static void EndPrint(const std::string& msg);

	//---| Warning |---
	static void Warning(const std::string& prefix, const std::string& msg);
	static void Warning(const std::string& msg);
	static void BeginWarning();
	static void BeginWarning(const std::string& msg);

	static void ContinueWarning(const std::string& msg);

	static void EndWarning();
	static void EndWarning(const std::string& msg);

	//---| Error |---
	static void Error(const std::string& prefix, const std::string& msg);
	static void Error(const std::string& msg);
	static void BeginError();
	static void BeginError(const std::string& msg);

	static void ContinueError(const std::string& msg);

	static void EndError();
	static void EndError(const std::string& msg);

private:
	BasicLogger() = delete;
	~BasicLogger() = delete;

	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = Clock::time_point;
	using Duration = Clock::duration;

	static constexpr unsigned TIMESTAMP_BEGIN_HOURS{ 1 }; //[
	static constexpr unsigned TIMESTAMP_BEGIN_MINUTES{ TIMESTAMP_BEGIN_HOURS + 2 + 1 }; //[00:
	static constexpr unsigned TIMESTAMP_BEGIN_SECONDS{ TIMESTAMP_BEGIN_MINUTES + 2 + 1 }; //[00:00:
	static constexpr unsigned TIMESTAMP_BEGIN_MILLIS{ TIMESTAMP_BEGIN_SECONDS + 2 + 1 }; //[00:00:00:
	static constexpr unsigned TIMESTAMP_LENGTH{ TIMESTAMP_BEGIN_MILLIS + 3 + 1 + 2 }; //[00:00:00:000] \0

	static TimePoint m_BeginDay;
	static char m_Timestamp[TIMESTAMP_LENGTH];

	//---| Update Timestamp |---
	static void UpdateTimestamp();
	template<unsigned TPower,unsigned TOffset>
	static void SetTimestampNumber(long long& total, long long& display, unsigned textBegin);
	template<typename TDuration, unsigned TNrNumbers>
	static void UpdateTimestamp(Duration& time, long long& total, long long& display, unsigned textBegin);
	template<unsigned TPower>
	static unsigned GetPower10();

	//---| Console Text Color |---
	static void SetColorYellow();
	static void SetColorRed();
	static void SetColorWhite();
	static void SetColor(short attribute);
};

template<unsigned TPower, unsigned TOffset>
inline void BasicLogger::SetTimestampNumber(long long& total, long long& display, unsigned textBegin)
{
	display = total / GetPower10<TPower>();
	total -= display * GetPower10<TPower>();
	m_Timestamp[textBegin + TOffset] = '0' + static_cast<char>(display);
}

template<typename TUnit, unsigned TNrNumbers>
inline void BasicLogger::UpdateTimestamp(Duration& time, long long& total, long long& display, unsigned textBegin)
{
	const TUnit inUnits{ std::chrono::duration_cast<TUnit>(time) };
	time -= inUnits;
	total = inUnits.count();
	if (TNrNumbers == 2)
	{
		SetTimestampNumber<1,0>(total, display, textBegin);
		SetTimestampNumber<0,1>(total, display, textBegin);
	}
	else if (TNrNumbers == 3)
	{
		SetTimestampNumber<2,0>(total, display, textBegin);
		SetTimestampNumber<1,1>(total, display, textBegin);
		SetTimestampNumber<0,2>(total, display, textBegin);
	}
}

template<>
inline unsigned BasicLogger::GetPower10<0>()
{
	return 1;
}
template<>
inline unsigned BasicLogger::GetPower10<1>()
{
	return 10;
}
template<>
inline unsigned BasicLogger::GetPower10<2>()
{
	return 100;
}

}