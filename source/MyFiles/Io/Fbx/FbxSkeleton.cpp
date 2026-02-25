#include "FbxSkeleton.h"

#include "FbxLoadData.h"
#include "Io/Fbx/Wrapping/FbxData.h"

using namespace MyEngine::Io::Fbx;

FbxSkeleton::FbxSkeleton(FbxLoadData& loadData)
{
	const Wrapping::Model* pRoot{ loadData.pFbxData->FindRootLimbNode() };
	if (!pRoot)
		return;

	CreateJoints(*pRoot, loadData);

	SetParentChildRelations(*pRoot, loadData);
	GetRootJoint().CalculateBindTransforms();
}

unsigned FbxSkeleton::GetNrJoints() const
{
	return m_Joints.GetSize();
}

bool FbxSkeleton::FindJointIdx(unsigned& idx, const std::string& name) const
{
	for (unsigned i{ 0 }; i < m_Joints.GetSize(); ++i)
		if (m_Joints[i].GetName() == name)
		{
			idx = i;
			return true;
		}
	return false;
}

void FbxSkeleton::PrintLocalJointData() const
{
	GetRootJoint().PrintLocalData();
}

void FbxSkeleton::CreateJoints(const Wrapping::Model& model, FbxLoadData& loadData)
{
	const unsigned id{ m_Joints.GetSize() };
	m_Joints.Add(FbxJoint{ model, loadData, id });
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
