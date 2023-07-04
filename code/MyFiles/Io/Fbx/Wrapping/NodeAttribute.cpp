#include "NodeAttribute.h"

#include "Io/Fbx/Reading/FbxObject.h"
#include "Io/Fbx/Reading/Properties/FbxPropPrimitive.h"
#include "Logger/Logger.h"

MyEngine::Io::Fbx::Wrapping::NodeAttribute::NodeAttribute(Reading::FbxObject& object)
	: Id{ object.GetProperty(0)->AsPrimitive<int64_t>().GetValue() }
	, Type{ object.GetProperty(2)->AsString() }
	, pProperties70{ &object.GetChild(0) }
{
}

void MyEngine::Io::Fbx::Wrapping::NodeAttribute::SetParentModel(const Model& parentModel)
{
	if (m_pParentModel)
		Logger::PrintError("NodeAttribute already has a parent model");
	m_pParentModel = &parentModel;
}

void MyEngine::Io::Fbx::Wrapping::NodeAttribute::AddAnimationCurveNode(const AnimationCurveNode& animationCurveNode)
{
	m_AnimationCurveNodes.Add(&animationCurveNode);
}
