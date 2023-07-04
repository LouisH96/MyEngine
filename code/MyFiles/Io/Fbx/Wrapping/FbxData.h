#pragma once
#include "AnimationCurve.h"
#include "AnimationCurveNode.h"
#include "AnimationLayer.h"
#include "AnimationStack.h"
#include "CollectionExclusive.h"
#include "Deformer.h"
#include "FbxWrapMaterial.h"
#include "FbxWrapTexture.h"
#include "Geometry.h"
#include "Model.h"
#include "NodeAttribute.h"
#include "Pose.h"
#include "Video.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				class FbxObject;
				class FbxReader;
			}

			namespace Wrapping
			{
				class FbxData
				{
				public:
					FbxData(const std::wstring& fbxPath);
					FbxData(Reading::FbxReader&& reader);

					const Geometry& GetGeometry(int idx) const { return m_Geometries[idx]; }
					const Array<Geometry>& GetGeometries() const { return m_Geometries; }
					Geometry& GetGeometry(int idx) { return m_Geometries[idx]; }
					Array<Geometry>& GetGeometries() { return m_Geometries; }
					const Model& GetModel(int idx) const { return m_Models[idx]; }
					const Array<Model>& GetModels() const { return m_Models; }
					Model& GetModel(int idx) { return m_Models[idx]; }
					Array<Model>& GetModels() { return m_Models; }
					Pose& GetBindPose() { return m_BindPose; }
					Array<AnimationStack> GetAnimationStacks() { return m_AnimationStacks; }
					Array<AnimationLayer>& GetAnimationLayers() { return m_AnimationLayers; }
					Array<AnimationCurveNode>& GetAnimationCurveNodes() { return m_AnimationCurveNodes; }

					Array<Model> GetModelsOfType(const std::string& typeName) const;
					Array<Model> GetLimbNodes() const;
					const Model* GetARootLimbNode() const;

					Geometry* FindGeometry(const int64_t& id);
					Model* FindModel(const int64_t& id);
					NodeAttribute* FindNodeAttribute(const int64_t& id);
					Deformer* FindDeformer(const int64_t& id);
					Pose::Node* FindPoseNode(const int64_t& id);
					AnimationStack* FindAnimationStack(const int64_t& id);
					AnimationLayer* FindAnimationLayer(const int64_t& id);
					AnimationCurve* FindAnimationCurve(const int64_t& id);
					AnimationCurveNode* FindAnimationCurveNode(const int64_t& id);
					Video* FindVideo(const int64_t& id);
					FbxWrapTexture* FindTexture(const int64_t& id);
					FbxWrapMaterial* FindMaterial(const int64_t& id);
					CollectionExclusive* FindCollection(const int64_t& id);

					const Geometry* FindGeometry(const int64_t& id) const;
					const Model* FindModel(const int64_t& id) const;
					const NodeAttribute* FindNodeAttribute(const int64_t& id) const;
					const Deformer* FindDeformer(const int64_t& id) const;
					const Pose::Node* FindPoseNode(const int64_t& id) const;
					const AnimationStack* FindAnimationStack(const int64_t& id) const;
					const AnimationLayer* FindAnimationLayer(const int64_t& id) const;
					const AnimationCurveNode* FindAnimationCurveNode(const int64_t& id) const;
					const AnimationCurve* FindAnimationCurve(const int64_t& id) const;
					const Video* FindVideo(const int64_t& id) const;
					const FbxWrapTexture* FindTexture(const int64_t& id) const;
					const FbxWrapMaterial* FindMaterial(const int64_t& id) const;
					const CollectionExclusive* FindCollection(const int64_t& id) const;

					Array<const Model*> GetChildren(const Model& model) const;
					Array<const Model*> GetChildren(const int64_t& id) const;

				private:
					Array<Geometry> m_Geometries{};
					Array<Model> m_Models{};
					Array<NodeAttribute> m_NodeAttributes{};
					Array<Deformer> m_Deformers{};
					Pose m_BindPose{};
					Array<Connection> m_Connections{};
					Array<AnimationStack> m_AnimationStacks{};
					Array<AnimationLayer> m_AnimationLayers{};
					Array<AnimationCurveNode> m_AnimationCurveNodes{};
					Array<AnimationCurve> m_AnimationCurves{};
					Array<FbxWrapTexture> m_Textures{};
					Array<Video> m_Videos{};
					Array<FbxWrapMaterial> m_Materials{};
					Array<CollectionExclusive> m_Collections{};

					void ReadGeometry(const Reading::FbxObject& objectsObject);
					void ReadModels(const Reading::FbxObject& objectsObject);
					void ReadNodeAttributes(const Reading::FbxObject& objectsObject);
					void ReadDeformers(const Reading::FbxObject& objectsObject);
					void ReadPoses(const Reading::FbxObject& objectsObject);
					void ReadConnections(const Reading::FbxObject& connectionsObject);
					void ReadAnimationStack(const Reading::FbxObject& objectsObject);
					void ReadAnimationLayers(const Reading::FbxObject& objectsObject);
					void ReadAnimationCurveNodes(const Reading::FbxObject& objectsObject);
					void ReadAnimationCurves(const Reading::FbxObject& objectsObject);
					void ReadTextures(const Reading::FbxObject& objectsObject);
					void ReadVideos(const Reading::FbxObject& objectsObject);
					void ReadMaterial(const Reading::FbxObject& objectsObject);
					void ReadCollections(const Reading::FbxObject& objectsObject);

					void HandleConnections();
					void HandleGeometryConnection(Geometry& geometry, const Connection& connection);
					void HandleModelConnection(Model& childModel, const Connection& connection);
					void HandleNodeAttributeConnection(NodeAttribute& nodeAttribute, const Connection& connection);
					void HandleDeformerConnection(Deformer& childDeformer, const Connection& connection);
					void HandleAnimationLayerConnection(AnimationLayer& animationLayer, const Connection& connection);
					void HandleAnimationCurveConnection(AnimationCurve& animationCurve, const Connection& connection);
					void HandleAnimationCurveNodeConnection(AnimationCurveNode& childAnimationCurveNode, const Connection& connection);
					void HandleVideoConnection(Video& video, const Connection& connection);
					void HandleTextureConnection(FbxWrapTexture& texture, const Connection& connection);
					void HandleMaterialConnection(FbxWrapMaterial& material, const Connection& connection);

					std::string FindTypeName(const int64_t& id) const;
					static void PrintUnhandledConnectionError(const std::string& parentType, const std::string& childType);
					static void PrintUnhandledConnectionError(const std::string& parentType, const int64_t& parentId, const std::string& childType, const int64_t& childId);
				};
			}
		}
	}
}
