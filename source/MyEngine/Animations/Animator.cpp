#include "pch.h"
#include "Animator.h"

using namespace MyEngine::Animations;

Animator::Animator(const Animation& animation)
	: m_Time{ 0 }
	, m_TimeScale{ 1.f / animation.GetDuration() }
	, m_Cache{ animation.MakeCachedData() }
{
	SetTime(animation, 0);
}

void Animator::ProgressTime(const Animation& animation)
{
	ProgressTime(animation, DELTA_TIME);
}

void Animator::ProgressTime(const Animation& animation, float time)
{
	m_Time += time * m_TimeScale;
	while (m_Time >= 1.f)
		m_Time -= 1.f;

	animation.UpdateModelBuffer(m_Time, m_Buffer, m_Cache);
}

void Animator::ProgressTimeUntil(const Animation& animation, float limit)
{
	ProgressTimeUntil(animation, limit, DELTA_TIME);
}

void Animator::ProgressTimeUntil(const Animation& animation, float limit, float amount)
{
	while (amount >= 1.f)
		amount -= 1.f;

	if (limit == 0 || limit == 1)
	{
		if (m_Time == 0)
			return;

		m_Time += amount * m_TimeScale;

		if (m_Time >= 1.f)
			m_Time = 0;
	}
	else
	{
		if (m_Time == limit)
			return;

		if (m_Time < limit)
		{
			m_Time += amount * m_TimeScale;
			if (m_Time > limit)
				m_Time = limit;
		}
		else
		{
			m_Time += amount * m_TimeScale;
			if (m_Time >= 1.f)
			{
				m_Time -= 1.f;
				if (m_Time > limit)
					m_Time = limit;
			}
		}
	}

	animation.UpdateModelBuffer(m_Time, m_Buffer, m_Cache);
}

void Animator::SetTime(const Animation& animation, float time)
{
	SetNormalizedTime(animation, time * m_TimeScale);
}

void Animator::SetNormalizedTime(const Animation& animation, float time)
{
	animation.UpdateModelBuffer(time, m_Buffer, m_Cache);
}

void Animator::SetDuration(float duration)
{
	m_TimeScale = 1.f / duration;
}

void Animator::SetTimeScale(float timeScale)
{
	m_TimeScale = timeScale;
}
