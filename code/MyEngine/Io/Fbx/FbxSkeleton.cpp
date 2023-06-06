#include "pch.h"
#include "FbxSkeleton.h"

#include "Io/Fbx/Wrapping/FbxData.h"

Io::Fbx::FbxSkeleton::FbxSkeleton(const Wrapping::FbxData& fbxData, const FbxClass& fbxClass)
{
	const Wrapping::Model* pRoot{fbxData.GetARootLimbNode()};
	if (pRoot->GetParentModel())
	{
		pRoot = pRoot->GetParentModel();
		m_RootJoints = { pRoot->GetChildModels().GetSize() };
		for (int i = 0; i < m_RootJoints.GetSize(); i++)
			m_RootJoints[i] = FbxJoint{ *pRoot->GetChildModels()[i], fbxData, fbxClass };
	}
	else
	{
		m_RootJoints = { 1 };
		m_RootJoints[0] = FbxJoint{ *pRoot, fbxData, fbxClass };
	}
}

void Io::Fbx::FbxSkeleton::AddToDebugRenderer(const FbxAnimationLayer& layer, float scale, float sphereSize) const
{
	for (int i = 0; i < m_RootJoints.GetSize(); i++)
		m_RootJoints[i].AddToDebugRender(layer, scale, sphereSize);
}

void Io::Fbx::FbxSkeleton::AddToDebugRenderer(const FbxAnimationLayer& layer, const int64_t& time, float scale, float sphereSize) const
{
	for (int i = 0; i < m_RootJoints.GetSize(); i++)
		m_RootJoints[i].AddToDebugRender(layer, time, scale, sphereSize);
}
