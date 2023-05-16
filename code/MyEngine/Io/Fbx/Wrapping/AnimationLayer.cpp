#include "pch.h"
#include "AnimationLayer.h"

#include "Io/Fbx/Reading/FbxObject.h"
#include "Io/Fbx/Reading/Properties/FbxPropPrimitive.h"

Io::Fbx::Wrapping::AnimationLayer::AnimationLayer(Reading::FbxObject& object)
	: Id{ object.GetProperty(0)->AsPrimitive<int64_t>().GetValue() }
	, Name{ object.GetProperty(1)->AsString() }
{

}

void Io::Fbx::Wrapping::AnimationLayer::SetAnimationStack(const AnimationStack& animationStack)
{
	if (m_pAnimationStack)
		Logger::PrintError("AnimationLayer already has a parent AnimationStack");
	m_pAnimationStack = &animationStack;
}

void Io::Fbx::Wrapping::AnimationLayer::AddAnimationCurveNode(const AnimationCurveNode& animationCurveNode)
{
	m_AnimationCurveNodes.Add(&animationCurveNode);
}
