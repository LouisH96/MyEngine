#include "pch.h"
#include "Animation.h"

#include <Transform\WorldMatrix.h>

using namespace MyEngine::Animations;
using namespace Io::Fbx;

Animation::Animation(const FbxClass& fbx, const FbxAnimation& animation)
	: m_Duration{ animation.GetDuration() }
{
	const Io::Fbx::FbxAnimationLayer& layer{ animation.GetLayers().First() };
	const List<FbxJoint>& joints{ fbx.GetSkeleton().GetJoints() };

	m_TimeValues = { joints, animation, layer };
	m_Skeleton = { fbx.GetSkeleton() };
}

void Animation::UpdateModelBuffer(float time, BonesBuffer& buffer) const
{
	const unsigned* pRoot{};
	const unsigned* pRootEnd{ m_Skeleton.GetRootJointsIt(pRoot) };
	while (pRoot != pRootEnd)
		UpdateTransforms(time, *pRoot++, WorldMatrix::GetIdentity(), buffer);
}

void Animation::UpdateModelBuffer(float time, BonesBuffer& buffer, CachedData& cache) const
{
	const unsigned* pRoot{};
	const unsigned* pRootEnd{ m_Skeleton.GetRootJointsIt(pRoot) };
	while (pRoot != pRootEnd)
		UpdateTransforms(time, *pRoot++, WorldMatrix::GetIdentity(), cache, buffer);
}

CachedData Animation::MakeCachedData() const
{
	return CachedData{ m_TimeValues };
}

void Animation::UpdateTransforms(float time, unsigned iJoint, const Float4X4& parent, BonesBuffer& buffer) const
{
	const Float3 position{ m_TimeValues.GetPosition(iJoint, time) };
	const Quaternion rotation{ m_TimeValues.GetQuaternion(iJoint, time) };
	const Float4X4 world{ WorldMatrix::FromPosAndQuat(position, rotation) * parent };

	const SkeletonData::JointData& jointData{ m_Skeleton.GetJointData(iJoint) };
	buffer.BoneTransforms[iJoint] =
		jointData.BindTransform * world;

	const unsigned* pChild{};
	const unsigned* pChildEnd{ m_Skeleton.GetChildrenIt(pChild, iJoint) };
	while (pChild != pChildEnd)
		UpdateTransforms(time, *pChild++, world, buffer);
}

void Animation::UpdateTransforms(float time, unsigned iJoint, const Float4X4& parent, CachedData& cache, BonesBuffer& buffer) const
{
	const JointCacheData& jointCache{ cache.Get(m_TimeValues, iJoint, time) };

	const Float3 position{ jointCache.GetPosition(time) };
	const Quaternion rotation{ jointCache.GetRotation(time) };
	const Float4X4 world{ WorldMatrix::FromPosAndQuat(position, rotation) * parent };

	const SkeletonData::JointData& jointData{ m_Skeleton.GetJointData(iJoint) };
	buffer.BoneTransforms[iJoint] =
		jointData.BindTransform * world;

	const unsigned* pChild{};
	const unsigned* pChildEnd{ m_Skeleton.GetChildrenIt(pChild, iJoint) };
	while (pChild != pChildEnd)
		UpdateTransforms(time, *pChild++, world, cache, buffer);
}
