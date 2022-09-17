#include "pch.h"
#include "FpsControl.h"

#include <cmath>
#include <thread>

MyEngine::App::FpsControl::FpsControl(float desiredFps)
	: m_DesiredInterval{ static_cast<int>(std::roundf(Time::SEC_TO_CLOCK_UNIT / desiredFps)) }
	, m_DesiredIntervalSec(1.f / desiredFps)
	, m_DesiredNextFrame{ static_cast<Time::Duration>(0) }
	, m_EndCurrentSecond{ static_cast<Time::Duration>(0) }
{
}

void MyEngine::App::FpsControl::Wait()
{
	const Time::TimePoint now = Time::Clock::now();

	//Find upcoming and next frame point
	Time::TimePoint upcomingFrame = m_DesiredNextFrame;
	m_DesiredNextFrame += m_DesiredInterval;
	if (now > upcomingFrame)
	{
		m_DurationLastFrameSec = static_cast<float>((m_DesiredInterval + (now - upcomingFrame)).count()) * Time::CLOCK_UNIT_TO_SEC;
		upcomingFrame = now;
		if (now > m_DesiredNextFrame)
			m_DesiredNextFrame = now + m_DesiredInterval;
	}
	else
		m_DurationLastFrameSec = m_DesiredIntervalSec;

	//Count frame in current second
	if (upcomingFrame <= m_EndCurrentSecond)
		m_NrFramesThisSec++;
	else
	{
		m_NrFramesLastSec = m_NrFramesThisSec;
		m_NrFramesThisSec = 1;
		m_EndCurrentSecond = upcomingFrame + Time::SEC_DURATION;
	}

	std::this_thread::sleep_until(upcomingFrame);
}
