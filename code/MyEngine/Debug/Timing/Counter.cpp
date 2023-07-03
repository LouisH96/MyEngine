#include "pch.h"
#include "Counter.h"

#include <sstream>

Timing::Counter::Counter()
	: m_Begin(App::Time::Clock::now())
{
}

void Timing::Counter::Begin()
{
	m_Begin =App::Time::Clock::now();
}

void Timing::Counter::End(const std::string& name) const
{
	const auto now = App::Time::Clock::now();
	const auto dur = now - m_Begin;

	std::stringstream ss{};
	ss << name << " took ";

	const double nrMillis = App::Time::ToMilliseconds(dur);
	if (nrMillis < 0)
		ss << dur.count() << " ticks";
	else if (nrMillis < 1000)
		ss << nrMillis << " milliseconds";
	else
		ss << App::Time::ToSeconds(dur) << " seconds";

	Logger::Print(ss.str());
}
