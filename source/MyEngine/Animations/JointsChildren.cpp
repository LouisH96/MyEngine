#include "pch.h"
#include "JointsChildren.h"

using namespace Animations;
using namespace Io::Fbx;

JointsChildren::JointsChildren(const List<FbxJoint>& joints)
{
	const unsigned nrJoints{ joints.GetSize() };

	//
	unsigned totalNrChildren{ 0 };
	for (unsigned iJoint{ 0 }; iJoint < nrJoints; ++iJoint)
		totalNrChildren += joints[iJoint].GetChildren().GetSize();

	//
	m_Data = Array<unsigned>{ joints.GetSize() + totalNrChildren + 1 }; //+1 for end
	m_Data[nrJoints] = m_Data.GetSize(); //end

	//lookup-part
	m_Data[0] = nrJoints + 1;
	unsigned* pData{ &m_Data[1] };
	const FbxJoint* pJoint{ &joints[0] };
	for (unsigned iJoint{ 1 }; iJoint < nrJoints; ++iJoint)
	{
		*pData = *(pData - 1) + pJoint++->GetChildren().GetSize();
		++pData;
	}

	//data-part
	pData = &m_Data[m_Data[0]];
	pJoint = joints.GetData();
	for (unsigned iJoint{ 0 }; iJoint < nrJoints; ++iJoint)
	{
		const Array<FbxJoint*>& childs{ pJoint++->GetChildren() };
		for (unsigned iChild{ 0 }; iChild < childs.GetSize(); ++iChild)
			*pData++ = childs[iChild]->GetId();
	}
}

const unsigned* JointsChildren::GetIterator(unsigned iJoint, const unsigned*& iBegin)
{
	iBegin = &m_Data[m_Data[iJoint]];
	return &m_Data[m_Data[iJoint + 1]];
}

void JointsChildren::Print(unsigned iJoint) const
{
	std::cout << "Joint(" << iJoint << "): ";

	const unsigned* iChild{ &m_Data[m_Data[iJoint]] };
	const unsigned* iChildEnd{ &m_Data[m_Data[iJoint + 1]] };

	if (iChild != iChildEnd)
		std::cout << *iChild++;

	while (iChild != iChildEnd)
		std::cout << ", " << *iChild++;

	std::cout << std::endl;
}
