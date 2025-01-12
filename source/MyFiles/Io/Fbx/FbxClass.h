#pragma once
#include "FbxSkeleton.h"
#include "FbxAnimation.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Wrapping
			{
				class FbxOrientation;
				class FbxData;
				class Model;
			}

			class FbxClass
			{
			public:
				//---| Types |---
				struct BlendData
				{
					static constexpr unsigned NR_WEIGHTS{ 4 };
					int jointIdx[NR_WEIGHTS];
					float weight[NR_WEIGHTS];

					Int4 IndicesAsInt4() const;
					Float4 WeightsAsFloat4() const;
				};

				struct Geometry
				{
					std::string Name{};
					Array<Float3> Points{};
					Array<Float3> Normals{};
					Array<Float2> Uvs{};
					Array<unsigned> Indices{};
					Array<BlendData> Weights{};
					Float3 RotationPivot{};
					Float3 RotationOffset{};
				};

				//---| Construction |---
				explicit FbxClass(const std::wstring& path, float scale, bool print = false);
				explicit FbxClass(Wrapping::FbxData&& data, float scale);
				~FbxClass() = default;

				FbxClass(const FbxClass& other) = delete;
				FbxClass(FbxClass&& other) noexcept = delete;
				FbxClass& operator=(const FbxClass& other) = delete;
				FbxClass& operator=(FbxClass&& other) noexcept = delete;

				//---| Functions |---
				const Geometry& GetGeometry(int idx) const { return m_Geometries[idx]; }
				Geometry& GetGeometry(int idx) { return m_Geometries[idx]; }
				const Array<Geometry>& GetGeometries() const { return m_Geometries; }
				Array<Geometry>& GetGeometries() { return m_Geometries; }

				const FbxSkeleton& GetSkeleton() const { return m_Skeleton; }
				const Array<FbxAnimation>& GetAnimations() const { return m_Animations; }
				int GetNrOfAnimationLayers() const;

			private:
				Array<Geometry> m_Geometries{};
				Array<FbxAnimation> m_Animations{};
				FbxSkeleton m_Skeleton{};

				static void MakeTriangleList(Geometry& geomStruct, const Wrapping::FbxOrientation& orientation);
			};
		}
	}
}
