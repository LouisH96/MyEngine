#pragma once
#include "Math/Float2.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			class FbxObject;
			class FbxReader;

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
				FbxData(FbxReader&& reader);

				const Geometry& GetGeometry(int idx) const { return m_Geometries[idx]; }
				Geometry& GetGeometry(int idx) { return m_Geometries[idx]; }
				const Array<Geometry>& GetGeometries() const { return m_Geometries; }
				Array<Geometry>& GetGeometries() { return m_Geometries; }

			private:
				Array<Geometry> m_Geometries{};

				static void LoadPoints(FbxObject& fbxGeometry, Geometry& geometryStruct);
				static void LoadNormals(FbxObject& geometry, Geometry& geometryStruct);
				static void LoadIndices(FbxObject& geometry, Geometry& geometryStruct);
				static void LoadUvs(FbxObject& geometry, Geometry& geometryStruct);
			};
		}
	}
}
