#include "pch.h"
#include "AnimationCurveNode.h"

#include "Io/Fbx/Reading/FbxObject.h"
#include "Io/Fbx/Reading/Properties/FbxPropPrimitive.h"

Io::Fbx::Wrapping::AnimationCurveNode::AnimationCurveNode(Reading::FbxObject& object)
	: Id{ object.GetProperty(0)->AsPrimitive<int64_t>().GetValue() }
	, Type{ object.GetProperty(1)->AsString() }
	, pProperties70{ &object.GetChild(0) }
{
}

void Io::Fbx::Wrapping::AnimationCurveNode::AddAnimationCurve(const AnimationCurve& animationCurve)
{
	m_AnimationCurves.Add(&animationCurve);
}

void Io::Fbx::Wrapping::AnimationCurveNode::SetParentModel(const Model& model)
{
	if (m_pParentModel)
		Logger::PrintError("AnimationCurveNode already has a parent model");
	m_pParentModel = &model;
}

void Io::Fbx::Wrapping::AnimationCurveNode::SetAnimationLayer(const AnimationLayer& animationLayer)
{
	if (m_pAnimationLayer)
		Logger::PrintError("AnimationCurveNode already has an animationLayer");
	m_pAnimationLayer = &animationLayer;
}
