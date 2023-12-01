#include "Model.h"

#include "AnimationCurveNode.h"
#include "AnimationLayer.h"
#include "FbxOrientation.h"
#include "Io/Fbx/Reading/FbxElement.h"
#include "Io/Fbx/Reading/Properties/FbxPropPrimitive.h"
#include "Io/Fbx/Reading/Properties70.h"
#include "Logger/Logger.h"

using namespace MyEngine::Io::Fbx::Wrapping;

Model::Model(Reading::FbxElement& modelObject, const FbxOrientation& orientation)
	: m_Id(modelObject.GetProperty(0).AsPrimitive<int64_t>().GetValue())
	, m_Name(modelObject.GetProperty(1).AsString())
	, m_TypeName(modelObject.GetProperty(2).AsString())
	, m_Version(modelObject.GetChild(0).GetProperty(0).AsPrimitive<int>().GetValue())
{
	const Reading::Properties70 properties{ modelObject.GetChild(1) };
	m_PreRotation = properties.GetFloat3("PreRotation", {});
	m_PostRotation = properties.GetFloat3("PostRotation", {});
	m_RotationOffset = properties.GetFloat3("RotationOffset", {});
	m_RotationPivot = properties.GetFloat3("RotationPivot", {});
	m_ScalingPivot = properties.GetFloat3("ScalingPivot", {});
	m_RotationActive = properties.GetBool("RotationActive", false);
	m_InheritType = properties.GetInt("InheritType", 0);
	m_ScalingMax = properties.GetFloat3("ScalingMax", { 1,1,1 });
	m_GeometricTranslation = properties.GetFloat3("GeometricTranslation", {});
	m_DefaultAttributeIndex = properties.GetInt("DefaultAttributeIndex", 0);
	m_LclTranslation = properties.GetFloat3("Lcl Translation", {});
	m_LclRotation = properties.GetFloat3("Lcl Rotation", {});
	m_LclScaling = properties.GetFloat3("Lcl Scaling", { 1,1,1 });
	m_CurrentUvSet = properties.GetString("currentUVSet", "");

	m_LclTranslation = orientation.ConvertPoint(m_LclTranslation);
	m_RotationPivot = orientation.ConvertPoint(m_RotationPivot);
	m_RotationOffset = orientation.ConvertPoint(m_RotationOffset);
	m_PreRotation = orientation.ConvertRotation(m_PreRotation);
	m_PostRotation = orientation.ConvertRotation(m_PostRotation);
	m_LclRotation = orientation.ConvertRotation(m_LclRotation);

	m_GeometricTranslation = orientation.ConvertPoint(m_GeometricTranslation);
}

bool Model::IsLimbNode() const
{
	return m_TypeName == "LimbNode";
}

void Model::SetParentModel(const Model& parent)
{
	if (m_pParentModel)
		Logger::PrintError("ParentModel already set");
	m_pParentModel = &parent;
}

void Model::SetNodeAttribute(const NodeAttribute& nodeAttribute)
{
	if (m_pNodeAttribute)
		Logger::PrintError("NodeAttribute in model already set");
	m_pNodeAttribute = &nodeAttribute;
}

void Model::AddChildModel(const Model& child)
{
	m_ChildModels.Add(&child);
}

void Model::AddDeformer(const Deformer& deformer)
{
	m_Deformers.Add(&deformer);
}

void Model::AddAnimationCurveNode(const AnimationCurveNode& animationCurveNode)
{
	m_AnimationCurveNodes.Add(&animationCurveNode);
}

void Model::AddMaterial(const FbxWrapMaterial& material)
{
	m_Materials.Add(&material);
}

void Model::SetCollection(const CollectionExclusive& collection)
{
	if (m_pCollection)
		Logger::PrintError("Model already has a CollectionExclusive");
	m_pCollection = &collection;
}

MyEngine::Array<const Model*> Model::GetLimbNodes() const
{
	Array<const Model*> limbNodeModels{};
	for (unsigned i = 0; i < m_ChildModels.GetSize(); i++)
		if (m_ChildModels[i]->IsLimbNode())
			limbNodeModels.Add(m_ChildModels[i]);
	return limbNodeModels;
}

const Model& Model::GetRootParentModel() const
{
	if (m_pParentModel)
		return m_pParentModel->GetRootParentModel();
	return *this;
}

MyEngine::Array<const Model*> Model::GetChildrenBreadthFirst() const
{
	Array<const Model*> children{ m_ChildModels };
	for (unsigned i = 0; i < m_ChildModels.GetSize(); i++)
		m_ChildModels[i]->AddChildrenBreadthFirst(children);
	return children;
}

void Model::AddChildrenBreadthFirst(
	Array<const Model*>& models) const
{
	unsigned iOutput{ models.GetSize() };
	models.IncreaseSizeWith(m_ChildModels.GetSize());
	for (unsigned iChild = 0; iChild < m_ChildModels.GetSize(); iChild++) models[iOutput++] = m_ChildModels[iChild];
	for (unsigned iChild = 0; iChild < m_ChildModels.GetSize(); iChild++) m_ChildModels[iChild]->AddChildrenBreadthFirst(models);
}

const AnimationCurveNode* Model::GetTranslationCurveNode(int64_t layerId) const
{
	for (unsigned i = 0; i < m_AnimationCurveNodes.GetSize(); i++)
	{
		const AnimationCurveNode& node{ *m_AnimationCurveNodes[i] };
		if (node.GetAnimationLayer().Id != layerId) continue;
		if (node.GetNodeType() != AnimationCurveNode::NodeType::Translation) continue;
		return &node;
	}
	return nullptr;
}

const AnimationCurveNode* Model::GetRotationCurveNode(int64_t layerId) const
{
	for (unsigned i = 0; i < m_AnimationCurveNodes.GetSize(); i++)
	{
		const AnimationCurveNode& node{ *m_AnimationCurveNodes[i] };
		if (node.GetAnimationLayer().Id != layerId) continue;
		if (node.GetNodeType() != AnimationCurveNode::NodeType::Rotation) continue;
		return &node;
	}
	return nullptr;
}

const AnimationCurveNode* Model::GetScaleCurveNode(int64_t layerId) const
{
	for (unsigned i = 0; i < m_AnimationCurveNodes.GetSize(); i++)
	{
		const AnimationCurveNode& node{ *m_AnimationCurveNodes[i] };
		if (node.GetAnimationLayer().Id != layerId) continue;
		if (node.GetNodeType() != AnimationCurveNode::NodeType::Scale) continue;
		return &node;
	}
	return nullptr;
}

MyEngine::Game::Transform Model::MakeLocalTransform(float scale) const
{
	using namespace Game;

	Transform preZYX{ {}, Quaternion::FromEulerDegrees({0,0,m_PreRotation.z}) };
	preZYX = Transform::LocalToWorld({ {}, Quaternion::FromEulerDegrees({0, m_PreRotation.y, 0}) }, preZYX);
	preZYX = Transform::LocalToWorld({ {}, Quaternion::FromEulerDegrees({m_PreRotation.x,0,0}) }, preZYX);

	Transform postXYZ{ {}, Quaternion::FromEulerDegrees({-m_PostRotation.x,0,0}) };
	postXYZ = Transform::LocalToWorld({ {}, Quaternion::FromEulerDegrees({0,-m_PostRotation.y, 0}) }, postXYZ);
	postXYZ = Transform::LocalToWorld({ {}, Quaternion::FromEulerDegrees({0,0, -m_PostRotation.z}) }, postXYZ);

	const Transform lclRotation{ {}, Quaternion::FromEulerDegrees(m_LclRotation) };

	Transform local{ m_LclTranslation * scale, {} };
	local = Transform::LocalToWorld(preZYX, local);
	local = Transform::LocalToWorld(lclRotation, local);
	local = Transform::LocalToWorld(postXYZ, local);
	return local;
}
