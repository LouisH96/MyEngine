#include "AnimationLayer.h"

#include "Io/Fbx/Reading/FbxElement.h"
#include "Io/Fbx/Reading/Properties/FbxPropPrimitive.h"
#include "Logger/Logger.h"

MyEngine::Io::Fbx::Wrapping::AnimationLayer::AnimationLayer(Reading::FbxElement& object)
	: Id{ object.GetProperty(0).AsPrimitive<int64_t>().GetValue() }
	, Name{ object.GetProperty(1).AsString() }
{
}

void MyEngine::Io::Fbx::Wrapping::AnimationLayer::SetAnimationStack(const AnimationStack& animationStack)
{
	if (m_pAnimationStack)
		Logger::PrintError("AnimationLayer already has a parent AnimationStack");
	m_pAnimationStack = &animationStack;
}

void MyEngine::Io::Fbx::Wrapping::AnimationLayer::AddAnimationCurveNode(const AnimationCurveNode& animationCurveNode)
{
	m_AnimationCurveNodes.Add(&animationCurveNode);
}
