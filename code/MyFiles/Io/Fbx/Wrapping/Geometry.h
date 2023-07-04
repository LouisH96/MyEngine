#pragma once
#include <cstdint>

#include "DataStructures/Array.h"
#include "Math/Vectors.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				class FbxObject;
			}

			namespace Wrapping
			{
				class Deformer;
				class Model;

				class Geometry
				{
				public:
					Geometry() = default;
					explicit Geometry(Reading::FbxObject& geometryObject);

					const int64_t& GetId() const { return m_Id; }
					Array<Float3>& GetPoints() { return m_Points; }
					Array<Float3>& GetNormals() { return m_Normals; }
					Array<Float2>& GetUvs() { return m_Uvs; }
					Array<int>& GetIndices() { return m_Indices; }
					const Array<Float3>& GetPoints() const { return m_Points; }
					const Array<Float3>& GetNormals() const { return m_Normals; }
					const Array<Float2>& GetUvs() const { return m_Uvs; }
					const Array<int>& GetIndices() const { return m_Indices; }

					void SetRootModel(const Model& model);
					void AddDeformer(const Deformer& deformer);

				private:
					int64_t m_Id{};
					std::string m_Name{};
					Array<Float3> m_Points{};
					Array<Float3> m_Normals{};
					Array<Float2> m_Uvs{};
					Array<int> m_Indices{};

					const Model* m_pRootModel{};
					Array<const Deformer*> m_Deformers{};

					void LoadPoints(const Reading::FbxObject& geometryObject);
					void LoadNormals(const Reading::FbxObject& geometryObject);
					void LoadIndices(Reading::FbxObject& geometryObject);
					void LoadUvs(const Reading::FbxObject& geometryObject);
				};
			}
		}
	}
}
