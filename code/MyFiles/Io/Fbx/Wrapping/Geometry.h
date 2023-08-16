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
				class FbxElement;
			}

			namespace Wrapping
			{
				class Deformer;
				class Model;

				class Geometry
				{
				public:
					//---| Construction |---
					Geometry() = default;
					explicit Geometry(Reading::FbxElement& geometryObject, int upAxis);
					~Geometry() = default;

					Geometry(const Geometry& other) = delete;
					Geometry& operator=(const Geometry& other) = delete;
					Geometry(Geometry&& other) = default;
					Geometry& operator= (Geometry&& other) = default;

					//---| Functions |---
					const int64_t& GetId() const { return m_Id; }
					Array<Float3>& GetPoints() { return m_Points; }
					Array<Float3>& GetNormals() { return m_Normals; }
					Array<Float2>& GetUvs() { return m_Uvs; }
					Array<unsigned>& GetIndices() { return m_Indices; }
					const Array<Float3>& GetPoints() const { return m_Points; }
					const Array<Float3>& GetNormals() const { return m_Normals; }
					const Array<Float2>& GetUvs() const { return m_Uvs; }
					const Array<unsigned>& GetIndices() const { return m_Indices; }

					void SetRootModel(const Model& model);
					void AddDeformer(const Deformer& deformer);

				private:
					int64_t m_Id{};
					std::string m_Name{};
					Array<Float3> m_Points{};
					Array<Float3> m_Normals{};
					Array<Float2> m_Uvs{};
					Array<unsigned> m_Indices{};

					const Model* m_pRootModel{};
					Array<const Deformer*> m_Deformers{};

					void LoadPoints(const Reading::FbxElement& geometryObject, int upAxis);
					void LoadNormals(const Reading::FbxElement& geometryObject);
					void LoadIndices(Reading::FbxElement& geometryObject);
					void LoadUvs(Reading::FbxElement& geometryObject);
				};
			}
		}
	}
}
