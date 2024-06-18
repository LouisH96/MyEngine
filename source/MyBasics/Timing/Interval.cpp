#include "Interval.h"

#include <chrono>

using namespace std::chrono_literals;
using namespace MyEngine::App;

MyEngine::Interval::Interval()
	: Interval{ 1s, false }
{
}

MyEngine::Interval::Interval(const Time::Duration& interval, bool startNow)
	: m_Interval{ interval }
	, m_Next{ Time::Clock::now() + (startNow ? -interval * 2 : interval) }
{
}

bool MyEngine::Interval::IsPassed()
{
	if (Time::Clock::now() < m_Next) return false;

	m_Next = Time::Clock::now() + m_Interval;
	return true;
}
