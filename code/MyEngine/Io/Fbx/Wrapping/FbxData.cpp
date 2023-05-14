#include "pch.h"
#include "FbxData.h"

#include "../Reading/FbxObject.h"
#include "../Reading/FbxReader.h"
#include "Debug/DebugRenderer.h"
#include "Game/Transform.h"

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
	MakeLinks();
	TempDisplayLimbNodes();
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

Io::Fbx::Wrapping::Model* Io::Fbx::Wrapping::FbxData::FindModel(const int64_t& id)
{
	for (int i = 0; i < m_Models.GetSize(); i++)
		if (m_Models[i].GetId() == id)
			return &m_Models[i];
	return nullptr;
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
	{
		m_BindPose = Pose{ *poses[0] };
		for (int i = 0; i < m_BindPose.Nodes.GetSize(); i++)
		{
			Model* pModel{ FindModel(m_BindPose.Nodes[i].Id) };
			if (!pModel)
			{
				Logger::PrintError("Model for PoseNode not found");
				continue;
			}
			pModel->SetBindPose(m_BindPose.Nodes[i]);
		}
	}
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

void Io::Fbx::Wrapping::FbxData::MakeLinks()
{
	for (int i = 0; i < m_Connections.GetSize(); i++)
	{
		Connection& connection{ m_Connections[i] };

		std::cout << i << ": " << connection.Relation << ", " << connection.Property << std::endl;
		Model* pModel{ FindModel(connection.Id) };
		if (!pModel)
			continue;
		if (connection.ParentId != 0)
		{
			Model* pParentModel{ FindModel(connection.ParentId) };
			if (!pParentModel)
				continue;
			if (pParentModel == pModel)
			{
				Logger::PrintError("Parent is same as child");
				continue;
			}
			connection.pParent = pParentModel;
		}
		pModel->AddConnection(connection);
	}
}

void Io::Fbx::Wrapping::FbxData::TempDisplayLimbNodes() const
{
	for (int i = 0; i < m_Models.GetSize(); i++)
	{
		const Model* pModel{ &m_Models[i] };
		if (pModel->GetTypeName() != "LimbNode") continue;

		/*std::stringstream ss{  };
		ss << "--- " + pModel->GetName() + " ---\n";
		ss << "PreRotation: " << ToString::Convert(pModel->GetPreRotation()) << "\n";
		ss << "PostRotation: " << ToString::Convert(pModel->GetPostRotation()) << "\n";
		ss << "LclTranslation: " << ToString::Convert(pModel->GetLclTranslation()) << "\n";
		ss << "LclRotation: " << ToString::Convert(pModel->GetLclRotation()) << "\n";
		ss << "LclScaling: " << ToString::Convert(pModel->GetLclScaling()) << "\n";
		ss << "RotationOffset: " << ToString::Convert(pModel->GetRotationOffset()) << "\n";
		ss << "RotationPivot: " << ToString::Convert(pModel->GetRotationPivot()) << "\n";
		ss << "ScalingPivot: " << ToString::Convert(pModel->GetScalingPivot()) << "\n";
		ss << "ScalingMax: " << ToString::Convert(pModel->GetScalingMax()) << "\n";
		ss << "InheritType: " << ToString::Convert(pModel->GetInheritType()) << "\n";
		ss << "NrConnections: " << pModel->GetConnections().GetSize() << "\n";
		const Connection& baseConnection{ pModel->GetConnections()[0] };
		if (baseConnection.pParent)
			ss << "Parent: " << baseConnection.pParent->GetName() << std::endl;
		std::cout << ss.str();*/

		//using model-data
		Game::Transform transform{};
		while (pModel)
		{
			const Float3 translation{ pModel->GetLclTranslation() };
			const Quaternion preRotation{ Quaternion::FromEulerDegrees(pModel->GetPreRotation()) };

			const Float3& postEulers{ pModel->GetPostRotation() };
			const Quaternion postRotationX{ Quaternion::FromEulerDegrees({postEulers.x, 0, 0}) };
			const Quaternion postRotationY{ Quaternion::FromEulerDegrees({0, postEulers.y, 0}) };
			const Quaternion postRotationZ{ Quaternion::FromEulerDegrees({0, 0, postEulers.z}) };

			Quaternion rotation{ postRotationZ };
			rotation.RotateBy(postRotationY);
			rotation.RotateBy(postRotationX);
			rotation.RotateBy(preRotation);

			transform = Game::Transform::LocalToWorld(transform, { translation, rotation });

			const Connection& modelConnection{ pModel->GetConnections()[0] };
			pModel = modelConnection.pParent;
		}
		DebugRenderer::AddSphere(transform.Position * 0.01, { 0,0,1 }, .1f);
	}
}
