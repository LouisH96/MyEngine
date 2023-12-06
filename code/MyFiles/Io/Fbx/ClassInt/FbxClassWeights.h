#pragma once
#include "DataStructures/List.h"
#include "Io/Fbx/FbxClass.h"
#include "Io/Fbx/FbxLoadData.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Wrapping
			{
				class Deformer;
				class Geometry;
			}

			class FbxClassWeights
			{
			public:
				explicit FbxClassWeights(const Array<Wrapping::Geometry>& geometries);

				void CreateWeights(
					Array<FbxClass::BlendData>& blendData,
					const Wrapping::Geometry& wrappingGeometry,
					const FbxLoadData::JointLookup& jointLookup);

			private:
				static constexpr unsigned NR_WEIGHTS{ FbxClass::BlendData::NR_WEIGHTS };
				struct Weight
				{
					int64_t JointModelIdx;
					double Amount;
				};
				Array<List<Weight>> m_Weights{};

				void CreateFromSkinDeformer(
					Array<FbxClass::BlendData>& blendData,
					const Wrapping::Deformer& skinDeformer,
					const FbxLoadData::JointLookup& jointLookup);

				void CreateFromRootJoint(
					Array<FbxClass::BlendData>& blendData,
					const Wrapping::Geometry& wrappingGeometry,
					const FbxLoadData::JointLookup& jointLookup) const;
			};
		}
	}
}
