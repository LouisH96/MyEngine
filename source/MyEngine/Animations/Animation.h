#pragma once
#include "BonesBuffer.h"
#include "JointsTimeValues.h"
#include "JointsChildren.h"
#include "SkeletonData.h"
#include "CachedData.h"

namespace MyEngine
{
namespace Animations
{
class Animation
{
public:
	Animation() = default;
	Animation(const Io::Fbx::FbxClass& fbx, const Io::Fbx::FbxAnimation& animation);

	void UpdateModelBuffer(float time, BonesBuffer& buffer) const;
	void UpdateModelBuffer(float time, BonesBuffer& buffer, CachedData& cache) const;

	float GetDuration() const { return m_Duration; }

	CachedData MakeCachedData() const;

private:
	JointsTimeValues m_TimeValues;
	SkeletonData m_Skeleton;
	float m_Duration;

	void UpdateTransforms(
		float time, unsigned iJoint,
		const Float4X4& parent, BonesBuffer& buffer) const;

	void UpdateTransforms(
		float time, unsigned iJoint,
		const Float4X4& parent, CachedData& cache,
		BonesBuffer& buffer) const;
};
}
}