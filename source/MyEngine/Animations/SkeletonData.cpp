#include "pch.h"
#include "SkeletonData.h"

#include <Io\Fbx\FbxSkeleton.h>

using namespace MyEngine::Animations;
using namespace Io::Fbx;

SkeletonData::SkeletonData(const FbxSkeleton& skeleton)
{
	CreateMemory(skeleton);
	FillMemory(skeleton);
}

const unsigned* SkeletonData::GetRootJointsIt(const unsigned*& pFirst) const
{
	pFirst = m_BoneRelations.GetData();
	return m_pLookup;
}

const unsigned* SkeletonData::GetChildrenIt(const unsigned*& pFirst, unsigned iJoint) const
{
	pFirst = &m_BoneRelations[m_pLookup[iJoint]];
	return &m_BoneRelations[m_pLookup[iJoint + 1]];
}

unsigned SkeletonData::FindParent(unsigned iJoint) const
{
	const unsigned* const pChildrenBegin{ &m_pLookup[GetNrJoints() + 1] };
	const unsigned* const pChildrenEnd{ &m_BoneRelations[m_BoneRelations.GetSize()] };

	const unsigned* pChildrenIt{ pChildrenBegin };
	while (pChildrenIt < pChildrenEnd)
	{
		if (*pChildrenIt == iJoint)
		{
			const unsigned index{ static_cast<unsigned>(pChildrenIt - m_BoneRelations.GetData()) };

			const unsigned* const pLookupBegin{ m_pLookup };
			const unsigned* const pLookupEnd{ pChildrenBegin };

			const unsigned* pLookupIt{ pLookupBegin };
			const unsigned* pLookupItNext{ pLookupIt + 1 };

			while (pLookupIt != pLookupEnd)
			{
				if (*pLookupIt <= index && *pLookupItNext > index)
					return static_cast<unsigned>(pLookupIt - m_pLookup);
				++pLookupIt;
				++pLookupItNext;
			}
		}
		++pChildrenIt;
	}
	return Uint::MAX;
}

void SkeletonData::CreateMemory(const FbxSkeleton& skeleton)
{
	const List<FbxJoint>& joints{ skeleton.GetJoints() };

	//Bone-Relations
	unsigned size{ skeleton.GetRootJoints().GetSize() };
	size += joints.GetSize() + 1;

	for (unsigned iJoint{ 0 }; iJoint < joints.GetSize(); ++iJoint)
		size += joints[iJoint].GetChildren().GetSize();
	m_BoneRelations = { size };
	m_pLookup = &m_BoneRelations[skeleton.GetRootJoints().GetSize()];

	//Joint-Data
	m_JointData = { joints.GetSize() };
}

void SkeletonData::FillMemory(const FbxSkeleton& skeleton)
{
	const Array<FbxJoint*>& rootJoints{ skeleton.GetRootJoints() };
	const List<FbxJoint>& joints{ skeleton.GetJoints() };
	unsigned* pLookup{ m_BoneRelations.GetData() };

	//Bone-Relations
	for (unsigned iRootJoint{ 0 }; iRootJoint < rootJoints.GetSize(); ++iRootJoint)
		*pLookup++ = rootJoints[iRootJoint]->GetId();

	unsigned* pChild{ pLookup + joints.GetSize() + 1 };
	for (unsigned iJoint{ 0 }; iJoint < joints.GetSize(); ++iJoint)
	{
		*pLookup++ = static_cast<unsigned>(pChild - m_BoneRelations.GetData());

		const FbxJoint& joint{ joints[iJoint] };
		const Array<FbxJoint*>& children{ joint.GetChildren() };
		for (unsigned iChild{ 0 }; iChild < children.GetSize(); ++iChild)
			*pChild++ = children[iChild]->GetId();
	}
	*pLookup = m_BoneRelations.GetSize();

	//Joint-Data
	for (unsigned iJoint{ 0 }; iJoint < joints.GetSize(); ++iJoint)
	{
		const FbxJoint& joint{ joints[iJoint] };
		JointData& jointData{ m_JointData[iJoint] };
		jointData.BindTransform = joint.GetBindTransform();
	}
}

unsigned SkeletonData::GetNrJoints() const
{
	return m_JointData.GetSize();
}
