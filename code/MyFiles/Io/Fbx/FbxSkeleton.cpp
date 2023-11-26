#include "FbxSkeleton.h"

#include "Io/Fbx/Wrapping/FbxData.h"

using namespace MyEngine::Io::Fbx;

FbxSkeleton::FbxSkeleton(const Wrapping::FbxData& fbxData, const FbxClass& fbxClass,
	Dictionary<uint64_t, unsigned>& modelToJoint)
{
	List<unsigned> rootJointIds{};

	const Wrapping::Model* pRoot{ fbxData.GetARootLimbNode() };
	if (pRoot->GetParentModel())
	{
		pRoot = pRoot->GetParentModel();
		m_RootJoints = { pRoot->GetChildModels().GetSize() };
		for (unsigned i = 0; i < m_RootJoints.GetSize(); i++)
		{
			rootJointIds.Add(m_Joints.GetSize());
			CreateJoints(*pRoot->GetChildModels()[i], fbxClass, modelToJoint);
		}

		for (unsigned i = 0; i < m_RootJoints.GetSize(); i++)
			SetParentChildRelations(*pRoot->GetChildModels()[i], modelToJoint);
	}
	else
	{
		m_RootJoints = { 1 };
		rootJointIds.Add(0);
		CreateJoints(*pRoot, fbxClass, modelToJoint);
		SetParentChildRelations(*pRoot, modelToJoint);
	}

	for (unsigned i = 0; i < rootJointIds.GetSize(); i++)
		m_RootJoints[i] = &m_Joints[rootJointIds[i]];
}

unsigned FbxSkeleton::GetNrJoints() const
{
	return m_Joints.GetSize();
}

void FbxSkeleton::CreateJoints(const Wrapping::Model& model, const FbxClass& fbxClass, Dictionary<uint64_t, unsigned>& modelToJoint)
{
	m_Joints.Add(FbxJoint{ model, fbxClass });
	modelToJoint.Add(model.GetId(), m_Joints.GetSize() - 1);

	for (unsigned i = 0; i < model.GetChildModels().GetSize(); i++)
		CreateJoints(*model.GetChildModels()[i], fbxClass, modelToJoint);
}

void FbxSkeleton::SetParentChildRelations(const Wrapping::Model& parent, const Dictionary<uint64_t, unsigned>& modelToJoint)
{
	FbxJoint& parentJoint{ m_Joints[*modelToJoint.Get(parent.GetId())] };

	Array<FbxJoint*> children{ parent.GetChildModels().GetSize() };
	for (unsigned i = 0; i < parent.GetChildModels().GetSize(); i++)
	{
		const Wrapping::Model& childModel{ *parent.GetChildModels()[i] };
		FbxJoint& childJoint{ m_Joints[*modelToJoint.Get(childModel.GetId())] };

		childJoint.SetParent(&parentJoint);
		children[i] = &childJoint;
	}
	parentJoint.SetChildren(std::move(children));

	//recursive
	for (unsigned i = 0; i < parent.GetChildModels().GetSize(); i++)
		SetParentChildRelations(*parent.GetChildModels()[i], modelToJoint);
}
