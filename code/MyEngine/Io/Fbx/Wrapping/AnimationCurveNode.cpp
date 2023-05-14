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
