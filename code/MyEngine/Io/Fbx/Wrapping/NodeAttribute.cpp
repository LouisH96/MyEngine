#include "pch.h"
#include "NodeAttribute.h"

#include "Io/Fbx/Reading/FbxObject.h"
#include "Io/Fbx/Reading/Properties/FbxPropPrimitive.h"

Io::Fbx::Wrapping::NodeAttribute::NodeAttribute(Reading::FbxObject& object)
	: Id{ object.GetProperty(0)->AsPrimitive<int64_t>().GetValue() }
	, Type{ object.GetProperty(2)->AsString() }
	, pProperties70{ &object.GetChild(0) }
{
}

void Io::Fbx::Wrapping::NodeAttribute::SetParentModel(const Model& parentModel)
{
	if (m_pParentModel)
		Logger::PrintError("NodeAttribute already has a parent model");
	m_pParentModel = &parentModel;
}
