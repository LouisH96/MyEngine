#include "FbxJoint.h"

#include "FbxClass.h"
#include "FbxLoadData.h"
#include "Io/Fbx/Wrapping/FbxData.h"
#include "Logger/Logger.h"
#include "Transform/WorldMatrix.h"

using namespace MyEngine;
using namespace Io::Fbx;

FbxJoint::FbxJoint(
	const Wrapping::Model& model,
	const FbxLoadData& loadData)
	: m_Name{ model.GetName() }
	, m_Curves{ loadData.pFbxClass->GetNrOfAnimationLayers() }
{
	//ANIMATION
	for (unsigned iAnimation = 0, iCurve = 0; iAnimation < loadData.pFbxClass->GetAnimations().GetSize(); iAnimation++)
	{
		const FbxAnimation& animation{ loadData.pFbxClass->GetAnimations()[iAnimation] };
		for (unsigned iLayer = 0; iLayer < animation.GetLayers().GetSize(); iLayer++, iCurve++)
			m_Curves[iCurve] = FbxTransformCurve{ model, animation.GetLayers()[iLayer], loadData };
	}

	//POSITION
	m_Translation = loadData.Orientation.ConvertPoint(model.GetLclTranslation());
	m_PreRotationEulers = loadData.Orientation.ConvertRotation(model.GetPreRotation());
	m_PostRotationEulers = loadData.Orientation.ConvertRotation(model.GetPostRotation());
	m_LclRotationEulers = loadData.Orientation.ConvertRotation(model.GetLclRotation());

	m_PreRotationTransform = { {}, Quaternion::FromEulerDegrees({0,0, m_PreRotationEulers.z}) };
	m_PreRotationTransform = Transform::LocalToWorld({ {}, Quaternion::FromEulerDegrees({0, m_PreRotationEulers.y, 0}) }, m_PreRotationTransform);
	m_PreRotationTransform = Transform::LocalToWorld({ {}, Quaternion::FromEulerDegrees({m_PreRotationEulers.x,0,0}) }, m_PreRotationTransform);

	m_PostRotationTransform = { {}, Quaternion::FromEulerDegrees({-m_PostRotationEulers.x,0,0}) };
	m_PostRotationTransform = Transform::LocalToWorld({ {}, Quaternion::FromEulerDegrees({0,-m_PostRotationEulers.y, 0}) }, m_PostRotationTransform);
	m_PostRotationTransform = Transform::LocalToWorld({ {}, Quaternion::FromEulerDegrees({0,0, -m_PostRotationEulers.z}) }, m_PostRotationTransform);

	m_LocalTransform = loadData.Orientation.MakeLocalTransform(model);

	//POSE
	const Wrapping::Pose::Node* pNode{ loadData.pFbxData->GetBindPose().FindNode(model.GetId()) };
	if (pNode)
	{
		const Double4X4& matrix{ pNode->Matrix };
		m_BindTransform = Float4X4{ matrix };
		m_BindTransform.Get(0, 3) *= -loadData.Orientation.GetScale();
		m_BindTransform.Get(1, 3) *= loadData.Orientation.GetScale();
		m_BindTransform.Get(2, 3) *= loadData.Orientation.GetScale();

		m_BindTransform.Get(0, 1) *= -1;
		m_BindTransform.Get(0, 2) *= -1;

		m_BindTransform.Get(1, 0) *= -1;
		m_BindTransform.Get(2, 0) *= -1;

		WorldMatrix::Inverse(m_BindTransform);
	}
	else
		SetInvalidBindTransform();
}

FbxJoint::FbxJoint(FbxJoint&& other) noexcept
	: m_Name{ std::move(other.m_Name) }
	, m_LocalTransform(other.m_LocalTransform)
	, m_PreRotationTransform{ other.m_PreRotationTransform }
	, m_PostRotationTransform{ other.m_PostRotationTransform }
	, m_BindTransform{ other.m_BindTransform }
	, m_Children{ std::move(other.m_Children) }
	, m_pParent{ other.m_pParent }
	, m_Curves{ std::move(other.m_Curves) }
	, m_Translation{ other.m_Translation }
	, m_PreRotation{ other.m_PreRotation }
	, m_PostRotation{ other.m_PostRotation }
	, m_LclRotationEulers{ other.m_LclRotationEulers }
	, m_PreRotationEulers{ other.m_PreRotationEulers }
	, m_PostRotationEulers{ other.m_PostRotationEulers }
{
	for (unsigned i = 0; i < m_Children.GetSize(); i++)
		m_Children[i]->m_pParent = this;
}

FbxJoint& FbxJoint::operator=(FbxJoint&& other) noexcept
{
	m_Name = std::move(other.m_Name);
	m_LocalTransform = other.m_LocalTransform;
	m_BindTransform = other.m_BindTransform;
	m_Children = std::move(other.m_Children);
	m_pParent = other.m_pParent;
	m_Curves = std::move(other.m_Curves);
	m_PreRotation = other.m_PreRotation;
	m_PostRotation = other.m_PostRotation;
	m_Translation = other.m_Translation;
	m_PreRotationEulers = other.m_PreRotationEulers;
	m_PostRotationEulers = other.m_PostRotationEulers;
	m_LclRotationEulers = other.m_LclRotationEulers;
	m_PostRotationTransform = other.m_PostRotationTransform;
	m_PreRotationTransform = other.m_PreRotationTransform;
	for (unsigned i = 0; i < m_Children.GetSize(); i++)
		m_Children[i]->m_pParent = this;
	return *this;
}

const FbxTransformCurve* FbxJoint::FindCurve(const FbxAnimationLayer& layer) const
{
	for (unsigned i = 0; i < m_Curves.GetSize(); i++)
		if (m_Curves[i].IsInLayer(layer))
			return &m_Curves[i];
	return nullptr;
}

void FbxJoint::PrintLocalData() const
{
	std::stringstream ss{};
	ss << m_Name << ":\n";
	if (m_pParent)
		ss << " Parent: " << m_pParent->GetName() << std::endl;
	ss << " Translation: " << Convert::ToString(m_Translation) << std::endl;
	ss << " PreRotation: " << Convert::ToString(m_PreRotationEulers) << std::endl;
	ss << " LclRotation: " << Convert::ToString(m_LclRotationEulers) << std::endl;
	ss << " PostRotation: " << Convert::ToString(m_PostRotationEulers) << std::endl;
	std::cout << ss.str() << std::endl;

	for (unsigned i = 0; i < m_Children.GetSize(); i++)
		m_Children[i]->PrintLocalData();
}

void FbxJoint::CalculateBindTransforms()
{
	if (!HasValidBindTransform())
	{
		if (m_pParent)
			m_BindTransform = m_pParent->GetBindTransform() * m_LocalTransform.AsInverseMatrix();
		else
			m_BindTransform = m_LocalTransform.AsInverseMatrix();
	}

	for (unsigned i = 0; i < m_Children.GetSize(); i++)
		m_Children[i]->CalculateBindTransforms();
}

bool FbxJoint::HasValidBindTransform() const
{
	return m_BindTransform.Get(0, 0) != Float::MAX;
}

void FbxJoint::SetInvalidBindTransform()
{
	m_BindTransform.Set(0, 0, Float::MAX);
}
