#include "pch.h"
#include "FbxSkeleton.h"

#include "Io/Fbx/Wrapping/FbxData.h"

Io::Fbx::FbxSkeleton::FbxSkeleton(const Wrapping::FbxData& fbxData)
	: m_RootJoint{ *fbxData.GetRootLimbNode(), fbxData }
{
}

void Io::Fbx::FbxSkeleton::AddToDebugRenderer(float sphereSize) const
{
	m_RootJoint.AddToDebugRender(sphereSize);
}

void Io::Fbx::FbxSkeleton::AddToDebugRenderer(const int64_t& time, float sphereSize) const
{
	m_RootJoint.AddToDebugRender(time, sphereSize);
}
