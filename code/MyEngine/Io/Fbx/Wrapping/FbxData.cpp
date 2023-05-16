#include "pch.h"
#include "FbxData.h"

#include "../Reading/FbxObject.h"
#include "../Reading/FbxReader.h"
#include "Debug/DebugRenderer.h"

using namespace Math;

Io::Fbx::Wrapping::FbxData::FbxData(const std::wstring& fbxPath)
	: FbxData(Fbx::Reading::FbxReader{ fbxPath })
{}

Io::Fbx::Wrapping::FbxData::FbxData(Reading::FbxReader&& reader)
{
	const Reading::FbxObject& objects{ *reader.GetRoot().GetChild("Objects") };
	const Reading::FbxObject& connections{ *reader.GetRoot().GetChild("Connections") };

	//reader.GetRoot().Print();

	ReadGeometry(objects);
	ReadModels(objects);
	ReadDeformers(objects);
	ReadPoses(objects);
	ReadConnections(connections);
	ReadAnimationStack(objects);
	ReadAnimationLayers(objects);
	ReadAnimationCurveNodes(objects);
	ReadAnimationCurves(objects);

	HandleConnections();
}

Array<Io::Fbx::Wrapping::Model> Io::Fbx::Wrapping::FbxData::GetModelsOfType(const std::string& typeName) const
{
	int count{ 0 };
	for (int i = 0; i < m_Models.GetSize(); i++)
		if (m_Models[i].GetTypeName() == typeName)
			count++;

	int iModel{ 0 };
	Array<Model> models{ count };
	for (int i = 0; i < m_Models.GetSize(); i++)
		if (m_Models[i].GetTypeName() == typeName)
			models[iModel++] = m_Models[i];

	return models;
}

Array<Io::Fbx::Wrapping::Model> Io::Fbx::Wrapping::FbxData::GetLimbNodes() const
{
	return GetModelsOfType("LimbNode");
}

const Io::Fbx::Wrapping::Model* Io::Fbx::Wrapping::FbxData::GetRootLimbNode() const
{
	for (int iModel = 0; iModel < m_Models.GetSize(); iModel++)
	{
		const Model& model{ m_Models[iModel] };
		if (model.GetTypeName() != "LimbNode") continue;

		//find root connection
		for (int iConnection = 0; iConnection < m_Connections.GetSize(); iConnection++)
		{
			const Connection& connection{ m_Connections[iConnection] };
			if (connection.ChildId != model.GetId()) continue;
			if (connection.ParentId == 0)
				return &model;

			const Model* pParent{ FindModel(connection.ParentId) };
			if (pParent->GetTypeName() == "Null")
				return &model;
		}
	}
	return nullptr;
}

Io::Fbx::Wrapping::Geometry* Io::Fbx::Wrapping::FbxData::FindGeometry(const int64_t& id)
{
	for (int i = 0; i < m_Geometries.GetSize(); i++)
		if (m_Geometries[i].GetId() == id)
			return &m_Geometries[i];
	return nullptr;
}

Io::Fbx::Wrapping::Model* Io::Fbx::Wrapping::FbxData::FindModel(const int64_t& id)
{
	for (int i = 0; i < m_Models.GetSize(); i++)
		if (m_Models[i].GetId() == id)
			return &m_Models[i];
	return nullptr;
}

Io::Fbx::Wrapping::Deformer* Io::Fbx::Wrapping::FbxData::FindDeformer(const int64_t& id)
{
	for (int i = 0; i < m_Deformers.GetSize(); i++)
		if (m_Deformers[i].Id == id)
			return &m_Deformers[i];
	return nullptr;
}

Io::Fbx::Wrapping::Pose::Node* Io::Fbx::Wrapping::FbxData::FindPoseNode(const int64_t& id)
{
	for (int i = 0; i < m_BindPose.Nodes.GetSize(); i++)
		if (m_BindPose.Nodes[i].Id == id)
			return &m_BindPose.Nodes[i];
	return nullptr;
}

Io::Fbx::Wrapping::AnimationCurve* Io::Fbx::Wrapping::FbxData::FindAnimationCurve(const int64_t& id)
{
	for (int i = 0; i < m_AnimationCurves.GetSize(); i++)
		if (m_AnimationCurves[i].Id == id)
			return &m_AnimationCurves[i];
	return nullptr;
}

Io::Fbx::Wrapping::AnimationCurveNode* Io::Fbx::Wrapping::FbxData::FindAnimationCurveNode(const int64_t& id)
{
	for (int i = 0; i < m_AnimationCurveNodes.GetSize(); i++)
		if (m_AnimationCurveNodes[i].Id == id)
			return &m_AnimationCurveNodes[i];
	return nullptr;
}

const Io::Fbx::Wrapping::Geometry* Io::Fbx::Wrapping::FbxData::FindGeometry(const int64_t& id) const
{
	for (int i = 0; i < m_Geometries.GetSize(); i++)
		if (m_Geometries[i].GetId() == id)
			return &m_Geometries[i];
	return nullptr;
}

const Io::Fbx::Wrapping::Deformer* Io::Fbx::Wrapping::FbxData::FindDeformer(const int64_t& id) const
{
	for (int i = 0; i < m_Deformers.GetSize(); i++)
		if (m_Deformers[i].Id == id)
			return &m_Deformers[i];
	return nullptr;
}

const Io::Fbx::Wrapping::Pose::Node* Io::Fbx::Wrapping::FbxData::FindPoseNode(const int64_t& id) const
{
	for (int i = 0; i < m_BindPose.Nodes.GetSize(); i++)
		if (m_BindPose.Nodes[i].Id == id)
			return &m_BindPose.Nodes[i];
	return nullptr;
}

const Io::Fbx::Wrapping::AnimationLayer* Io::Fbx::Wrapping::FbxData::FindAnimationLayer(const int64_t& id) const
{
	for (int i = 0; i < m_AnimationLayers.GetSize(); i++)
		if (m_AnimationLayers[i].Id == id)
			return &m_AnimationLayers[i];
	return nullptr;
}

const Io::Fbx::Wrapping::AnimationCurve* Io::Fbx::Wrapping::FbxData::FindAnimationCurve(const int64_t& id) const
{
	for (int i = 0; i < m_AnimationCurves.GetSize(); i++)
		if (m_AnimationCurves[i].Id == id)
			return &m_AnimationCurves[i];
	return nullptr;
}

const Io::Fbx::Wrapping::AnimationCurveNode* Io::Fbx::Wrapping::FbxData::FindAnimationCurveNode(const int64_t& id) const
{
	for (int i = 0; i < m_AnimationCurveNodes.GetSize(); i++)
		if (m_AnimationCurveNodes[i].Id == id)
			return &m_AnimationCurveNodes[i];
	return nullptr;
}

const Io::Fbx::Wrapping::Model* Io::Fbx::Wrapping::FbxData::FindModel(const int64_t& id) const
{
	for (int i = 0; i < m_Models.GetSize(); i++)
		if (m_Models[i].GetId() == id)
			return &m_Models[i];
	return nullptr;
}

Array<const Io::Fbx::Wrapping::Model*> Io::Fbx::Wrapping::FbxData::GetChildren(const Model& model) const
{
	return GetChildren(model.GetId());
}

Array<const Io::Fbx::Wrapping::Model*> Io::Fbx::Wrapping::FbxData::GetChildren(const int64_t& id) const
{
	Array<const Model*> children{ 0 };
	for (int i = 0; i < m_Connections.GetSize(); i++)
	{
		const Connection& connection{ m_Connections[i] };
		if (connection.ParentId != id) continue;
		if (connection.Relation != "OO") continue;
		const Model* pChild{ FindModel(connection.ChildId) };
		if (pChild)
			children.Add(pChild);
	}
	return children;
}

void Io::Fbx::Wrapping::FbxData::ReadGeometry(const Reading::FbxObject& objectsObject)
{
	const std::vector<Reading::FbxObject*> geometries{ objectsObject.GetChildren("Geometry") };
	m_Geometries = { static_cast<int>(geometries.size()) };
	for (int i = 0; i < geometries.size(); i++)
		m_Geometries[i] = Geometry{ *geometries[i] };
}

void Io::Fbx::Wrapping::FbxData::ReadModels(const Reading::FbxObject& objectsObject)
{
	const std::vector<Reading::FbxObject*> models{ objectsObject.GetChildren("Model") };
	m_Models = { static_cast<int>(models.size()) };
	for (int i = 0; i < m_Models.GetSize(); i++)
		m_Models[i] = Model{ *models[i] };
}

void Io::Fbx::Wrapping::FbxData::ReadDeformers(const Reading::FbxObject& objectsObject)
{
	const std::vector<Reading::FbxObject*> deformers{ objectsObject.GetChildren("Deformer") };
	m_Deformers = { static_cast<int>(deformers.size()) };
	for (int i = 0; i < m_Deformers.GetSize(); i++)
		m_Deformers[i] = Deformer{ *deformers[i] };
}

void Io::Fbx::Wrapping::FbxData::ReadPoses(const Reading::FbxObject& objectsObject)
{
	const std::vector<Reading::FbxObject*> poses{ objectsObject.GetChildren("Pose") };
	if (poses.size() > 1) Logger::PrintWarning("Doesn't support multiple poses");
	if (poses.size() > 0)
		m_BindPose = Pose{ *poses[0] };
}

void Io::Fbx::Wrapping::FbxData::ReadConnections(const Reading::FbxObject& connectionsObject)
{
	const std::vector<Reading::FbxObject*> readerConnections{ connectionsObject.GetChildren() };
	m_Connections = { readerConnections.size() };
	for (int i = 0; i < readerConnections.size(); i++)
		m_Connections[i] = Connection{ *readerConnections[i] };
}

void Io::Fbx::Wrapping::FbxData::ReadAnimationStack(const Reading::FbxObject& objectsObject)
{
	const std::vector<Reading::FbxObject*> readerAnimationStacks{ objectsObject.GetChildren("AnimationStack") };
	if (readerAnimationStacks.size() > 1)
		Logger::PrintWarning("Multiple animation-stacks not supported");
	if (readerAnimationStacks.size() > 0)
		m_AnimationStack = AnimationStack{ *readerAnimationStacks[0] };
}

void Io::Fbx::Wrapping::FbxData::ReadAnimationLayers(const Reading::FbxObject& objectsObject)
{
	const std::vector<Reading::FbxObject*> readerAnimationLayers{ objectsObject.GetChildren("AnimationLayer") };
	if (readerAnimationLayers.empty())return;
	m_AnimationLayers = { readerAnimationLayers.size() };
	for (int i = 0; i < m_AnimationLayers.GetSize(); i++)
		m_AnimationLayers[i] = AnimationLayer{ *readerAnimationLayers[i] };
}

void Io::Fbx::Wrapping::FbxData::ReadAnimationCurveNodes(const Reading::FbxObject& objectsObject)
{
	const std::vector<Reading::FbxObject*> curveNodeObjects{ objectsObject.GetChildren("AnimationCurveNode") };
	m_AnimationCurveNodes = { curveNodeObjects.size() };
	for (int i = 0; i < m_AnimationCurveNodes.GetSize(); i++)
		m_AnimationCurveNodes[i] = AnimationCurveNode{ *curveNodeObjects[i] };
}

void Io::Fbx::Wrapping::FbxData::ReadAnimationCurves(const Reading::FbxObject& objectsObject)
{
	const std::vector<Reading::FbxObject*> objects{ objectsObject.GetChildren("AnimationCurve") };
	m_AnimationCurves = { objects.size() };
	for (int i = 0; i < m_AnimationCurves.GetSize(); i++)
		m_AnimationCurves[i] = AnimationCurve{ *objects[i] };
}

void Io::Fbx::Wrapping::FbxData::HandleConnections()
{
	for (int iConnection = 0; iConnection < m_Connections.GetSize(); iConnection++)
	{
		const Connection& connection{ m_Connections[iConnection] };

		Geometry* pGeometry{ FindGeometry(connection.ChildId) };
		if (pGeometry)
		{
			HandleGeometryConnection(*pGeometry, connection);
			continue;
		}

		Model* pModel{ FindModel(connection.ChildId) };
		if (pModel)
		{
			HandleModelConnection(*pModel, connection);
			continue;
		}

		Deformer* pDeformer{ FindDeformer(connection.ChildId) };
		if(pDeformer)
		{
			HandleDeformerConnection(*pDeformer, connection);
			continue;
		}

		PrintUnhandledConnectionError(FindTypeName(connection.ParentId), FindTypeName(connection.ChildId));;
	}
}

void Io::Fbx::Wrapping::FbxData::HandleGeometryConnection(Geometry& geometry, const Connection& connection)
{
	Model* pModel{ FindModel(connection.ParentId) };
	if (pModel)
	{
		geometry.SetRootModel(*pModel);
		return;
	}

	PrintUnhandledConnectionError(FindTypeName(connection.ParentId), "Geometry");
}

void Io::Fbx::Wrapping::FbxData::HandleModelConnection(Model& childModel, const Connection& connection)
{
	if (connection.ParentId == 0) return;

	Model* pModel{ FindModel(connection.ParentId) };
	if (pModel)
	{
		childModel.SetParentModel(*pModel);
		pModel->AddChildModel(childModel);
		return;
	}

	Deformer* pDeformer{ FindDeformer(connection.ParentId) };
	if (pDeformer)
	{
		childModel.AddDeformer(*pDeformer);
		return;
	}

	PrintUnhandledConnectionError(FindTypeName(connection.ParentId), "Model");
}

void Io::Fbx::Wrapping::FbxData::HandleDeformerConnection(Deformer& childDeformer, const Connection& connection)
{
	Deformer* pParentDeformer{ FindDeformer(connection.ParentId) };
	if(pParentDeformer)
	{
		childDeformer.SetParentDeformer(*pParentDeformer);
		pParentDeformer->AddChildDeformer(childDeformer);
		return;
	}

	PrintUnhandledConnectionError(FindTypeName(connection.ParentId), "Deformer");
}

std::string Io::Fbx::Wrapping::FbxData::FindTypeName(const int64_t& id) const
{
	if (FindGeometry(id)) return "Geometry";
	if (FindModel(id)) return "Model";
	if (FindDeformer(id)) return "Deformer";
	if (FindPoseNode(id)) return "PoseNode";
	if (FindAnimationLayer(id)) return "AnimationLayer";
	if (FindAnimationCurve(id)) return "AnimationCurve";
	if (FindAnimationCurveNode(id)) return "AnimationCurveNode";
	if (m_AnimationStack.Id == id) return "AnimationStack";
	return "Unknown type";
}

void Io::Fbx::Wrapping::FbxData::PrintUnhandledConnectionError(const std::string& parentType,
	const std::string& childType)
{
	Logger::PrintError(childType + " has an unsupported connection to a parent " + parentType);
}
