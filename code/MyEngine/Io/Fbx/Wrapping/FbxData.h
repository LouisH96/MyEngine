#pragma once
#include "Math/Float2.h"

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
					//---| Types |---
					struct Geometry
					{
						Array<Math::Float3> Points{};
						Array<Math::Float3> Normals{};
						Array<Math::Float2> Uvs{};
						Array<int> Indices{};
					};

					FbxData(const std::wstring& fbxPath);
					FbxData(Reading::FbxReader&& reader);

					const Geometry& GetGeometry(int idx) const { return m_Geometries[idx]; }
					Geometry& GetGeometry(int idx) { return m_Geometries[idx]; }
					const Array<Geometry>& GetGeometries() const { return m_Geometries; }
					Array<Geometry>& GetGeometries() { return m_Geometries; }

				private:
					Array<Geometry> m_Geometries{};

					static void LoadPoints(Reading::FbxObject& fbxGeometry, Geometry& geometryStruct);
					static void LoadNormals(Reading::FbxObject& geometry, Geometry& geometryStruct);
					static void LoadIndices(Reading::FbxObject& geometry, Geometry& geometryStruct);
					static void LoadUvs(Reading::FbxObject& geometry, Geometry& geometryStruct);
				};
			}
		}
	}
}
