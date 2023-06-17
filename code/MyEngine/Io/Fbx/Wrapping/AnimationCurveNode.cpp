#include "pch.h"
#include "AnimationCurveNode.h"

#include "Io/Fbx/Reading/FbxObject.h"
#include "Io/Fbx/Reading/Properties/FbxPropPrimitive.h"

Io::Fbx::Wrapping::AnimationCurveNode::AnimationCurveNode(Reading::FbxObject& object)
	: Id{ object.GetProperty(0)->AsPrimitive<int64_t>().GetValue() }
	, m_NodeType{ GetNodeType(object.GetProperty(1)->AsString()) }
{
	const Reading::Properties70 props{ object.GetChild(0) };
	switch (m_NodeType)
	{
		//probably not correct value since double is stored
	case NodeType::LockInfluenceWeights:
		m_Value.boolValue = props.GetDouble(0) == 1;
		break;
	case NodeType::Translation:
		m_Value.vectorValue = {
			props.GetDouble(0),
			props.GetDouble(1),
			props.GetDouble(2)
		};
		break;
	case NodeType::Rotation:
		m_Value.vectorValue = {
			props.GetDouble(0),
			props.GetDouble(1),
			props.GetDouble(2)
		};
		break;
	case NodeType::Scale:
		m_Value.vectorValue = {
			props.GetDouble(0),
			props.GetDouble(1),
			props.GetDouble(2)
		};
		break;
		//probably not correct value since double is stored
	case NodeType::Visibility:
		m_Value.boolValue = props.GetDouble(0) == 1;
		break;
	case NodeType::FilmboxTypeId:
		m_Value.shortValue = props.GetShort(0);
		break;
	case NodeType::FocalLength:
		m_Value.vectorValue.x = props.GetDouble(0);
		break;
	default:break;
	}
}

void Io::Fbx::Wrapping::AnimationCurveNode::AddNodeAttribute(const NodeAttribute& nodeAttribute)
{
	m_NodeAttributes.Add(&nodeAttribute);
}

void Io::Fbx::Wrapping::AnimationCurveNode::AddAnimationCurve(const AnimationCurve& animationCurve)
{
	m_AnimationCurves.Add(&animationCurve);
}

void Io::Fbx::Wrapping::AnimationCurveNode::SetParentModel(const Model& model)
{
	if (m_pParentModel)
		Logger::PrintError("AnimationCurveNode already has a parent model");
	m_pParentModel = &model;
}

void Io::Fbx::Wrapping::AnimationCurveNode::SetAnimationLayer(const AnimationLayer& animationLayer)
{
	if (m_pAnimationLayer)
		Logger::PrintError("AnimationCurveNode already has an animationLayer");
	m_pAnimationLayer = &animationLayer;
}

Io::Fbx::Wrapping::AnimationCurveNode::NodeType Io::Fbx::Wrapping::AnimationCurveNode::GetNodeType(
	const std::string& typeName)
{
	if (typeName == "lockInfluenceWeights") return NodeType::LockInfluenceWeights;
	if (typeName == "Visibility") return NodeType::Visibility;
	if (typeName == "T") return NodeType::Translation;
	if (typeName == "R") return NodeType::Rotation;
	if (typeName == "S") return NodeType::Scale;
	if (typeName == "filmboxTypeID") return NodeType::FilmboxTypeId;
	if (typeName == "FocalLength") return NodeType::FocalLength;
	Logger::PrintWarning("Unknown AnimationCurveNode type: " + typeName);
	return NodeType::Other;
}

void Io::Fbx::Wrapping::AnimationCurveNode::Print() const
{
	switch (m_NodeType)
	{
	case NodeType::LockInfluenceWeights:
		std::cout << "LockInfluenceWeight: " << m_Value.boolValue << std::endl;
		break;
	case NodeType::Translation:
		std::cout << "Translation: " << ToString::Convert(m_Value.vectorValue) << std::endl;
		break;
	case NodeType::Rotation:
		std::cout << "Rotation: " << ToString::Convert(m_Value.vectorValue) << std::endl;
		break;
	case NodeType::Scale:
		std::cout << "Scale: " << ToString::Convert(m_Value.vectorValue) << std::endl;
		break;
	case NodeType::Visibility:
		std::cout << "Visibility: " << m_Value.boolValue << std::endl;
		break;
	case NodeType::FilmboxTypeId:
		std::cout << "FilmboxTypeId: " << m_Value.shortValue << std::endl;
		break;
	case NodeType::FocalLength: 
		std::cout << "FocalLength: " << m_Value.vectorValue.x << std::endl;
		break;
	default:
	case NodeType::Other:
		std::cout << "Other" << std::endl;
		break;
	}
}

bool Io::Fbx::Wrapping::AnimationCurveNode::IsTransformNode() const
{
	return m_NodeType == NodeType::Translation
		|| m_NodeType == NodeType::Rotation
		|| m_NodeType == NodeType::Scale;
}
