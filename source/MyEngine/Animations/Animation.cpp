#include "pch.h"
#include "Animation.h"

#include <Transform\WorldMatrix.h>

using namespace MyEngine::Animations;
using namespace Io::Fbx;

Animation::Animation(const FbxClass& fbx, const FbxAnimation& animation)
{
	m_ModelBuffer.World = WorldMatrix::GetIdentity();

	const Io::Fbx::FbxAnimationLayer& layer{ animation.GetLayers().First() };
	const List<FbxJoint>& joints{ fbx.GetSkeleton().GetJoints() };

	m_TimeValues = { joints, animation, layer };
	m_Skeleton = { fbx.GetSkeleton() };
	m_Cache = { m_TimeValues };
}

void Animation::UpdateModelBuffer(float time)
{
	const unsigned* pRoot{};
	const unsigned* pRootEnd{ m_Skeleton.GetRootJointsIt(pRoot) };
	while (pRoot != pRootEnd)
		UpdateTransforms(time, *pRoot++, WorldMatrix::GetIdentity());
}

void Animation::UpdateTransforms(float time, unsigned iJoint, const Float4X4& parent)
{
	const JointCacheData& cache{ m_Cache.Get(m_TimeValues, iJoint, time) };

	const Float3 position{ cache.GetPosition(time)};
	const Quaternion rotation{ cache.GetRotation(time)};
	const Float4X4 world{ WorldMatrix::FromPosAndQuat(position, rotation) * parent };

	const SkeletonData::JointData& jointData{ m_Skeleton.GetJointData(iJoint) };
	m_ModelBuffer.BoneTransforms[iJoint] =
		jointData.BindTransform * world;

	const unsigned* pChild{};
	const unsigned* pChildEnd{ m_Skeleton.GetChildrenIt(pChild, iJoint) };
	while (pChild != pChildEnd)
		UpdateTransforms(time, *pChild++, world);
}

