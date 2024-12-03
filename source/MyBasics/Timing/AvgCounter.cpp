#include "AvgCounter.h"

#include <sstream>
#include <Logger\Logger.h>

using namespace MyEngine::Timing;

AvgCounter::AvgCounter(std::string name, unsigned interval)
	: m_Name{ name }
	, m_Interval{ interval }
	, m_Count{ 0 }
	, m_Avg{ 0 }
	, m_Scale{ 1.0 / interval }
	, m_Prev{ App::Time::Clock::now() }
{
}

void AvgCounter::Begin()
{
	m_Prev = App::Time::Clock::now();
}

void AvgCounter::End()
{
	const auto now = App::Time::Clock::now();
	const auto dur = now - m_Prev;
	const double nrMilis = App::Time::ToMilliseconds(dur);

	m_Avg += nrMilis * m_Scale;
	m_Count++;

	if (m_Count == m_Interval)
	{
		Print();
		Reset();
	}
}

void AvgCounter::Print() const
{
	std::stringstream ss{};
	ss << m_Name << "(" << m_Interval << ") took ";

	if (m_Avg < 1000)
		ss << m_Avg << " milliseconds";
	else
		ss << m_Avg * 1000 << " seconds";

	Logger::Print(ss.str());
}

void AvgCounter::Reset()
{
	m_Avg = 0;
	m_Count = 0;
}
