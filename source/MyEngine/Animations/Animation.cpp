#include "pch.h"
#include "Animation.h"

#include <Transform\WorldMatrix.h>

using namespace MyEngine::Animations;
using namespace Io::Fbx;

Animation::Animation(const FbxClass& fbx, const FbxAnimation& animation)
	: Animation{ fbx, animation, animation.GetLocalStart(), animation.GetLocalStop() }
{
}

Animation::Animation(
	const Io::Fbx::FbxClass& fbx,
	const Io::Fbx::FbxAnimation& animation,
	uint64_t start, uint64_t end)
	: m_Duration{ FbxAnimation::ToSeconds(end - start) }
{
	const Io::Fbx::FbxAnimationLayer& layer{ animation.GetLayers().First() };
	const List<FbxJoint>& joints{ fbx.GetSkeleton().GetJoints() };

	m_TimeValues = { joints, animation, layer, start, end };
	m_Skeleton = { fbx.GetSkeleton() };
}

void Animation::UpdateBonesBuffer(float time, Array<Float4X4>& bones) const
{
	UpdateTransforms(time, 0, WorldMatrix::GetIdentity(), bones);
}

void Animation::UpdateBonesBuffer(float time, Array<Float4X4>& bones, CachedData& cache) const
{
	UpdateTransforms(time, 0, WorldMatrix::GetIdentity(), cache, bones);
}

CachedData Animation::MakeCachedData() const
{
	return CachedData{ m_TimeValues };
}

Float4X4 Animation::GetModelMatrix(unsigned iJoint, float time) const
{
	Float4X4 matrix{ Float4X4::GetIdentity() };

	while (iJoint != Uint::MAX)
	{
		const Float3 position{ m_TimeValues.GetPosition(iJoint, time) };
		const Quaternion rotation{ m_TimeValues.GetQuaternion(iJoint, time) };
		matrix *= WorldMatrix::FromPosAndQuat(position, rotation);

		iJoint = m_Skeleton.FindParent(iJoint);
	}

	return matrix;
}

Float3 Animation::GetModelPosition(unsigned iJoint, float time) const
{
	return WorldMatrix::GetPosition(GetModelMatrix(iJoint, time));
}

Float2 Animation::GetFullRootMotionXz() const
{
	const Float2 begin{ m_TimeValues.GetPosition(0,0).Xz() };
	const Float2 end{ m_TimeValues.GetPosition(0,1).Xz() };
	return end - begin;
}

void Animation::UpdateTransforms(float time, unsigned iJoint, const Float4X4& parent, Array<Float4X4>& bones) const
{
	const Float3 position{ m_TimeValues.GetPosition(iJoint, time) };
	const Quaternion rotation{ m_TimeValues.GetQuaternion(iJoint, time) };
	const Float4X4 world{ WorldMatrix::FromPosAndQuat(position, rotation) * parent };

	const SkeletonData::JointData& jointData{ m_Skeleton.GetJointData(iJoint) };
	bones[iJoint] =
		jointData.BindTransform * world;

	const unsigned* pChild{};
	const unsigned* pChildEnd{ m_Skeleton.GetChildrenIt(pChild, iJoint) };
	while (pChild != pChildEnd)
		UpdateTransforms(time, *pChild++, world, bones);
}

void Animation::UpdateTransforms(float time, unsigned iJoint, const Float4X4& parent, CachedData& cache, Array<Float4X4>& bones) const
{
	const JointCacheData& jointCache{ cache.Get(m_TimeValues, iJoint, time) };

	const Float3 position{ jointCache.GetPosition(time) };
	const Quaternion rotation{ jointCache.GetRotation(time) };
	const Float4X4 world{ WorldMatrix::FromPosAndQuat(position, rotation) * parent };

	const SkeletonData::JointData& jointData{ m_Skeleton.GetJointData(iJoint) };
	bones[iJoint] =
		jointData.BindTransform * world;

	const unsigned* pChild{};
	const unsigned* pChildEnd{ m_Skeleton.GetChildrenIt(pChild, iJoint) };
	while (pChild != pChildEnd)
		UpdateTransforms(time, *pChild++, world, cache, bones);
}
