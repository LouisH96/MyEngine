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
			if (connection.Id != model.GetId()) continue;
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
		const Model* pChild{ FindModel(connection.Id) };
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

		Geometry* pGeometry{ FindGeometry(connection.Id) };
		if(pGeometry)
		{
			HandleGeometryConnection(*pGeometry, connection);
			continue;
		}

		Logger::PrintWarning("Connection child not found");
	}
}

void Io::Fbx::Wrapping::FbxData::HandleGeometryConnection(Geometry& geometry, const Connection& connection)
{
	Model* pModel{ FindModel(connection.ParentId) };
	if(pModel)
	{
		geometry.AddParentModel(*pModel);
		return;
	}

	Logger::PrintWarning("Geometry connection not found");
}
