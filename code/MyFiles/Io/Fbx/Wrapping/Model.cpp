#include "Model.h"

#include "AnimationCurveNode.h"
#include "AnimationLayer.h"
#include "Io/Fbx/Reading/FbxObject.h"
#include "Io/Fbx/Reading/Properties/FbxPropPrimitive.h"
#include "Io/Fbx/Reading/Properties70.h"
#include "Logger/Logger.h"

MyEngine::Io::Fbx::Wrapping::Model::Model(Reading::FbxObject& modelObject)
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
	m_RotationActive = properties.GetBool("RotationActive", false);
	m_InheritType = properties.GetInt("InheritType", 0);
	m_ScalingMax = properties.GetFloat3("ScalingMax", {1,1,1});
	m_DefaultAttributeIndex = properties.GetInt("DefaultAttributeIndex",0);
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

bool MyEngine::Io::Fbx::Wrapping::Model::IsLimbNode() const
{
	return m_TypeName == "LimbNode";
}

void MyEngine::Io::Fbx::Wrapping::Model::SetParentModel(const Model& parent)
{
	if (m_pParentModel)
		Logger::PrintError("ParentModel already set");
	m_pParentModel = &parent;
}

void MyEngine::Io::Fbx::Wrapping::Model::SetNodeAttribute(const NodeAttribute& nodeAttribute)
{
	if (m_pNodeAttribute)
		Logger::PrintError("NodeAttribute in model already set");
	m_pNodeAttribute = &nodeAttribute;
}

void MyEngine::Io::Fbx::Wrapping::Model::AddChildModel(const Model& child)
{
	m_ChildModels.Add(&child);
}

void MyEngine::Io::Fbx::Wrapping::Model::AddDeformer(const Deformer& deformer)
{
	m_Deformers.Add(&deformer);
}

void MyEngine::Io::Fbx::Wrapping::Model::AddAnimationCurveNode(const AnimationCurveNode& animationCurveNode)
{
	m_AnimationCurveNodes.Add(&animationCurveNode);
}

void MyEngine::Io::Fbx::Wrapping::Model::AddMaterial(const FbxWrapMaterial& material)
{
	m_Materials.Add(&material);
}

void MyEngine::Io::Fbx::Wrapping::Model::SetCollection(const CollectionExclusive& collection)
{
	if (m_pCollection)
		Logger::PrintError("Model already has a CollectionExclusive");
	m_pCollection = &collection;
}

MyEngine::Array<const MyEngine::Io::Fbx::Wrapping::Model*> MyEngine::Io::Fbx::Wrapping::Model::GetLimbNodes() const
{
	Array<const Model*> limbNodeModels{};
	for (int i = 0; i < m_ChildModels.GetSize(); i++)
		if (m_ChildModels[i]->IsLimbNode())
			limbNodeModels.Add(m_ChildModels[i]);
	return limbNodeModels;
}

const MyEngine::Io::Fbx::Wrapping::Model& MyEngine::Io::Fbx::Wrapping::Model::GetRootParentModel() const
{
	if (m_pParentModel)
		return m_pParentModel->GetRootParentModel();
	return *this;
}

MyEngine::Array<const MyEngine::Io::Fbx::Wrapping::Model*> MyEngine::Io::Fbx::Wrapping::Model::GetChildrenBreadthFirst() const
{
	Array<const Model*> children{ m_ChildModels };
	for (int i = 0; i < m_ChildModels.GetSize(); i++)
		m_ChildModels[i]->AddChildrenBreadthFirst(children);
	return children;
}

void MyEngine::Io::Fbx::Wrapping::Model::AddChildrenBreadthFirst(
	Array<const Model*>& models) const
{
	int iOutput{ models.GetSize() };
	models.IncreaseSizeWith(m_ChildModels.GetSize());
	for (int iChild = 0; iChild < m_ChildModels.GetSize(); iChild++) models[iOutput++] = m_ChildModels[iChild];
	for (int iChild = 0; iChild < m_ChildModels.GetSize(); iChild++) m_ChildModels[iChild]->AddChildrenBreadthFirst(models);
}

const MyEngine::Io::Fbx::Wrapping::AnimationCurveNode* MyEngine::Io::Fbx::Wrapping::Model::GetTranslationCurveNode(int64_t layerId) const
{
	for (int i = 0; i < m_AnimationCurveNodes.GetSize(); i++)
	{
		const AnimationCurveNode& node{ *m_AnimationCurveNodes[i] };
		if (node.GetAnimationLayer().Id != layerId) continue;
		if (node.GetNodeType() != AnimationCurveNode::NodeType::Translation) continue;
		return &node;
	}
	return nullptr;
}

const MyEngine::Io::Fbx::Wrapping::AnimationCurveNode* MyEngine::Io::Fbx::Wrapping::Model::GetRotationCurveNode(int64_t layerId) const
{
	for (int i = 0; i < m_AnimationCurveNodes.GetSize(); i++)
	{
		const AnimationCurveNode& node{ *m_AnimationCurveNodes[i] };
		if (node.GetAnimationLayer().Id != layerId) continue;
		if (node.GetNodeType() != AnimationCurveNode::NodeType::Rotation) continue;
		return &node;
	}
	return nullptr;
}

const MyEngine::Io::Fbx::Wrapping::AnimationCurveNode* MyEngine::Io::Fbx::Wrapping::Model::GetScaleCurveNode(int64_t layerId) const
{
	for (int i = 0; i < m_AnimationCurveNodes.GetSize(); i++)
	{
		const AnimationCurveNode& node{ *m_AnimationCurveNodes[i] };
		if (node.GetAnimationLayer().Id != layerId) continue;
		if (node.GetNodeType() != AnimationCurveNode::NodeType::Scale) continue;
		return &node;
	}
	return nullptr;
}
