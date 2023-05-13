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
					std::string Name{};
					Array<Float3> Points{};
					Array<Float3> Normals{};
					Array<Float2> Uvs{};
					Array<int> Indices{};
					Float3 RotationPivot{};
					Float3 RotationOffset{};
				};
				struct LimbNode
				{
					std::string Name{};
					Float3 Position{};
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

				const Array<LimbNode>& GetLimbNodes() const { return m_LimbNodes; }

			private:
				Array<Geometry> m_Geometries{};
				Array<LimbNode> m_LimbNodes{};

				static void MakeTriangleList(Geometry& geomStruct);
			};
		}
	}
}
