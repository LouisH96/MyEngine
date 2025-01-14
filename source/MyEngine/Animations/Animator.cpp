#include "pch.h"
#include "Animator.h"

#include <Transform\WorldMatrix.h>

using namespace MyEngine::Animations;

Animator::Animator(const Animation& animation)
	: m_Bones{ animation.GetNrBones() }
	, m_Time{ 0 }
	, m_TimeScale{ 1.f / animation.GetDuration() }
	, m_Cache{ animation.MakeCachedData() }
{
	SetTime(animation, 0);
}

unsigned Animator::ProgressTime(const Animation& animation)
{
	return ProgressTime(animation, DELTA_TIME);
}

unsigned Animator::ProgressTime(const Animation& animation, float time)
{
	unsigned nrLoops{ 0 };
	m_Time += time * m_TimeScale;
	while (m_Time >= 1.f)
	{
		m_Time -= 1.f;
		nrLoops++;
	}
	animation.UpdateBonesBuffer(m_Time, m_Bones, m_Cache);
	return nrLoops;
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
	animation.UpdateBonesBuffer(m_Time, m_Bones, m_Cache);
}

void Animator::SetTime(const Animation& animation, float time)
{
	SetNormalizedTime(animation, time * m_TimeScale);
}

void Animator::SetNormalizedTime(const Animation& animation, float time)
{
	animation.UpdateBonesBuffer(time, m_Bones, m_Cache);
}

void Animator::SetDuration(float duration)
{
	m_TimeScale = 1.f / duration;
}

void Animator::SetTimeScale(float timeScale)
{
	m_TimeScale = timeScale;
}

Float2 Animator::GetRootXz() const
{
	return WorldMatrix::GetPositionXz(m_Bones.First());
}
