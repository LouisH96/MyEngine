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
	static constexpr unsigned MAX_BONES = 100;
	using BonesBuffer = Rendering::CB_BonesBuffer;

	Animation() = default;
	Animation(const Io::Fbx::FbxClass& fbx, const Io::Fbx::FbxAnimation& animation);
	Animation(const Io::Fbx::FbxClass& fbx, const Io::Fbx::FbxAnimation& animation, uint64_t start, uint64_t end);

	void UpdateModelBuffer(float time, BonesBuffer& buffer) const;
	void UpdateModelBuffer(float time, BonesBuffer& buffer, CachedData& cache) const;

	float GetDuration() const { return m_Duration; }

	CachedData MakeCachedData() const;

	Float4X4 GetModelMatrix(unsigned iJoint, float time) const;
	Float3 GetModelPosition(unsigned iJoint, float time) const;

	const SkeletonData& GetSkeleton() const { return m_Skeleton; }

private:
	JointsTimeValues m_TimeValues;
	SkeletonData m_Skeleton;
	float m_Duration{};

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