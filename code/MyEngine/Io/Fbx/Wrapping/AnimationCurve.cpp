#include "pch.h"
#include "AnimationCurve.h"

#include "Io/Fbx/Reading/FbxObject.h"
#include "Io/Fbx/Reading/Properties/FbxPropArray.h"
#include "Io/Fbx/Reading/Properties/FbxPropPrimitive.h"

Io::Fbx::Wrapping::AnimationCurve::AnimationCurve(Reading::FbxObject& object)
	: Id{ object.GetProperty(0)->AsPrimitive<int64_t>().GetValue() }
	, Default{ object.GetChild(0).GetProperty(0)->AsPrimitive<double>().GetValue() }
	, KeyVer{ object.GetChild(1).GetProperty(0)->AsPrimitive<int>().GetValue() }
	, KeyTimes{ std::move(object.GetChild(2).GetProperty(0)->AsArray<int64_t>().GetValues()) }
	, KeyValueFloats{ std::move(object.GetChild(3).GetProperty(0)->AsArray<float>().GetValues()) }
	, KeyAttrFlags{ std::move(object.GetChild(4).GetProperty(0)->AsArray<int>().GetValues()) }
	, KeyAttrDataFloat{ std::move(object.GetChild(5).GetProperty(0)->AsArray<float>().GetValues()) }
	, KeyAttrRefCount{ std::move(object.GetChild(6).GetProperty(0)->AsArray<int>().GetValues()) }
{
}

void Io::Fbx::Wrapping::AnimationCurve::SetAnimationCurveNode(const AnimationCurveNode& animationCurveNode)
{
	if (m_pAnimationCurveNode)
		Logger::PrintError("AnimationCurveNode already set");
	m_pAnimationCurveNode = &animationCurveNode;
}
