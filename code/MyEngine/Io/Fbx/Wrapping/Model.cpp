#include "pch.h"
#include "Model.h"

#include "Io/Fbx/Reading/FbxObject.h"
#include "Io/Fbx/Reading/Properties/FbxPropPrimitive.h"
#include "Io/Fbx/Reading/Properties70.h"

Io::Fbx::Wrapping::Model::Model(Reading::FbxObject& modelObject)
	: m_Id(modelObject.GetProperty(0)->AsPrimitive<int64_t>().GetValue())
	, m_Name(modelObject.GetProperty(1)->AsString())
	, m_TypeName(modelObject.GetProperty(2)->AsString())
	, m_Version(modelObject.GetChild(0).GetProperty(0)->AsPrimitive<int>().GetValue())
{
	const Reading::Properties70 properties{ modelObject.GetChild(1) };
	m_RotationOffset = properties.GetFloat3("RotationOffset", {});
	m_RotationPivot = properties.GetFloat3("RotationPivot", {});
	m_ScalingPivot = properties.GetFloat3("ScalingPivot", {});
	m_RotationActive = properties.GetBool("RotationActive");
	m_InheritType = properties.GetInt("InheritType");
	m_ScalingMax = properties.GetFloat3("ScalingMax");
	m_DefaultAttributeIndex = properties.GetInt("DefaultAttributeIndex");
	m_LclRotation = properties.GetFloat3("Lcl Rotation", {});
	m_CurrentUvSet = properties.GetString("currentUVSet");
}
