#include "pch.h"
#include "FbxSkeleton.h"

#include "Io/Fbx/Wrapping/FbxData.h"

Io::Fbx::FbxSkeleton::FbxSkeleton(const Wrapping::FbxData& fbxData, const FbxClass& fbxClass)
	: m_RootJoint{ *fbxData.GetRootLimbNode(), fbxData, fbxClass }
{
}

void Io::Fbx::FbxSkeleton::AddToDebugRenderer(const FbxAnimationLayer& layer, float scale, float sphereSize) const
{
	m_RootJoint.AddToDebugRender(layer, scale, sphereSize);
}

void Io::Fbx::FbxSkeleton::AddToDebugRenderer(const FbxAnimationLayer& layer, const int64_t& time, float scale, float sphereSize) const
{
	m_RootJoint.AddToDebugRender(layer, time, scale, sphereSize);
}
