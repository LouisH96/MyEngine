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
}

void Animation::UpdateModelBuffer(float time)
{
	const unsigned* pRoot{};
	const unsigned* pRootEnd{ m_Skeleton.GetRootJointsIt(pRoot) };
	while (pRoot != pRootEnd)
		UpdateTransforms(time, *pRoot++, Transform{});
}

void Animation::UpdateTransforms(float time, unsigned iJoint, const Transform& parent)
{
	const Float3 position{ m_TimeValues.GetPosition(iJoint, time) };
	const Quaternion rotation{ m_TimeValues.GetQuaternion(iJoint, time) };

	const SkeletonData::JointData& jointData{ m_Skeleton.GetJointData(iJoint) };

	const Transform world{ Transform::LocalToWorld({position, rotation}, {parent}) };

	m_ModelBuffer.BoneTransforms[iJoint] =
		jointData.BindTransform * world.AsMatrix();

	const unsigned* pChild{};
	const unsigned* pChildEnd{ m_Skeleton.GetChildrenIt(pChild, iJoint) };
	while (pChild != pChildEnd)
		UpdateTransforms(time, *pChild++, world);
}

