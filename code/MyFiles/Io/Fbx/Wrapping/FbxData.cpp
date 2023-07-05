#include "FbxData.h"

#include "Video.h"
#include "../Reading/FbxObject.h"
#include "../Reading/FbxReader.h"
#include "Logger/Logger.h"

MyEngine::Io::Fbx::Wrapping::FbxData::FbxData(const std::wstring& path)
	: FbxData{ Reading::FbxReader{ path } }
{}

MyEngine::Io::Fbx::Wrapping::FbxData::FbxData(Reading::FbxReader&& reader)
{
	const Reading::FbxObject& objects{ *reader.GetRoot().GetChild("Objects") };
	const Reading::FbxObject& connections{ *reader.GetRoot().GetChild("Connections") };

	//reader.GetRoot().Print();

	ReadGeometry(objects);
	ReadModels(objects);
	ReadNodeAttributes(objects);
	ReadDeformers(objects);
	ReadPoses(objects);
	ReadConnections(connections);
	ReadAnimationStack(objects);
	ReadAnimationLayers(objects);
	ReadAnimationCurveNodes(objects);
	ReadAnimationCurves(objects);
	ReadTextures(objects);
	ReadVideos(objects);
	ReadMaterial(objects);
	ReadCollections(objects);

	HandleConnections();
}

MyEngine::Array<MyEngine::Io::Fbx::Wrapping::Model> MyEngine::Io::Fbx::Wrapping::FbxData::GetModelsOfType(const std::string& typeName) const
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

MyEngine::Array<MyEngine::Io::Fbx::Wrapping::Model> MyEngine::Io::Fbx::Wrapping::FbxData::GetLimbNodes() const
{
	return GetModelsOfType("LimbNode");
}

const MyEngine::Io::Fbx::Wrapping::Model* MyEngine::Io::Fbx::Wrapping::FbxData::GetARootLimbNode() const
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

MyEngine::Io::Fbx::Wrapping::Geometry* MyEngine::Io::Fbx::Wrapping::FbxData::FindGeometry(const int64_t& id)
{
	for (int i = 0; i < m_Geometries.GetSize(); i++)
		if (m_Geometries[i].GetId() == id)
			return &m_Geometries[i];
	return nullptr;
}

MyEngine::Io::Fbx::Wrapping::Model* MyEngine::Io::Fbx::Wrapping::FbxData::FindModel(const int64_t& id)
{
	for (int i = 0; i < m_Models.GetSize(); i++)
		if (m_Models[i].GetId() == id)
			return &m_Models[i];
	return nullptr;
}

MyEngine::Io::Fbx::Wrapping::NodeAttribute* MyEngine::Io::Fbx::Wrapping::FbxData::FindNodeAttribute(const int64_t& id)
{
	for (int i = 0; i < m_NodeAttributes.GetSize(); i++)
		if (m_NodeAttributes[i].Id == id)
			return &m_NodeAttributes[i];
	return nullptr;
}

MyEngine::Io::Fbx::Wrapping::Deformer* MyEngine::Io::Fbx::Wrapping::FbxData::FindDeformer(const int64_t& id)
{
	for (int i = 0; i < m_Deformers.GetSize(); i++)
		if (m_Deformers[i].Id == id)
			return &m_Deformers[i];
	return nullptr;
}

MyEngine::Io::Fbx::Wrapping::Pose::Node* MyEngine::Io::Fbx::Wrapping::FbxData::FindPoseNode(const int64_t& id)
{
	for (int i = 0; i < m_BindPose.Nodes.GetSize(); i++)
		if (m_BindPose.Nodes[i].Id == id)
			return &m_BindPose.Nodes[i];
	return nullptr;
}

MyEngine::Io::Fbx::Wrapping::AnimationStack* MyEngine::Io::Fbx::Wrapping::FbxData::FindAnimationStack(const int64_t& id)
{
	for (int i = 0; i < m_AnimationStacks.GetSize(); i++)
		if (m_AnimationStacks[i].Id == id)
			return &m_AnimationStacks[i];
	return nullptr;
}

MyEngine::Io::Fbx::Wrapping::AnimationLayer* MyEngine::Io::Fbx::Wrapping::FbxData::FindAnimationLayer(const int64_t& id)
{
	for (int i = 0; i < m_AnimationLayers.GetSize(); i++)
		if (m_AnimationLayers[i].Id == id)
			return &m_AnimationLayers[i];
	return nullptr;
}

MyEngine::Io::Fbx::Wrapping::AnimationCurve* MyEngine::Io::Fbx::Wrapping::FbxData::FindAnimationCurve(const int64_t& id)
{
	for (int i = 0; i < m_AnimationCurves.GetSize(); i++)
		if (m_AnimationCurves[i].Id == id)
			return &m_AnimationCurves[i];
	return nullptr;
}

MyEngine::Io::Fbx::Wrapping::AnimationCurveNode* MyEngine::Io::Fbx::Wrapping::FbxData::FindAnimationCurveNode(const int64_t& id)
{
	for (int i = 0; i < m_AnimationCurveNodes.GetSize(); i++)
		if (m_AnimationCurveNodes[i].Id == id)
			return &m_AnimationCurveNodes[i];
	return nullptr;
}

MyEngine::Io::Fbx::Wrapping::Video* MyEngine::Io::Fbx::Wrapping::FbxData::FindVideo(const int64_t& id)
{
	for (int i = 0; i < m_Videos.GetSize(); i++)
		if (m_Videos[i].Id == id)
			return &m_Videos[i];
	return nullptr;
}

MyEngine::Io::Fbx::Wrapping::FbxWrapTexture* MyEngine::Io::Fbx::Wrapping::FbxData::FindTexture(const int64_t& id)
{
	for (int i = 0; i < m_Textures.GetSize(); i++)
		if (m_Textures[i].Id == id)
			return &m_Textures[i];
	return nullptr;
}

MyEngine::Io::Fbx::Wrapping::FbxWrapMaterial* MyEngine::Io::Fbx::Wrapping::FbxData::FindMaterial(const int64_t& id)
{
	for (int i = 0; i < m_Materials.GetSize(); i++)
		if (m_Materials[i].Id == id)
			return &m_Materials[i];
	return nullptr;
}

MyEngine::Io::Fbx::Wrapping::CollectionExclusive* MyEngine::Io::Fbx::Wrapping::FbxData::FindCollection(const int64_t& id)
{
	for (int i = 0; i < m_Collections.GetSize(); i++)
		if (m_Collections[i].Id == id)
			return &m_Collections[i];
	return nullptr;
}

const MyEngine::Io::Fbx::Wrapping::Geometry* MyEngine::Io::Fbx::Wrapping::FbxData::FindGeometry(const int64_t& id) const
{
	for (int i = 0; i < m_Geometries.GetSize(); i++)
		if (m_Geometries[i].GetId() == id)
			return &m_Geometries[i];
	return nullptr;
}

const MyEngine::Io::Fbx::Wrapping::Deformer* MyEngine::Io::Fbx::Wrapping::FbxData::FindDeformer(const int64_t& id) const
{
	for (int i = 0; i < m_Deformers.GetSize(); i++)
		if (m_Deformers[i].Id == id)
			return &m_Deformers[i];
	return nullptr;
}

const MyEngine::Io::Fbx::Wrapping::Pose::Node* MyEngine::Io::Fbx::Wrapping::FbxData::FindPoseNode(const int64_t& id) const
{
	for (int i = 0; i < m_BindPose.Nodes.GetSize(); i++)
		if (m_BindPose.Nodes[i].Id == id)
			return &m_BindPose.Nodes[i];
	return nullptr;
}

const MyEngine::Io::Fbx::Wrapping::AnimationStack* MyEngine::Io::Fbx::Wrapping::FbxData::FindAnimationStack(const int64_t& id) const
{
	for (int i = 0; i < m_AnimationStacks.GetSize(); i++)
		if (m_AnimationStacks[i].Id == id)
			return &m_AnimationStacks[i];
	return nullptr;
}

const MyEngine::Io::Fbx::Wrapping::AnimationLayer* MyEngine::Io::Fbx::Wrapping::FbxData::FindAnimationLayer(const int64_t& id) const
{
	for (int i = 0; i < m_AnimationLayers.GetSize(); i++)
		if (m_AnimationLayers[i].Id == id)
			return &m_AnimationLayers[i];
	return nullptr;
}

const MyEngine::Io::Fbx::Wrapping::AnimationCurve* MyEngine::Io::Fbx::Wrapping::FbxData::FindAnimationCurve(const int64_t& id) const
{
	for (int i = 0; i < m_AnimationCurves.GetSize(); i++)
		if (m_AnimationCurves[i].Id == id)
			return &m_AnimationCurves[i];
	return nullptr;
}

const MyEngine::Io::Fbx::Wrapping::Video* MyEngine::Io::Fbx::Wrapping::FbxData::FindVideo(const int64_t& id) const
{
	for (int i = 0; i < m_Videos.GetSize(); i++)
		if (m_Videos[i].Id == id)
			return &m_Videos[i];
	return nullptr;
}

const MyEngine::Io::Fbx::Wrapping::FbxWrapTexture* MyEngine::Io::Fbx::Wrapping::FbxData::FindTexture(const int64_t& id) const
{
	for (int i = 0; i < m_Textures.GetSize(); i++)
		if (m_Textures[i].Id == id)
			return &m_Textures[i];
	return nullptr;
}

const MyEngine::Io::Fbx::Wrapping::FbxWrapMaterial* MyEngine::Io::Fbx::Wrapping::FbxData::FindMaterial(const int64_t& id) const
{
	for (int i = 0; i < m_Materials.GetSize(); i++)
		if (m_Materials[i].Id == id)
			return &m_Materials[i];
	return nullptr;
}

const MyEngine::Io::Fbx::Wrapping::CollectionExclusive* MyEngine::Io::Fbx::Wrapping::FbxData::FindCollection(const int64_t& id) const
{
	for (int i = 0; i < m_Collections.GetSize(); i++)
		if (m_Collections[i].Id == id)
			return &m_Collections[i];
	return nullptr;
}

const MyEngine::Io::Fbx::Wrapping::AnimationCurveNode* MyEngine::Io::Fbx::Wrapping::FbxData::FindAnimationCurveNode(const int64_t& id) const
{
	for (int i = 0; i < m_AnimationCurveNodes.GetSize(); i++)
		if (m_AnimationCurveNodes[i].Id == id)
			return &m_AnimationCurveNodes[i];
	return nullptr;
}

const MyEngine::Io::Fbx::Wrapping::Model* MyEngine::Io::Fbx::Wrapping::FbxData::FindModel(const int64_t& id) const
{
	for (int i = 0; i < m_Models.GetSize(); i++)
		if (m_Models[i].GetId() == id)
			return &m_Models[i];
	return nullptr;
}

const MyEngine::Io::Fbx::Wrapping::NodeAttribute* MyEngine::Io::Fbx::Wrapping::FbxData::FindNodeAttribute(const int64_t& id) const
{
	for (int i = 0; i < m_NodeAttributes.GetSize(); i++)
		if (m_NodeAttributes[i].Id == id)
			return &m_NodeAttributes[i];
	return nullptr;
}

MyEngine::Array<const MyEngine::Io::Fbx::Wrapping::Model*> MyEngine::Io::Fbx::Wrapping::FbxData::GetChildren(const Model& model) const
{
	return GetChildren(model.GetId());
}

MyEngine::Array<const MyEngine::Io::Fbx::Wrapping::Model*> MyEngine::Io::Fbx::Wrapping::FbxData::GetChildren(const int64_t& id) const
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

void MyEngine::Io::Fbx::Wrapping::FbxData::ReadGeometry(const Reading::FbxObject& objectsObject)
{
	const std::vector<Reading::FbxObject*> geometries{ objectsObject.GetChildren("Geometry") };
	m_Geometries = { static_cast<int>(geometries.size()) };
	for (int i = 0; i < geometries.size(); i++)
		m_Geometries[i] = Geometry{ *geometries[i] };
}

void MyEngine::Io::Fbx::Wrapping::FbxData::ReadModels(const Reading::FbxObject& objectsObject)
{
	const std::vector<Reading::FbxObject*> models{ objectsObject.GetChildren("Model") };
	m_Models = { static_cast<int>(models.size()) };
	for (int i = 0; i < m_Models.GetSize(); i++)
		m_Models[i] = Model{ *models[i] };
}

void MyEngine::Io::Fbx::Wrapping::FbxData::ReadNodeAttributes(const Reading::FbxObject& objectsObject)
{
	const std::vector<Reading::FbxObject*> nodeAttributes{ objectsObject.GetChildren("NodeAttribute") };
	m_NodeAttributes = { static_cast<int>(nodeAttributes.size()) };
	for (int i = 0; i < m_NodeAttributes.GetSize(); i++)
		m_NodeAttributes[i] = NodeAttribute{ *nodeAttributes[i] };
}

void MyEngine::Io::Fbx::Wrapping::FbxData::ReadDeformers(const Reading::FbxObject& objectsObject)
{
	const std::vector<Reading::FbxObject*> deformers{ objectsObject.GetChildren("Deformer") };
	m_Deformers = { static_cast<int>(deformers.size()) };
	for (int i = 0; i < m_Deformers.GetSize(); i++)
		m_Deformers[i] = Deformer{ *deformers[i] };
}

void MyEngine::Io::Fbx::Wrapping::FbxData::ReadPoses(const Reading::FbxObject& objectsObject)
{
	const std::vector<Reading::FbxObject*> poses{ objectsObject.GetChildren("Pose") };
	if (poses.size() > 1) Logger::PrintWarning("Doesn't support multiple poses");
	if (poses.size() > 0)
		m_BindPose = Pose{ *poses[0] };
}

void MyEngine::Io::Fbx::Wrapping::FbxData::ReadConnections(const Reading::FbxObject& connectionsObject)
{
	const std::vector<Reading::FbxObject*> readerConnections{ connectionsObject.GetChildren() };
	m_Connections = { readerConnections.size() };
	for (int i = 0; i < readerConnections.size(); i++)
		m_Connections[i] = Connection{ *readerConnections[i] };
}

void MyEngine::Io::Fbx::Wrapping::FbxData::ReadAnimationStack(const Reading::FbxObject& objectsObject)
{
	const std::vector<Reading::FbxObject*> readerAnimationStacks{ objectsObject.GetChildren("AnimationStack") };
	m_AnimationStacks = { readerAnimationStacks.size() };
	for (int i = 0; i < readerAnimationStacks.size(); i++)
		m_AnimationStacks[i] = AnimationStack{ *readerAnimationStacks[i] };
}

void MyEngine::Io::Fbx::Wrapping::FbxData::ReadAnimationLayers(const Reading::FbxObject& objectsObject)
{
	const std::vector<Reading::FbxObject*> readerAnimationLayers{ objectsObject.GetChildren("AnimationLayer") };
	if (readerAnimationLayers.empty())return;
	m_AnimationLayers = { readerAnimationLayers.size() };
	for (int i = 0; i < m_AnimationLayers.GetSize(); i++)
		m_AnimationLayers[i] = AnimationLayer{ *readerAnimationLayers[i] };
}

void MyEngine::Io::Fbx::Wrapping::FbxData::ReadAnimationCurveNodes(const Reading::FbxObject& objectsObject)
{
	const std::vector<Reading::FbxObject*> curveNodeObjects{ objectsObject.GetChildren("AnimationCurveNode") };
	m_AnimationCurveNodes = { curveNodeObjects.size() };
	for (int i = 0; i < m_AnimationCurveNodes.GetSize(); i++)
		m_AnimationCurveNodes[i] = AnimationCurveNode{ *curveNodeObjects[i] };
}

void MyEngine::Io::Fbx::Wrapping::FbxData::ReadAnimationCurves(const Reading::FbxObject& objectsObject)
{
	const std::vector<Reading::FbxObject*> objects{ objectsObject.GetChildren("AnimationCurve") };
	m_AnimationCurves = { objects.size() };
	for (int i = 0; i < m_AnimationCurves.GetSize(); i++)
		m_AnimationCurves[i] = AnimationCurve{ *objects[i] };
}

void MyEngine::Io::Fbx::Wrapping::FbxData::ReadTextures(const Reading::FbxObject& objectsObject)
{
	const std::vector<Reading::FbxObject*> objects{ objectsObject.GetChildren("Texture") };
	m_Textures = { objects.size() };
	for (int i = 0; i < m_Textures.GetSize(); i++)
		m_Textures[i] = FbxWrapTexture{ *objects[i] };
}

void MyEngine::Io::Fbx::Wrapping::FbxData::ReadVideos(const Reading::FbxObject& objectsObject)
{
	const std::vector<Reading::FbxObject*> objects{ objectsObject.GetChildren("Video") };
	m_Videos = { objects.size() };
	for (int i = 0; i < m_Videos.GetSize(); i++)
		m_Videos[i] = Video{ *objects[i] };
}

void MyEngine::Io::Fbx::Wrapping::FbxData::ReadMaterial(const Reading::FbxObject& objectsObject)
{
	const std::vector<Reading::FbxObject*> objects{ objectsObject.GetChildren("Material") };
	m_Materials = { objects.size() };
	for (int i = 0; i < m_Materials.GetSize(); i++)
		m_Materials[i] = FbxWrapMaterial{ *objects[i] };
}

void MyEngine::Io::Fbx::Wrapping::FbxData::ReadCollections(const Reading::FbxObject& objectsObject)
{
	const std::vector<Reading::FbxObject*> objects{ objectsObject.GetChildren("CollectionExclusive") };
	m_Collections = { objects.size() };
	for (int i = 0; i < m_Collections.GetSize(); i++)
		m_Collections[i] = CollectionExclusive{ *objects[i] };
}

void MyEngine::Io::Fbx::Wrapping::FbxData::HandleConnections()
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

		NodeAttribute* pNodeAttribute{ FindNodeAttribute(connection.ChildId) };
		if (pNodeAttribute)
		{
			HandleNodeAttributeConnection(*pNodeAttribute, connection);
			continue;
		}

		Deformer* pDeformer{ FindDeformer(connection.ChildId) };
		if (pDeformer)
		{
			HandleDeformerConnection(*pDeformer, connection);
			continue;
		}

		AnimationLayer* pAnimationLayer{ FindAnimationLayer(connection.ChildId) };
		if (pAnimationLayer)
		{
			HandleAnimationLayerConnection(*pAnimationLayer, connection);
			continue;
		}

		AnimationCurve* pAnimationCurve{ FindAnimationCurve(connection.ChildId) };
		if (pAnimationCurve)
		{
			HandleAnimationCurveConnection(*pAnimationCurve, connection);
			continue;
		}

		AnimationCurveNode* pAnimationCurveNode{ FindAnimationCurveNode(connection.ChildId) };
		if (pAnimationCurveNode)
		{
			HandleAnimationCurveNodeConnection(*pAnimationCurveNode, connection);
			continue;
		}

		Video* pVideo{ FindVideo(connection.ChildId) };
		if (pVideo)
		{
			HandleVideoConnection(*pVideo, connection);
			continue;
		}

		FbxWrapTexture* pTexture{ FindTexture(connection.ChildId) };
		if (pTexture)
		{
			HandleTextureConnection(*pTexture, connection);
			continue;
		}

		FbxWrapMaterial* pMaterial{ FindMaterial(connection.ChildId) };
		if (pMaterial)
		{
			HandleMaterialConnection(*pMaterial, connection);
			continue;
		}

		PrintUnhandledConnectionError(FindTypeName(connection.ParentId), connection.ParentId, FindTypeName(connection.ChildId), connection.ChildId);;
		//PrintUnhandledConnectionError(FindTypeName(connection.ParentId), FindTypeName(connection.ChildId));;
	}
}

void MyEngine::Io::Fbx::Wrapping::FbxData::HandleGeometryConnection(Geometry& geometry, const Connection& connection)
{
	Model* pModel{ FindModel(connection.ParentId) };
	if (pModel)
	{
		geometry.SetRootModel(*pModel);
		return;
	}

	PrintUnhandledConnectionError(FindTypeName(connection.ParentId), "Geometry");
}

void MyEngine::Io::Fbx::Wrapping::FbxData::HandleModelConnection(Model& childModel, const Connection& connection)
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

	CollectionExclusive* pCollection{ FindCollection(connection.ParentId) };
	if (pCollection)
	{
		childModel.SetCollection(*pCollection);
		pCollection->AddModel(childModel);
		return;
	}

	PrintUnhandledConnectionError(FindTypeName(connection.ParentId), connection.ParentId, "Model", connection.ChildId);
}

void MyEngine::Io::Fbx::Wrapping::FbxData::HandleNodeAttributeConnection(NodeAttribute& nodeAttribute,
	const Connection& connection)
{
	Model* pModel{ FindModel(connection.ParentId) };
	if (pModel)
	{
		nodeAttribute.SetParentModel(*pModel);
		pModel->SetNodeAttribute(nodeAttribute);
		return;
	}

	PrintUnhandledConnectionError(FindTypeName(connection.ParentId), "NodeAttribute");
}

void MyEngine::Io::Fbx::Wrapping::FbxData::HandleDeformerConnection(Deformer& childDeformer, const Connection& connection)
{
	Deformer* pParentDeformer{ FindDeformer(connection.ParentId) };
	if (pParentDeformer)
	{
		childDeformer.SetParentDeformer(*pParentDeformer);
		pParentDeformer->AddChildDeformer(childDeformer);
		return;
	}

	Geometry* pGeometry{ FindGeometry(connection.ParentId) };
	if (pGeometry)
	{
		childDeformer.SetParentGeometry(*pGeometry);
		pGeometry->AddDeformer(childDeformer);
		return;
	}

	PrintUnhandledConnectionError(FindTypeName(connection.ParentId), "Deformer");
}

void MyEngine::Io::Fbx::Wrapping::FbxData::HandleAnimationLayerConnection(AnimationLayer& animationLayer,
	const Connection& connection)
{
	AnimationStack* pAnimationStack{ FindAnimationStack(connection.ParentId) };
	if (pAnimationStack)
	{
		animationLayer.SetAnimationStack(*pAnimationStack);
		pAnimationStack->AddAnimationLayer(animationLayer);
		return;
	}

	PrintUnhandledConnectionError(FindTypeName(connection.ParentId), "AnimationLayer");
}

void MyEngine::Io::Fbx::Wrapping::FbxData::HandleAnimationCurveConnection(AnimationCurve& animationCurve, const Connection& connection)
{
	AnimationCurveNode* pAnimationCurveNode{ FindAnimationCurveNode(connection.ParentId) };
	if (pAnimationCurveNode)
	{
		pAnimationCurveNode->AddAnimationCurve(animationCurve);
		animationCurve.SetAnimationCurveNode(*pAnimationCurveNode);
		return;
	}

	PrintUnhandledConnectionError(FindTypeName(connection.ParentId), "AnimationCurve");
}

void MyEngine::Io::Fbx::Wrapping::FbxData::HandleAnimationCurveNodeConnection(AnimationCurveNode& childAnimationCurveNode,
	const Connection& connection)
{
	Model* pModel{ FindModel(connection.ParentId) };
	if (pModel)
	{
		childAnimationCurveNode.SetParentModel(*pModel);
		pModel->AddAnimationCurveNode(childAnimationCurveNode);
		return;
	}

	AnimationLayer* pAnimationLayer{ FindAnimationLayer(connection.ParentId) };
	if (pAnimationLayer)
	{
		childAnimationCurveNode.SetAnimationLayer(*pAnimationLayer);
		pAnimationLayer->AddAnimationCurveNode(childAnimationCurveNode);
		return;
	}

	NodeAttribute* pNodeAttribute{ FindNodeAttribute(connection.ParentId) };
	if (pNodeAttribute)
	{
		childAnimationCurveNode.AddNodeAttribute(*pNodeAttribute);
		pNodeAttribute->AddAnimationCurveNode(childAnimationCurveNode);
		return;
	}

	PrintUnhandledConnectionError(FindTypeName(connection.ParentId), "AnimationCurveNode");
}

void MyEngine::Io::Fbx::Wrapping::FbxData::HandleVideoConnection(Video& video, const Connection& connection)
{
	FbxWrapTexture* pTexture{ FindTexture(connection.ParentId) };
	if (pTexture)
	{
		video.AddTexture(*pTexture);
		pTexture->AddLinkedVideo(video);
		return;
	}

	PrintUnhandledConnectionError(FindTypeName(connection.ParentId), "Video");
}

void MyEngine::Io::Fbx::Wrapping::FbxData::HandleTextureConnection(FbxWrapTexture& texture, const Connection& connection)
{
	FbxWrapMaterial* pMaterial{ FindMaterial(connection.ParentId) };
	if (pMaterial)
	{
		pMaterial->AddTexture(texture);
		texture.AddMaterial(*pMaterial);
		return;
	}

	PrintUnhandledConnectionError(FindTypeName(connection.ParentId), "Texture");
}

void MyEngine::Io::Fbx::Wrapping::FbxData::HandleMaterialConnection(FbxWrapMaterial& material, const Connection& connection)
{
	Model* pModel{ FindModel(connection.ParentId) };
	if (pModel)
	{
		pModel->AddMaterial(material);
		material.AddModel(*pModel);
		return;
	}

	PrintUnhandledConnectionError(FindTypeName(connection.ParentId), "Material");
}

std::string MyEngine::Io::Fbx::Wrapping::FbxData::FindTypeName(const int64_t& id) const
{
	if (FindAnimationStack(id)) return "AnimationStack";
	if (FindGeometry(id)) return "Geometry";
	if (FindModel(id)) return "Model";
	if (FindNodeAttribute(id)) return "NodeAttribute";
	if (FindDeformer(id)) return "Deformer";
	if (FindPoseNode(id)) return "PoseNode";
	if (FindAnimationLayer(id)) return "AnimationLayer";
	if (FindAnimationCurve(id)) return "AnimationCurve";
	if (FindAnimationCurveNode(id)) return "AnimationCurveNode";
	if (FindVideo(id)) return "Video";
	if (FindTexture(id)) return "Texture";
	if (FindMaterial(id)) return "Material";
	return "Unknown type";
}

void MyEngine::Io::Fbx::Wrapping::FbxData::PrintUnhandledConnectionError(const std::string& parentType,
	const std::string& childType)
{
	Logger::PrintError(childType + " has an unsupported connection to a parent " + parentType);
}

void MyEngine::Io::Fbx::Wrapping::FbxData::PrintUnhandledConnectionError(const std::string& parentType, const int64_t& parentId,
	const std::string& childType, const int64_t& childId)
{
	Logger::PrintError(childType + "(" + ToString::Convert(childId)
		+ ") has an unsupported connection to a parent " + parentType + "(" + ToString::Convert(parentId) + ")");
}
