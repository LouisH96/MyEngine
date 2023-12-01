#include "FbxJoint.h"

#include "FbxClass.h"
#include "FbxLoadData.h"
#include "Io/Fbx/Wrapping/FbxData.h"
#include "Logger/Logger.h"

using namespace MyEngine;
using namespace Io::Fbx;
using namespace Game;

FbxJoint::FbxJoint(
	const Wrapping::Model& model,
	FbxLoadData& loadData)
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

	m_LocalTransform = loadData.Orientation.MakeLocalTransform(model);
}

FbxJoint::FbxJoint(FbxJoint&& other) noexcept
	: m_Name{ std::move(other.m_Name) }
	, m_LocalTransform(other.m_LocalTransform)
	, m_BoneTransform{ other.m_BoneTransform }
	, m_Children{ std::move(other.m_Children) }
	, m_pParent{ other.m_pParent }
	, m_Curves{ std::move(other.m_Curves) }
	, m_Translation{ other.m_Translation }
	, m_PreRotation{ other.m_PreRotation }
	, m_PostRotation{ other.m_PostRotation }
	, m_LclRotationEulers{other.m_LclRotationEulers}
	, m_PreRotationEulers{other.m_PreRotationEulers}
	, m_PostRotationEulers{other.m_PostRotationEulers}
{
	for (unsigned i = 0; i < m_Children.GetSize(); i++)
		m_Children[i]->m_pParent = this;
}

FbxJoint& FbxJoint::operator=(FbxJoint&& other) noexcept
{
	m_Name = std::move(other.m_Name);
	m_LocalTransform = other.m_LocalTransform;
	m_BoneTransform = other.m_BoneTransform;
	m_Children = std::move(other.m_Children);
	m_pParent = other.m_pParent;
	m_Curves = std::move(other.m_Curves);
	m_PreRotation = other.m_PreRotation;
	m_PostRotation = other.m_PostRotation;
	m_Translation = other.m_Translation;
	m_PreRotationEulers = other.m_PreRotationEulers;
	m_PostRotationEulers = other.m_PostRotationEulers;
	m_LclRotationEulers = other.m_LclRotationEulers;
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

void FbxJoint::CalculateBoneTransforms()
{
	if (m_pParent)
		m_BoneTransform = Transform::LocalToWorld(m_LocalTransform, m_pParent->GetBoneTransform());
	else
		m_BoneTransform = m_LocalTransform;

	for (unsigned i = 0; i < m_Children.GetSize(); i++)
		m_Children[i]->CalculateBoneTransforms();
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
