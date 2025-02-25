#include "AnimationCurve.h"

#include "Io/Fbx/Reading/FbxElement.h"
#include "Io/Fbx/Reading/Properties/FbxPropArray.h"
#include "Io/Fbx/Reading/Properties/FbxPropPrimitive.h"
#include "Logger/Logger.h"

MyEngine::Io::Fbx::Wrapping::AnimationCurve::AnimationCurve(Reading::FbxElement& object)
	: Id{ object.GetProperty(0).AsPrimitive<int64_t>().GetValue() }
	, Default{ object.GetChild(0).GetProperty(0).AsPrimitive<double>().GetValue() }
	, KeyVer{ object.GetChild(1).GetProperty(0).AsPrimitive<int>().GetValue() }
{
	object.GetChild(2).MovePropertyTo(0, KeyTimes);
	object.GetChild(3).MovePropertyTo(0, KeyValueFloats);
	object.GetChild(4).MovePropertyTo(0, KeyAttrFlags);
	object.GetChild(5).MovePropertyTo(0, KeyAttrDataFloat);
	object.GetChild(6).MovePropertyTo(0, KeyAttrRefCount);
}

void MyEngine::Io::Fbx::Wrapping::AnimationCurve::SetAnimationCurveNode(const AnimationCurveNode& animationCurveNode)
{
	if (m_pAnimationCurveNode)
		Logger::Error("AnimationCurveNode already set");
	m_pAnimationCurveNode = &animationCurveNode;
}
