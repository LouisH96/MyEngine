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

	void ProgressTimeUntil(const Animation& animation, float limit);
	void ProgressTimeUntil(const Animation& animation, float limit, float amount);

	void SetTime(const Animation& animation, float time);
	void SetNormalizedTime(const Animation& animation, float time);

	const Array<Float4X4>& GetBones() const { return m_Bones; }

	const float GetNormalizedTime() const { return m_Time; }

	void SetDuration(float duration);
	void SetTimeScale(float timeScale);

private:
	Array<Float4X4> m_Bones;
	CachedData m_Cache;
	float m_Time{ 0 };
	float m_TimeScale{ 1 };
};
}
}