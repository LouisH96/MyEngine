#include "FbxJoint.h"

#include "FbxClass.h"
#include "Io/Fbx/Wrapping/FbxData.h"
#include "Logger/Logger.h"

using namespace MyEngine;
using namespace Io::Fbx;
using namespace Game;

FbxJoint::FbxJoint(
	const Wrapping::Model& model,
	const FbxClass& fbxClass)
	: m_Name{ model.GetName() }
	, m_Curves{ fbxClass.GetNrOfAnimationLayers() }
{
	//ANIMATION
	for (unsigned iAnimation = 0, iCurve = 0; iAnimation < fbxClass.GetAnimations().GetSize(); iAnimation++)
	{
		const FbxAnimation& animation{ fbxClass.GetAnimations()[iAnimation] };
		for (unsigned iLayer = 0; iLayer < animation.GetLayers().GetSize(); iLayer++, iCurve++)
			m_Curves[iCurve] = FbxTransformCurve{ model, animation.GetLayers()[iLayer] };
	}

	//POSITION
	const Float3 translation{ model.GetLclTranslation() };
	const Quaternion preRotation{ Quaternion::FromEulerDegrees(model.GetPreRotation()) };

	const Float3& postEulers{ model.GetPostRotation() };
	const Quaternion postRotationX{ Quaternion::FromEulerDegrees({postEulers.x, 0, 0}) };
	const Quaternion postRotationY{ Quaternion::FromEulerDegrees({0, postEulers.y, 0}) };
	const Quaternion postRotationZ{ Quaternion::FromEulerDegrees({0, 0, postEulers.z}) };

	Quaternion rotation{ postRotationZ };
	rotation.RotateBy(postRotationY);
	rotation.RotateBy(postRotationX);
	rotation.RotateBy(preRotation);

	m_LocalTransform = { translation, rotation };

	//
	m_LocalTranslation = translation;
	m_PreRotation = preRotation;
	m_PostRotation = { postRotationZ };
	m_PostRotation.RotateBy(postRotationY);
	m_PostRotation.RotateBy(postRotationX);
}

FbxJoint::FbxJoint(FbxJoint&& other) noexcept
	: m_Name{ std::move(other.m_Name) }
	, m_LocalTransform(std::move(other.m_LocalTransform))
	, m_BoneTransform{ other.m_BoneTransform }
	, m_Children{ std::move(other.m_Children) }
	, m_pParent{ std::move(other.m_pParent) }
	, m_Curves{ std::move(other.m_Curves) }
	, m_LocalTranslation{ std::move(other.m_LocalTranslation) }
	, m_PreRotation{ std::move(other.m_PreRotation) }
	, m_PostRotation{ std::move(other.m_PostRotation) }
{
	for (unsigned i = 0; i < m_Children.GetSize(); i++)
		m_Children[i]->m_pParent = this;
}

FbxJoint& FbxJoint::operator=(FbxJoint&& other) noexcept
{
	m_Name = std::move(other.m_Name);
	m_LocalTransform = std::move(other.m_LocalTransform);
	m_BoneTransform = other.m_BoneTransform;
	m_Children = std::move(other.m_Children);
	m_pParent = std::move(other.m_pParent);
	m_Curves = std::move(other.m_Curves);
	m_LocalTranslation = std::move(other.m_LocalTranslation);
	m_PreRotation = std::move(other.m_PreRotation);
	m_PostRotation = std::move(other.m_PostRotation);
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
		m_BoneTransform = m_pParent->GetBoneTransform() * m_LocalTransform.AsMatrix();
	else
		m_BoneTransform = m_LocalTransform.AsMatrix();

	for (unsigned i = 0; i < m_Children.GetSize(); i++)
		m_Children[i]->CalculateBoneTransforms();
}
