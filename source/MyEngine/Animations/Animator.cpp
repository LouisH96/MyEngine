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

void Animator::SetTime(const Animation& animation, float time)
{
	SetNormalizedTime(animation, time * m_TimeScale);
}

void Animator::SetNormalizedTime(const Animation& animation, float time)
{
	animation.UpdateModelBuffer(time, m_Buffer, m_Cache);
}
