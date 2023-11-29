#include "FbxData.h"

#include "Video.h"
#include "../Reading/FbxReader.h"
#include "Io/Fbx/Reading/FbxFile.h"
#include "Logger/Logger.h"

using namespace MyEngine::Io::Fbx::Wrapping;
using namespace MyEngine::Io::Fbx::Reading;

FbxData::FbxData(const std::wstring& path)
	: FbxData{ FbxReader::Read(path) }
{}

FbxData::FbxData(FbxFile data)
{
	FbxElement& objects{ *data.GetRoot().GetChild("Objects") };
	FbxElement& connections{ *data.GetRoot().GetChild("Connections") };

	//data.GetRoot().Print();

	ReadInfo(data.GetRoot());
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

MyEngine::Array<Model> FbxData::GetModelsOfType(const std::string& typeName) const
{
	int count{ 0 };
	for (unsigned i = 0; i < m_Models.GetSize(); i++)
		if (m_Models[i].GetTypeName() == typeName)
			count++;

	int iModel{ 0 };
	Array<Model> models{ count };
	for (unsigned i = 0; i < m_Models.GetSize(); i++)
		if (m_Models[i].GetTypeName() == typeName)
			models[iModel++] = m_Models[i];

	return models;
}

MyEngine::Array<Model> FbxData::GetLimbNodes() const
{
	return GetModelsOfType("LimbNode");
}

const Model* FbxData::GetARootLimbNode() const
{
	for (unsigned iModel = 0; iModel < m_Models.GetSize(); iModel++)
	{
		const Model& model{ m_Models[iModel] };
		if (model.GetTypeName() != "LimbNode") continue;

		//find root connection
		for (unsigned iConnection = 0; iConnection < m_Connections.GetSize(); iConnection++)
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

Geometry* FbxData::FindGeometry(const int64_t& id)
{
	for (unsigned i = 0; i < m_Geometries.GetSize(); i++)
		if (m_Geometries[i].GetId() == id)
			return &m_Geometries[i];
	return nullptr;
}

Model* FbxData::FindModel(const int64_t& id)
{
	for (unsigned i = 0; i < m_Models.GetSize(); i++)
		if (m_Models[i].GetId() == id)
			return &m_Models[i];
	return nullptr;
}

NodeAttribute* FbxData::FindNodeAttribute(const int64_t& id)
{
	for (unsigned i = 0; i < m_NodeAttributes.GetSize(); i++)
		if (m_NodeAttributes[i].Id == id)
			return &m_NodeAttributes[i];
	return nullptr;
}

Deformer* FbxData::FindDeformer(const int64_t& id)
{
	for (unsigned i = 0; i < m_Deformers.GetSize(); i++)
		if (m_Deformers[i].Id == id)
			return &m_Deformers[i];
	return nullptr;
}

Pose::Node* FbxData::FindPoseNode(const int64_t& id)
{
	for (unsigned i = 0; i < m_BindPose.Nodes.GetSize(); i++)
		if (m_BindPose.Nodes[i].Id == id)
			return &m_BindPose.Nodes[i];
	return nullptr;
}

AnimationStack* FbxData::FindAnimationStack(const int64_t& id)
{
	for (unsigned i = 0; i < m_AnimationStacks.GetSize(); i++)
		if (m_AnimationStacks[i].Id == id)
			return &m_AnimationStacks[i];
	return nullptr;
}

AnimationLayer* FbxData::FindAnimationLayer(const int64_t& id)
{
	for (unsigned i = 0; i < m_AnimationLayers.GetSize(); i++)
		if (m_AnimationLayers[i].Id == id)
			return &m_AnimationLayers[i];
	return nullptr;
}

AnimationCurve* FbxData::FindAnimationCurve(const int64_t& id)
{
	for (unsigned i = 0; i < m_AnimationCurves.GetSize(); i++)
		if (m_AnimationCurves[i].Id == id)
			return &m_AnimationCurves[i];
	return nullptr;
}

AnimationCurveNode* FbxData::FindAnimationCurveNode(const int64_t& id)
{
	for (unsigned i = 0; i < m_AnimationCurveNodes.GetSize(); i++)
		if (m_AnimationCurveNodes[i].Id == id)
			return &m_AnimationCurveNodes[i];
	return nullptr;
}

Video* FbxData::FindVideo(const int64_t& id)
{
	for (unsigned i = 0; i < m_Videos.GetSize(); i++)
		if (m_Videos[i].Id == id)
			return &m_Videos[i];
	return nullptr;
}

FbxWrapTexture* FbxData::FindTexture(const int64_t& id)
{
	for (unsigned i = 0; i < m_Textures.GetSize(); i++)
		if (m_Textures[i].Id == id)
			return &m_Textures[i];
	return nullptr;
}

FbxWrapMaterial* FbxData::FindMaterial(const int64_t& id)
{
	for (unsigned i = 0; i < m_Materials.GetSize(); i++)
		if (m_Materials[i].Id == id)
			return &m_Materials[i];
	return nullptr;
}

CollectionExclusive* FbxData::FindCollection(const int64_t& id)
{
	for (unsigned i = 0; i < m_Collections.GetSize(); i++)
		if (m_Collections[i].Id == id)
			return &m_Collections[i];
	return nullptr;
}

const Geometry* FbxData::FindGeometry(const int64_t& id) const
{
	for (unsigned i = 0; i < m_Geometries.GetSize(); i++)
		if (m_Geometries[i].GetId() == id)
			return &m_Geometries[i];
	return nullptr;
}

const Deformer* FbxData::FindDeformer(const int64_t& id) const
{
	for (unsigned i = 0; i < m_Deformers.GetSize(); i++)
		if (m_Deformers[i].Id == id)
			return &m_Deformers[i];
	return nullptr;
}

const Pose::Node* FbxData::FindPoseNode(const int64_t& id) const
{
	for (unsigned i = 0; i < m_BindPose.Nodes.GetSize(); i++)
		if (m_BindPose.Nodes[i].Id == id)
			return &m_BindPose.Nodes[i];
	return nullptr;
}

const AnimationStack* FbxData::FindAnimationStack(const int64_t& id) const
{
	for (unsigned i = 0; i < m_AnimationStacks.GetSize(); i++)
		if (m_AnimationStacks[i].Id == id)
			return &m_AnimationStacks[i];
	return nullptr;
}

const AnimationLayer* FbxData::FindAnimationLayer(const int64_t& id) const
{
	for (unsigned i = 0; i < m_AnimationLayers.GetSize(); i++)
		if (m_AnimationLayers[i].Id == id)
			return &m_AnimationLayers[i];
	return nullptr;
}

const AnimationCurve* FbxData::FindAnimationCurve(const int64_t& id) const
{
	for (unsigned i = 0; i < m_AnimationCurves.GetSize(); i++)
		if (m_AnimationCurves[i].Id == id)
			return &m_AnimationCurves[i];
	return nullptr;
}

const Video* FbxData::FindVideo(const int64_t& id) const
{
	for (unsigned i = 0; i < m_Videos.GetSize(); i++)
		if (m_Videos[i].Id == id)
			return &m_Videos[i];
	return nullptr;
}

const FbxWrapTexture* FbxData::FindTexture(const int64_t& id) const
{
	for (unsigned i = 0; i < m_Textures.GetSize(); i++)
		if (m_Textures[i].Id == id)
			return &m_Textures[i];
	return nullptr;
}

const FbxWrapMaterial* FbxData::FindMaterial(const int64_t& id) const
{
	for (unsigned i = 0; i < m_Materials.GetSize(); i++)
		if (m_Materials[i].Id == id)
			return &m_Materials[i];
	return nullptr;
}

const CollectionExclusive* FbxData::FindCollection(const int64_t& id) const
{
	for (unsigned i = 0; i < m_Collections.GetSize(); i++)
		if (m_Collections[i].Id == id)
			return &m_Collections[i];
	return nullptr;
}

const AnimationCurveNode* FbxData::FindAnimationCurveNode(const int64_t& id) const
{
	for (unsigned i = 0; i < m_AnimationCurveNodes.GetSize(); i++)
		if (m_AnimationCurveNodes[i].Id == id)
			return &m_AnimationCurveNodes[i];
	return nullptr;
}

const Model* FbxData::FindModel(const int64_t& id) const
{
	for (unsigned i = 0; i < m_Models.GetSize(); i++)
		if (m_Models[i].GetId() == id)
			return &m_Models[i];
	return nullptr;
}

const NodeAttribute* FbxData::FindNodeAttribute(const int64_t& id) const
{
	for (unsigned i = 0; i < m_NodeAttributes.GetSize(); i++)
		if (m_NodeAttributes[i].Id == id)
			return &m_NodeAttributes[i];
	return nullptr;
}

MyEngine::Array<const Model*> FbxData::GetChildren(const Model& model) const
{
	return GetChildren(model.GetId());
}

MyEngine::Array<const Model*> FbxData::GetChildren(const int64_t& id) const
{
	Array<const Model*> children{ 0 };
	for (unsigned i = 0; i < m_Connections.GetSize(); i++)
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

void FbxData::ReadInfo(FbxElement& root)
{
	const FbxElement& globalSettings{ *root.GetChild("GlobalSettings") };
	const Properties70 props{ *globalSettings.GetChild("Properties70") };

	const int up{ props.GetInt("OriginalUpAxis") };
	const int upSign{ props.GetInt("OriginalUpAxisSign") };

	m_Orientation = FbxOrientation{ up * upSign };
}

void FbxData::ReadGeometry(FbxElement& objectsObject)
{
	const List<FbxElement*> geometries{ objectsObject.GetChildren("Geometry") };
	m_Geometries = { geometries.GetSize() };
	for (unsigned i = 0; i < geometries.GetSize(); i++)
		m_Geometries[i] = Geometry{ *geometries[i], m_Orientation };
}

void FbxData::ReadModels(FbxElement& objectsObject)
{
	const List<FbxElement*> models{ objectsObject.GetChildren("Model") };
	m_Models = { models.GetSize() };
	for (unsigned i = 0; i < m_Models.GetSize(); i++)
		m_Models[i] = Model{ *models[i], m_Orientation };
}

void FbxData::ReadNodeAttributes(FbxElement& objectsObject)
{
	const List<FbxElement*> nodeAttributes{ objectsObject.GetChildren("NodeAttribute") };
	m_NodeAttributes = { nodeAttributes.GetSize() };
	for (unsigned i = 0; i < m_NodeAttributes.GetSize(); i++)
		m_NodeAttributes[i] = NodeAttribute{ *nodeAttributes[i] };
}

void FbxData::ReadDeformers(FbxElement& objectsObject)
{
	const List<FbxElement*> deformers{ objectsObject.GetChildren("Deformer") };
	m_Deformers = { deformers.GetSize() };
	for (unsigned i = 0; i < m_Deformers.GetSize(); i++)
		m_Deformers[i] = Deformer{ *deformers[i] };
}

void FbxData::ReadPoses(FbxElement& objectsObject)
{
	const List<FbxElement*> poses{ objectsObject.GetChildren("Pose") };
	if (poses.GetSize() > 1) Logger::PrintWarning("Doesn't support multiple poses");
	if (poses.GetSize() > 0)
		m_BindPose = Pose{ *poses[0] };
}

void FbxData::ReadConnections(FbxElement& connectionsObject)
{
	List<FbxElement>& readerConnections{ connectionsObject.GetChildren() };
	m_Connections = { readerConnections.GetSize() };
	for (unsigned i = 0; i < readerConnections.GetSize(); i++)
		m_Connections[i] = Connection{ readerConnections[i] };
}

void FbxData::ReadAnimationStack(FbxElement& objectsObject)
{
	const List<FbxElement*> readerAnimationStacks{ objectsObject.GetChildren("AnimationStack") };
	m_AnimationStacks = { readerAnimationStacks.GetSize() };
	for (unsigned i = 0; i < readerAnimationStacks.GetSize(); i++)
		m_AnimationStacks[i] = AnimationStack{ *readerAnimationStacks[i] };
}

void FbxData::ReadAnimationLayers(FbxElement& objectsObject)
{
	const List<FbxElement*> readerAnimationLayers{ objectsObject.GetChildren("AnimationLayer") };
	if (readerAnimationLayers.IsEmpty())return;
	m_AnimationLayers = { readerAnimationLayers.GetSize() };
	for (unsigned i = 0; i < m_AnimationLayers.GetSize(); i++)
		m_AnimationLayers[i] = AnimationLayer{ *readerAnimationLayers[i] };
}

void FbxData::ReadAnimationCurveNodes(FbxElement& objectsObject)
{
	const List<FbxElement*> curveNodeObjects{ objectsObject.GetChildren("AnimationCurveNode") };
	m_AnimationCurveNodes = { curveNodeObjects.GetSize() };
	for (unsigned i = 0; i < m_AnimationCurveNodes.GetSize(); i++)
		m_AnimationCurveNodes[i] = AnimationCurveNode{ *curveNodeObjects[i] };
}

void FbxData::ReadAnimationCurves(FbxElement& objectsObject)
{
	const List<FbxElement*> objects{ objectsObject.GetChildren("AnimationCurve") };
	m_AnimationCurves = { objects.GetSize() };
	for (unsigned i = 0; i < m_AnimationCurves.GetSize(); i++)
		m_AnimationCurves[i] = AnimationCurve{ *objects[i] };
}

void FbxData::ReadTextures(FbxElement& objectsObject)
{
	const List<FbxElement*> objects{ objectsObject.GetChildren("Texture") };
	m_Textures = { objects.GetSize() };
	for (unsigned i = 0; i < m_Textures.GetSize(); i++)
		m_Textures[i] = FbxWrapTexture{ *objects[i] };
}

void FbxData::ReadVideos(FbxElement& objectsObject)
{
	const List<FbxElement*> objects{ objectsObject.GetChildren("Video") };
	m_Videos = { objects.GetSize() };
	for (unsigned i = 0; i < m_Videos.GetSize(); i++)
		m_Videos[i] = Video{ *objects[i] };
}

void FbxData::ReadMaterial(FbxElement& objectsObject)
{
	const List<FbxElement*> objects{ objectsObject.GetChildren("Material") };
	m_Materials = { objects.GetSize() };
	for (unsigned i = 0; i < m_Materials.GetSize(); i++)
		m_Materials[i] = FbxWrapMaterial{ *objects[i] };
}

void FbxData::ReadCollections(FbxElement& objectsObject)
{
	const List<FbxElement*> objects{ objectsObject.GetChildren("CollectionExclusive") };
	m_Collections = { objects.GetSize() };
	for (unsigned i = 0; i < m_Collections.GetSize(); i++)
		m_Collections[i] = CollectionExclusive{ *objects[i] };
}

void FbxData::HandleConnections()
{
	for (unsigned iConnection = 0; iConnection < m_Connections.GetSize(); iConnection++)
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

void FbxData::HandleGeometryConnection(Geometry& geometry, const Connection& connection)
{
	Model* pModel{ FindModel(connection.ParentId) };
	if (pModel)
	{
		geometry.SetRootModel(*pModel);
		return;
	}

	PrintUnhandledConnectionError(FindTypeName(connection.ParentId), "Geometry");
}

void FbxData::HandleModelConnection(Model& childModel, const Connection& connection)
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
		pDeformer->SetModel(childModel);
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

void FbxData::HandleNodeAttributeConnection(NodeAttribute& nodeAttribute,
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

void FbxData::HandleDeformerConnection(Deformer& childDeformer, const Connection& connection)
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

void FbxData::HandleAnimationLayerConnection(AnimationLayer& animationLayer,
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

void FbxData::HandleAnimationCurveConnection(AnimationCurve& animationCurve, const Connection& connection)
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

void FbxData::HandleAnimationCurveNodeConnection(AnimationCurveNode& childAnimationCurveNode,
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

void FbxData::HandleVideoConnection(Video& video, const Connection& connection)
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

void FbxData::HandleTextureConnection(FbxWrapTexture& texture, const Connection& connection)
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

void FbxData::HandleMaterialConnection(FbxWrapMaterial& material, const Connection& connection)
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

std::string FbxData::FindTypeName(const int64_t& id) const
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

void FbxData::PrintUnhandledConnectionError(const std::string& parentType,
	const std::string& childType)
{
	Logger::PrintError(childType + " has an unsupported connection to a parent " + parentType);
}

void FbxData::PrintUnhandledConnectionError(const std::string& parentType, const int64_t& parentId,
	const std::string& childType, const int64_t& childId)
{
	Logger::PrintError(childType + "(" + ToString::Convert(childId)
		+ ") has an unsupported connection to a parent " + parentType + "(" + ToString::Convert(parentId) + ")");
}
