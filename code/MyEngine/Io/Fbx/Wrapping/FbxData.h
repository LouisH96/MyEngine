#pragma once
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
					const Array<Model>& GetModels(int idx) const { return m_Models; }
					Model& GetModel(int idx) { return m_Models[idx]; }
					Array<Model>& GetModels() { return m_Models; }
					Pose& GetBindPose() { return m_BindPose; }
					AnimationStack& GetAnimationStack() { return m_AnimationStack; }

					Array<Model> GetModelsOfType(const std::string& typeName) const;

					Model* FindModel(const int64_t& id);

				private:
					Array<Geometry> m_Geometries{};
					Array<Model> m_Models{};
					Pose m_BindPose{};
					AnimationStack m_AnimationStack{};
				};
			}
		}
	}
}
