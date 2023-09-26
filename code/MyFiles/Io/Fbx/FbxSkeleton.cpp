#include "FbxSkeleton.h"

#include "Io/Fbx/Wrapping/FbxData.h"

using namespace MyEngine::Io::Fbx;

FbxSkeleton::FbxSkeleton(const Wrapping::FbxData& fbxData, const FbxClass& fbxClass)
{
	const Wrapping::Model* pRoot{fbxData.GetARootLimbNode()};
	if (pRoot->GetParentModel())
	{
		pRoot = pRoot->GetParentModel();
		m_RootJoints = { pRoot->GetChildModels().GetSize() };
		for (unsigned i = 0; i < m_RootJoints.GetSize(); i++)
			m_RootJoints[i] = FbxJoint{ *pRoot->GetChildModels()[i], fbxData, fbxClass };
	}
	else
	{
		m_RootJoints = { 1 };
		m_RootJoints[0] = FbxJoint{ *pRoot, fbxData, fbxClass };
	}
}
