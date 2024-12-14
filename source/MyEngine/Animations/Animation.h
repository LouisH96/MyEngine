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
	struct ModelBuffer
	{
		static constexpr unsigned MAX_BONES = 100;
		Float4X4 BoneTransforms[MAX_BONES];
		Float4X4 World;
	};

	Animation() = default;
	Animation(const Io::Fbx::FbxClass& fbx, const Io::Fbx::FbxAnimation& animation);

	void UpdateModelBuffer(float time);
	const ModelBuffer& GetModelBuffer() const { return m_ModelBuffer; }

private:
	JointsTimeValues m_TimeValues;
	SkeletonData m_Skeleton;
	CachedData m_Cache;

	ModelBuffer m_ModelBuffer;

	void UpdateTransforms(float time, unsigned iJoint, const Float4X4& parent);
};
}
}