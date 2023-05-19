#include "pch.h"
#include "Skeleton.h"

#include "Io/Fbx/Wrapping/FbxData.h"

Animation::Skeleton::Skeleton(const Io::Fbx::Wrapping::FbxData& fbxData)
	: m_RootJoint{ *fbxData.GetRootLimbNode(), fbxData }
{
}

void Animation::Skeleton::AddToDebugRenderer(float sphereSize) const
{
	m_RootJoint.AddToDebugRender(sphereSize);
}

void Animation::Skeleton::AddToDebugRenderer(const int64_t& time, float sphereSize) const
{
	m_RootJoint.AddToDebugRender(time, sphereSize);
}
