#pragma once
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
	Animation(const Io::Fbx::FbxClass& fbx, const Io::Fbx::FbxAnimation& animation, uint64_t start, uint64_t end);

	void UpdateBonesBuffer(float time, Array<Float4X4>& bones) const;
	void UpdateBonesBuffer(float time, Array<Float4X4>& bones, CachedData& cache) const;

	float GetDuration() const { return m_Duration; }

	CachedData MakeCachedData() const;

	Float4X4 GetModelMatrix(unsigned iJoint, float time) const;
	Float3 GetModelPosition(unsigned iJoint, float time) const;

	Float2 GetFullRootMotionXz() const;

	const SkeletonData& GetSkeleton() const { return m_Skeleton; }
	unsigned GetNrBones() const { return m_Skeleton.GetNrBones(); }

private:
	JointsTimeValues m_TimeValues;
	SkeletonData m_Skeleton;
	float m_Duration{};

	void UpdateTransforms(
		float time, unsigned iJoint,
		const Float4X4& parent, Array<Float4X4>& bones) const;

	void UpdateTransforms(
		float time, unsigned iJoint,
		const Float4X4& parent, CachedData& cache,
		Array<Float4X4>& bones) const;
};
}
}