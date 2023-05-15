#include "pch.h"
#include "FbxSkeleton.h"

#include "Wrapping/FbxData.h"

Io::Fbx::FbxSkeleton::FbxSkeleton(const Wrapping::FbxData& fbxData)
	: m_RootJoint{ *fbxData.GetRootLimbNode(), fbxData }
{
}

void Io::Fbx::FbxSkeleton::AddToDebugRenderer(float sphereSize) const
{
	m_RootJoint.AddToDebugRender(sphereSize);
}
