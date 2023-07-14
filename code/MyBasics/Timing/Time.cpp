#include "Time.h"

double MyEngine::App::Time::ToSeconds(const Duration& duration)
{
	constexpr double unitInSeconds = 1;
	return static_cast<double>(duration.count()) / (Duration::period::den / unitInSeconds);
}

double MyEngine::App::Time::ToMilliseconds(const Duration& duration)
{
	constexpr double unitInSeconds = 1000;
	return static_cast<double>(duration.count()) / (Duration::period::den / unitInSeconds);
}

MyEngine::App::Time::Duration MyEngine::App::Time::DurationFromSeconds(float seconds)
{
	return Duration{ static_cast<int>(SEC_TO_CLOCK_UNIT * seconds) };
}
