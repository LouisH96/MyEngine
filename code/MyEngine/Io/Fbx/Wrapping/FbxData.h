#pragma once
#include "AnimationCurve.h"
#include "AnimationCurveNode.h"
#include "AnimationLayer.h"
#include "AnimationStack.h"
#include "Geometry.h"
#include "Model.h"
#include "Pose.h"

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
					AnimationStack& GetAnimationStack() { return m_AnimationStack; }
					Array<AnimationLayer>& GetAnimationLayers() { return m_AnimationLayers; }
					Array<AnimationCurveNode>& GetAnimationCurveNodes() { return m_AnimationCurveNodes; }

					Array<Model> GetModelsOfType(const std::string& typeName) const;
					Array<Model> GetLimbNodes() const;
					const Model* GetRootLimbNode() const;

					Geometry* FindGeometry(const int64_t& id);

					Model* FindModel(const int64_t& id);
					const Model* FindModel(const int64_t& id) const;

					Array<const Model*> GetChildren(const Model& model) const;
					Array<const Model*> GetChildren(const int64_t& id) const;

				private:
					Array<Geometry> m_Geometries{};
					Array<Model> m_Models{};
					Pose m_BindPose{};
					Array<Connection> m_Connections{};
					AnimationStack m_AnimationStack{};
					Array<AnimationLayer> m_AnimationLayers{};
					Array<AnimationCurveNode> m_AnimationCurveNodes{};
					Array<AnimationCurve> m_AnimationCurves{};

					void ReadGeometry(const Reading::FbxObject& objectsObject);
					void ReadModels(const Reading::FbxObject& objectsObject);
					void ReadPoses(const Reading::FbxObject& objectsObject);
					void ReadConnections(const Reading::FbxObject& connectionsObject);
					void ReadAnimationStack(const Reading::FbxObject& objectsObject);
					void ReadAnimationLayers(const Reading::FbxObject& objectsObject);
					void ReadAnimationCurveNodes(const Reading::FbxObject& objectsObject);
					void ReadAnimationCurves(const Reading::FbxObject& objectsObject);

					void HandleConnections();

					void HandleGeometryConnection(Geometry& geometry, const Connection& connection);
				};
			}
		}
	}
}
