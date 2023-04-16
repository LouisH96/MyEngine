#pragma once
#include "Math/Float2.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Wrapping
			{
				class FbxData;
			}

			class FbxClass
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

				//---| Class |---
				FbxClass(const std::wstring& path);
				~FbxClass() = default; FbxClass(const FbxClass& other) = default;
				FbxClass(FbxClass&& other) noexcept = default;
				FbxClass& operator=(const FbxClass& other) = default;
				FbxClass& operator=(FbxClass&& other) noexcept = default;

				const Geometry& GetGeometry(int idx) const { return m_Geometries[idx]; }
				Geometry& GetGeometry(int idx) { return m_Geometries[idx]; }
				const Array<Geometry>& GetGeometries() const { return m_Geometries; }
				Array<Geometry>& GetGeometries() { return m_Geometries; }

			private:
				Array<Geometry> m_Geometries{};

				static void MakeTriangleList(Geometry& geomStruct);
			};
		}
	}
}
