#include "pch.h"
#include "FpsControl.h"

#include <thread>

#include "Rendering/FpsDisplay.h"
#include "Timing/Time.h"

App::FpsControl::FpsControl(float desiredFps, Rendering::FpsDisplay* pDisplay)
	: m_pDisplay(pDisplay)
	, m_DesiredInterval{ static_cast<int>(std::roundf(Time::SEC_TO_CLOCK_UNIT / desiredFps)) }
{
	const Time::TimePoint now{ Time::Clock::now() };
	m_BeginPrevFrame = now;
	m_BeginPrevUpdate = now;
	m_EndCurrentSecond = now + Time::SEC_DURATION;
	m_DurationLastFrame = m_DesiredInterval.count() * Time::SEC_TO_CLOCK_UNIT;
}

void App::FpsControl::Reset()
{
	const Time::TimePoint now{ Time::Clock::now() };
	m_BeginPrevFrame = now;
	m_BeginPrevUpdate = now;
	m_EndCurrentSecond = now + Time::SEC_DURATION;
}

void App::FpsControl::Wait()
{
	const Time::TimePoint now{ Time::Clock::now() };
	const Time::Duration updateDuration{ now - m_BeginPrevUpdate };
	const Time::TimePoint newUpdateBegin{ now + m_DesiredInterval - updateDuration };

	if (now > m_EndCurrentSecond)
	{
		if (m_pDisplay)
		{
			m_NrFramesPrevSec = m_NrFramesThisSec;
			m_pDisplay->SetFps(m_NrFramesThisSec);
		}
		m_NrFramesThisSec = 1;
		m_EndCurrentSecond += Time::SEC_DURATION;
	}
	else
		m_NrFramesThisSec++;

	m_DurationLastFrame = (now - m_BeginPrevFrame).count() * Time::CLOCK_UNIT_TO_SEC;
	Globals::DeltaTime = m_DurationLastFrame > .1f ? .1f : m_DurationLastFrame;
	m_BeginPrevFrame = now;

	//Way1
	//std::this_thread::sleep_until(newUpdateBegin);

	//Way2
	/*constexpr Time::Duration interval{ std::chrono::milliseconds{11} };
	if (now + interval < newUpdateBegin)
		std::this_thread::sleep_until(newUpdateBegin - interval);

	while (Time::Clock::now() < newUpdateBegin)
	{}*/

	//Way3
	constexpr Time::Duration sleepOffset{
		std::chrono::milliseconds{5} };
	std::this_thread::sleep_until(newUpdateBegin - sleepOffset);

	m_BeginPrevUpdate = Time::Clock::now();
}

void App::FpsControl::NoWait()
{
	const Time::TimePoint now{ Time::Clock::now() };

	if (now > m_EndCurrentSecond)
	{
		if (m_pDisplay)
		{
			m_NrFramesPrevSec = m_NrFramesThisSec;
			m_pDisplay->SetFps(m_NrFramesThisSec);
		}
		m_NrFramesThisSec = 1;
		m_EndCurrentSecond = now + Time::SEC_DURATION;
	}
	else
		m_NrFramesThisSec++;

	m_DurationLastFrame = (now - m_BeginPrevFrame).count() * Time::CLOCK_UNIT_TO_SEC;
	Globals::DeltaTime = m_DurationLastFrame > .1f ? .1f : m_DurationLastFrame;
	m_BeginPrevFrame = now;
	m_BeginPrevUpdate = now;
}

void App::FpsControl::SetFpsDisplay(Rendering::FpsDisplay& display)
{
	m_pDisplay = &display;
}
