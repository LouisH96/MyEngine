#pragma once
#include "Animation.h"
#include "CachedData.h"

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

	const Animation::BonesBuffer& GetBonesBuffer() const { return m_Buffer; }

private:
	Animation::BonesBuffer m_Buffer;
	CachedData m_Cache;
	float m_Time{ 0 };
	float m_TimeScale{ 1 };
};
}
}