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
				class FbxData;
				class Model;
			}

			class FbxClass
			{
			public:
				//---| Types |---
				struct BlendData
				{
					const FbxJoint* pJoint;
					double Weight;
				};
				struct Geometry
				{
					std::string Name{};
					Array<Float3> Points{};
					Array<Float3> Normals{};
					Array<Float2> Uvs{};
					Array<unsigned> Indices{};
					Array<List<BlendData>> Weights{};
					Float3 RotationPivot{};
					Float3 RotationOffset{};
				};

				//---| Construction |---
				explicit FbxClass(const std::wstring& path);
				explicit FbxClass(Wrapping::FbxData&& data);
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

				static void MakeTriangleList(Geometry& geomStruct);
			};
		}
	}
}
