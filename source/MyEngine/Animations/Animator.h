#pragma once

#include "Animation.h"
#include "CachedData.h"
#include "BonesBuffer.h"

namespace MyEngine
{
namespace Animations
{
class Animator
{
public:
	Animator() = default;
	Animator(const Animation& animation);

	void ProgressTime(const Animation& animation);
	void ProgressTime(const Animation& animation, float time);

	void SetTime(const Animation& animation, float time);
	void SetNormalizedTime(const Animation& animation, float time);

	const BonesBuffer& GetBonesBuffer() const { return m_Buffer; }

private:
	BonesBuffer m_Buffer;
	CachedData m_Cache;
	float m_Time;
	float m_TimeScale;
};
}
}