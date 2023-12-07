#include "FbxClassWeights.h"

#include "DataStructures/PeakCollection.h"
#include "Io/Fbx/Wrapping/Deformer.h"
#include "Io/Fbx/Wrapping/Geometry.h"
#include "Io/Fbx/Wrapping/Model.h"

using namespace MyEngine;
using namespace Io::Fbx;
using namespace Wrapping;

FbxClassWeights::FbxClassWeights(const Array<Geometry>& geometries)
{
	unsigned maxVertices{ 0 };
	for (unsigned i = 0; i < geometries.GetSize(); i++)
		maxVertices = Uint::Max(maxVertices, geometries[i].GetPoints().GetSize());

	m_Weights = { maxVertices };
}

void FbxClassWeights::CreateWeights(
	Array<FbxClass::BlendData>& blendData,
	const Geometry& wrappingGeometry,
	const FbxLoadData::JointLookup& jointLookup)
{
	const Deformer* pSkinDeformer{ wrappingGeometry.GetSkinDeformer() };

	if (pSkinDeformer)
		CreateFromSkinDeformer(blendData, *pSkinDeformer, jointLookup);
	else
		CreateFromRootJoint(blendData, wrappingGeometry, jointLookup);
}

void FbxClassWeights::CreateFromSkinDeformer(
	Array<FbxClass::BlendData>& blendData,
	const Deformer& skinDeformer,
	const FbxLoadData::JointLookup& jointLookup)
{
	//Clear
	for (unsigned i = 0; i < blendData.GetSize(); i++)
		m_Weights[i].Clear();

	//Fill WeightCollections
	for (unsigned iCluster = 0; iCluster < skinDeformer.GetChildDeformers().GetSize(); iCluster++)
	{
		const Deformer& deformer{ *skinDeformer.GetChildDeformers()[iCluster] };
		if (!deformer.HasClusterData())
		{
			Logger::PrintWarning("[FbxClassWeights::GetWeightsFromSkinDeformer] child-deformer isn't a cluster-deformer");
			continue;
		}
		const DeformerClusterData& clusterData{ deformer.GetClusterData() };
		const int64_t jointModelIdx{ deformer.GetModel()->GetId() };

		for (unsigned iIndex = 0; iIndex < clusterData.Indexes.GetSize(); iIndex++)
		{
			const int vertexIdx{ clusterData.Indexes[iIndex] };
			const double weight{ clusterData.Weights[iIndex] };
			m_Weights[vertexIdx].Add({ jointModelIdx, weight });
		}
	}

	//Create BlendData
	for (unsigned iVertex = 0; iVertex < blendData.GetSize(); iVertex++)
	{
		const List<Weight>& weights{ m_Weights[iVertex] };

		//init
		PeakCollection<Weight> highestWeights{ NR_WEIGHTS, Weight{Scalar<int64_t>::MAX, 0} };

		//find x-amount highest
		for (unsigned iWeight = 0; iWeight < weights.GetSize(); iWeight++)
			highestWeights.TryAdd({ weights[iWeight] });

		//recalculate highest
		double weightsSum{ 0 };
		for (unsigned i = 0; i < highestWeights.GetData().GetSize(); i++)
			weightsSum += highestWeights.GetData()[i].Amount;
		const double invSum{ weightsSum != 0 ? 1.0 / weightsSum : 0 };

		//store in blendData
		FbxClass::BlendData& blend{ blendData[iVertex] };
		const Weight* pHighestWeight{ &highestWeights.GetData()[highestWeights.GetData().GetSize() - 1] };
		for (unsigned iWeight = 0; iWeight < NR_WEIGHTS; iWeight++, pHighestWeight--)
		{
			blend.jointIdx[iWeight] = pHighestWeight->JointModelIdx == Scalar<int64_t>::MAX
				? -1
				: static_cast<int>(*jointLookup.Get(pHighestWeight->JointModelIdx));
			blend.weight[iWeight] = static_cast<float>(pHighestWeight->Amount * invSum);
		}
	}
}

void FbxClassWeights::CreateFromRootJoint(
	Array<FbxClass::BlendData>& blendData,
	const Geometry& wrappingGeometry,
	const FbxLoadData::JointLookup& jointLookup) const
{
	const Model& rootModel{ wrappingGeometry.GetRootModel() };
	const unsigned* pRootJointIdx{ jointLookup.Get(rootModel.GetId()) };
	if (!pRootJointIdx)
	{
		Logger::PrintError("[FbxClassWeights::CreateWeights] geometry does not have a skin-deformer nor a root-joint");
		return;
	}

	for (unsigned iVertex = 0; iVertex < blendData.GetSize(); iVertex++)
	{
		FbxClass::BlendData& weights{ blendData[iVertex] };
		weights.jointIdx[0] = static_cast<int>(*pRootJointIdx);
		weights.weight[0] = 1;
		for (unsigned iWeight = 1; iWeight < NR_WEIGHTS; iWeight++)
		{
			weights.jointIdx[iWeight] = -1;
			weights.weight[iWeight] = 0;
		}
	}
}
