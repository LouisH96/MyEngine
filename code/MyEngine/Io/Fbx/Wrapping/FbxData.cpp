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

	//reader.GetRoot().Print();

	//Geometry
	const std::vector<Reading::FbxObject*> geometries{ objects.GetChildren("Geometry") };
	m_Geometries = { static_cast<int>(geometries.size()) };
	for (int i = 0; i < geometries.size(); i++)
		m_Geometries[i] = Geometry{ *geometries[i] };

	//Models
	const std::vector<Reading::FbxObject*> models{ objects.GetChildren("Model") };
	m_Models = { static_cast<int>(models.size()) };
	for (int i = 0; i < m_Models.GetSize(); i++)
		m_Models[i] = Model{ *models[i] };

	//Poses
	const std::vector<Reading::FbxObject*> poses{ objects.GetChildren("Pose") };
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

	//Connections
	const std::vector<Reading::FbxObject*> readerConnections{ reader.GetRoot().GetChild("Connections")->GetChildren() };
	for (int i = 0; i < readerConnections.size(); i++)
	{
		Connection connection{ *readerConnections[i] };
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

	//temp - display spheres on limb-nodes
	for (int i = 0; i < m_Models.GetSize(); i++)
	{
		Model* pModel{ &m_Models[i] };
		if (pModel->GetTypeName() != "LimbNode") continue;

		/*std::stringstream ss{  };
		ss << "--- " + pModel->GetName() + " ---\n";
		ss << "PreRotation: " << ToString::Convert(pModel->GetPreRotation()) << "\n";
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
			const Game::Transform preRotation{ {}, Quaternion::FromEulerDegrees(pModel->GetPreRotation()) };
			const Game::Transform translation{ pModel->GetLclTranslation() , {} };

			transform = Game::Transform::LocalToWorld(transform, preRotation);
			transform = Game::Transform::LocalToWorld(transform, translation);

			const Connection& modelConnection{ pModel->GetConnections()[0] };
			pModel = modelConnection.pParent;
		}
		DebugRenderer::AddSphere(transform.Position * 0.01, { 0,0,1 }, .05f);
	}
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
