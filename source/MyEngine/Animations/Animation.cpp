#include "pch.h"
#include "Animation.h"

#include <Transform\WorldMatrix.h>

using namespace MyEngine::Animations;
using namespace Io::Fbx;

Animation::Animation(const FbxClass& fbx, const Io::Fbx::FbxAnimation& animation)
{
	m_ModelBuffer.World = WorldMatrix::GetIdentity();

	const Io::Fbx::FbxAnimationLayer& layer{ animation.GetLayers().First() };
	const List<FbxJoint>& joints{ fbx.GetSkeleton().GetJoints() };

	m_TimeValues = { joints, animation, layer };
	m_ChildrenLookup = { joints };

	//Pre & Post Rotations
	m_PreAndPostRotations = { joints.GetSize() * 2 };
	Quaternion* pQuaternion{ m_PreAndPostRotations.GetData() };
	for (unsigned iJoint{ 0 }; iJoint < joints.GetSize(); iJoint++)
	{
		const FbxJoint& joint{ joints[iJoint] };
		*pQuaternion++ = joint.GetPreRotationZYX().Rotation;
		*pQuaternion++ = joint.GetPostRotationXYZ().Rotation;
	}

	//BindTransforms
	m_BindTransforms = Array<Float4X4>{ joints.GetSize() };
	for (unsigned iJoint{ 0 }; iJoint < joints.GetSize(); ++iJoint)
		m_BindTransforms[iJoint] = joints[iJoint].GetBindTransform();

	//RootJoints
	const Array<FbxJoint*>& rootJoints{ fbx.GetSkeleton().GetRootJoints() };
	m_RootJoints = Array<unsigned>{ rootJoints.GetSize() };
	for (unsigned iJoint{ 0 }; iJoint < m_RootJoints.GetSize(); ++iJoint)
		m_RootJoints[iJoint] = rootJoints[iJoint]->GetId();
}

void Animation::UpdateModelBuffer(float time)
{
	for (unsigned iJoint{ 0 }; iJoint < m_RootJoints.GetSize(); ++iJoint)
		UpdateTransforms(time, m_RootJoints[iJoint], Transform{});
}

void Animation::UpdateTransforms(float time, unsigned iJoint, const Transform& parent)
{
	const Float3 position{ m_TimeValues.GetPosition(iJoint, time) };
	const Float3 rotation{ m_TimeValues.GetEulerDegrees(iJoint, time) };

	const Quaternion& preRotation{ m_PreAndPostRotations[iJoint * 2] };
	const Quaternion& postRotation{ m_PreAndPostRotations[iJoint * 2 + 1] };

	Transform world{ parent };
	world = Transform::LocalToWorld({ position, {} }, world);
	world = Transform::LocalToWorld({ {}, preRotation }, world);
	world = Transform::LocalToWorld({ {}, Quaternion::FromEulerDegrees(rotation) }, world);
	world = Transform::LocalToWorld({ {}, postRotation }, world);

	m_ModelBuffer.BoneTransforms[iJoint] =
		m_BindTransforms[iJoint] * world.AsMatrix();

	const unsigned* pChild{};
	const unsigned* pChildEnd{ m_ChildrenLookup.GetIterator(iJoint, pChild) };
	while (pChild != pChildEnd)
		UpdateTransforms(time, *pChild++, world);
}

