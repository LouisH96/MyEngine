#include "FbxSkeleton.h"

#include "FbxLoadData.h"
#include "Io/Fbx/Wrapping/FbxData.h"

using namespace MyEngine::Io::Fbx;

FbxSkeleton::FbxSkeleton(FbxLoadData& loadData)
{
	List<unsigned> rootJointIds{};

	const Wrapping::Model* pRoot{ loadData.pFbxData->GetARootLimbNode() };
	if (pRoot->GetParentModel())
	{
		pRoot = pRoot->GetParentModel();
		m_RootJoints = { pRoot->GetChildModels().GetSize() };
		for (unsigned i = 0; i < m_RootJoints.GetSize(); i++)
		{
			rootJointIds.Add(m_Joints.GetSize());
			CreateJoints(*pRoot->GetChildModels()[i], loadData);
		}

		for (unsigned i = 0; i < m_RootJoints.GetSize(); i++)
			SetParentChildRelations(*pRoot->GetChildModels()[i], loadData);
	}
	else
	{
		m_RootJoints = { 1 };
		rootJointIds.Add(0);
		CreateJoints(*pRoot, loadData);
		SetParentChildRelations(*pRoot, loadData);
	}

	for (unsigned i = 0; i < rootJointIds.GetSize(); i++)
	{
		m_RootJoints[i] = &m_Joints[rootJointIds[i]];
		m_RootJoints[i]->CalculateBindTransforms();
	}
}

unsigned FbxSkeleton::GetNrJoints() const
{
	return m_Joints.GetSize();
}

void FbxSkeleton::PrintLocalJointData() const
{
	for (unsigned i = 0; i < m_RootJoints.GetSize(); i++)
		m_RootJoints[i]->PrintLocalData();
}

void FbxSkeleton::CreateJoints(const Wrapping::Model& model, FbxLoadData& loadData)
{
	m_Joints.Add(FbxJoint{ model, loadData });
	loadData.ModelToJoint.Add(model.GetId(), m_Joints.GetSize() - 1);

	for (unsigned i = 0; i < model.GetChildModels().GetSize(); i++)
		CreateJoints(*model.GetChildModels()[i], loadData);
}

void FbxSkeleton::SetParentChildRelations(const Wrapping::Model& parent, FbxLoadData& loadData)
{
	FbxJoint& parentJoint{ m_Joints[*loadData.ModelToJoint.Get(parent.GetId())] };

	Array<FbxJoint*> children{ parent.GetChildModels().GetSize() };
	for (unsigned i = 0; i < parent.GetChildModels().GetSize(); i++)
	{
		const Wrapping::Model& childModel{ *parent.GetChildModels()[i] };
		FbxJoint& childJoint{ m_Joints[*loadData.ModelToJoint.Get(childModel.GetId())] };

		childJoint.SetParent(&parentJoint);
		children[i] = &childJoint;
	}
	parentJoint.SetChildren(std::move(children));

	//recursive
	for (unsigned i = 0; i < parent.GetChildModels().GetSize(); i++)
		SetParentChildRelations(*parent.GetChildModels()[i], loadData);
}
