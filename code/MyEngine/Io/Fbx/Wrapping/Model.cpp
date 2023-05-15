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
	m_PreRotation = properties.GetFloat3("PreRotation", {});
	m_PostRotation = properties.GetFloat3("PostRotation", {});
	m_RotationOffset = properties.GetFloat3("RotationOffset", {});
	m_RotationPivot = properties.GetFloat3("RotationPivot", {});
	m_ScalingPivot = properties.GetFloat3("ScalingPivot", {});
	m_RotationActive = properties.GetBool("RotationActive");
	m_InheritType = properties.GetInt("InheritType");
	m_ScalingMax = properties.GetFloat3("ScalingMax");
	m_DefaultAttributeIndex = properties.GetInt("DefaultAttributeIndex");
	m_LclTranslation = properties.GetFloat3("Lcl Translation", {});
	m_LclRotation = properties.GetFloat3("Lcl Rotation", {});
	m_LclScaling = properties.GetFloat3("Lcl Scaling", { 1,1,1 });
	m_CurrentUvSet = properties.GetString("currentUVSet", "");

	m_RotationPivot.x = -m_RotationPivot.x;
	m_RotationOffset.x = -m_RotationOffset.x;

	//LH & Y-UP & X-RIGHT
	m_LclTranslation.x = -m_LclTranslation.x;
	m_PreRotation.y = -m_PreRotation.y;
	m_PreRotation.z = -m_PreRotation.z;
	m_PostRotation.x = -m_PostRotation.x;
}

void Io::Fbx::Wrapping::Model::SetParentModel(const Model& parent)
{
	if (m_pParentModel)
		Logger::PrintError("ParentModel already set");
	m_pParentModel = &parent;
}

void Io::Fbx::Wrapping::Model::AddChildModel(const Model& child)
{
	m_ChildModels.Add(&child);
}

void Io::Fbx::Wrapping::Model::AddDeformer(const Deformer& deformer)
{
	m_Deformers.Add(&deformer);
}
