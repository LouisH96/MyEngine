#include "pch.h"
#include "Time.h"

double App::Time::ToSeconds(const Duration& duration)
{
	constexpr double unitInSeconds = 1;
	return static_cast<double>(duration.count()) / (Duration::period::den / unitInSeconds);
}

double App::Time::ToMilliseconds(const Duration& duration)
{
	constexpr double unitInSeconds = 1000;
	return static_cast<double>(duration.count()) / (Duration::period::den / unitInSeconds);
}
