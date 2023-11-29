#include "FbxClass.h"

#include "FbxLoadData.h"
#include "DataStructures/Dictionary.h"
#include "DataStructures/DsUtils.h"
#include "Wrapping/FbxData.h"
#include "Wrapping/Model.h"

using namespace MyEngine::Io::Fbx;
using namespace Wrapping;

FbxClass::FbxClass(const std::wstring& path, float scale)
	: FbxClass{ FbxData{path}, scale }
{
}

FbxClass::FbxClass(FbxData&& data, float scale)
{
	FbxLoadData loadData;
	loadData.Scale = scale;
	loadData.pFbxClass = this;
	loadData.pFbxData = &data;

	if (data.GetARootLimbNode())
	{
		m_Animations = { data.GetAnimationStacks().GetSize() };
		for (unsigned i = 0; i < m_Animations.GetSize(); i++)
			m_Animations[i] = FbxAnimation{ data.GetAnimationStacks()[i] };

		m_Skeleton = FbxSkeleton{ loadData, data.GetOrientation() };
	}

	m_Geometries = { data.GetGeometries().GetSize() };
	for (unsigned iGeom = 0; iGeom < m_Geometries.GetSize(); iGeom++)
	{
		Wrapping::Geometry& dataGeometry = data.GetGeometries()[iGeom];
		Model& dataModel = data.GetModel(iGeom);
		Geometry& modelGeometry = m_Geometries[iGeom];
		modelGeometry.Indices = std::move(dataGeometry.GetIndices());
		modelGeometry.Normals = std::move(dataGeometry.GetNormals());
		modelGeometry.Points = std::move(dataGeometry.GetPoints());
		modelGeometry.Uvs = std::move(dataGeometry.GetUvs());
		modelGeometry.Name = std::move(dataModel.GetName());
		modelGeometry.RotationOffset = dataModel.GetRotationOffset();
		modelGeometry.RotationPivot = dataModel.GetRotationPivot();
		modelGeometry.Weights = Array<List<BlendData>>{ modelGeometry.Points.GetSize() };

		const Model& rootModel{ dataGeometry.GetRootModel() };
		const Float3 offset{
			rootModel.GetGeometricTranslation() -
			rootModel.MakeLocalTransform(loadData.Scale).LocalToWorld(rootModel.GetLclTranslation())
		};

		//Translate & Scale
		for (unsigned iPoint = 0; iPoint < modelGeometry.Points.GetSize(); iPoint++)
		{
			modelGeometry.Points[iPoint] += offset;
			modelGeometry.Points[iPoint] *= loadData.Scale;
		}

		//Weights
		const Deformer* pSkinDeformer{ dataGeometry.GetSkinDeformer() };
		if (pSkinDeformer)
		{
			for (unsigned iCluster = 0; iCluster < pSkinDeformer->GetChildDeformers().GetSize(); iCluster++)
			{
				const Deformer& deformer{ *pSkinDeformer->GetChildDeformers()[iCluster] };
				if (!deformer.HasClusterData())
				{
					Logger::PrintWarning("[FbxClass] child-deformer isn't a cluster-deformer");
					continue;
				}
				const DeformerClusterData& clusterData{ deformer.GetClusterData() };
				const FbxJoint& joint{ m_Skeleton.GetJoint(*loadData.ModelToJoint.Get(deformer.GetModel()->GetId())) };

				for (unsigned iVertex = 0; iVertex < clusterData.Indexes.GetSize(); iVertex++)
				{
					const int vertexIdx{ clusterData.Indexes[iVertex] };
					const double weight{ clusterData.Weights[iVertex] };
					modelGeometry.Weights[vertexIdx].Add(BlendData{ &joint, weight });
				}
			}
		}

		//find most weights
		//(found up to 20..., low influence however)
		unsigned most{ 0 };
		for (unsigned iWeight = 0; iWeight < modelGeometry.Weights.GetSize(); iWeight++)
		{
			const unsigned nrWeights{ modelGeometry.Weights[iWeight].GetSize() };
			most = Uint::Max(most, nrWeights);
		}
		Logger::Print("[FbxClass] Most weights", most);
		Logger::Print("[FbxClass] Nr joints", m_Skeleton.GetNrJoints()); //found up to 88
	}

	for (unsigned i = 0; i < m_Geometries.GetSize(); i++)
		MakeTriangleList(m_Geometries[i], data.GetOrientation());
}

int FbxClass::GetNrOfAnimationLayers() const
{
	int total = 0;
	for (unsigned i = 0; i < m_Animations.GetSize(); i++)
		total += m_Animations[i].GetNrLayers();
	return total;
}

void FbxClass::MakeTriangleList(Geometry& geomStruct, const Wrapping::FbxOrientation& orientation)
{
	std::vector<Float3> positions{};
	std::vector<Float3> normals{};
	std::vector<Float2> uvs{};
	List<List<BlendData>> weights{ geomStruct.Indices.GetSize() };
	positions.reserve(geomStruct.Indices.GetSize());
	uvs.reserve(geomStruct.Indices.GetSize());

	int index0 = 0;
	int index1 = 1;
	int index2 = 2;

	int* pFirst;
	int* pSecond;
	if (orientation.HasClockwiseWinding())
	{
		pFirst = &index0;
		pSecond = &index1;
	}
	else
	{
		pFirst = &index1;
		pSecond = &index0;
	}

	while (index2 < static_cast<int>(geomStruct.Indices.GetSize()))
	{
		//fbx to right-hand side, this program lhs
		uvs.push_back(geomStruct.Uvs[*pFirst]);
		uvs.push_back(geomStruct.Uvs[*pSecond]);
		uvs.push_back(geomStruct.Uvs[index2]);
		const int pointIdx0 = static_cast<int>(geomStruct.Indices[*pFirst]);
		const int pointIdx1 = static_cast<int>(geomStruct.Indices[*pSecond]);
		positions.push_back(geomStruct.Points[pointIdx0]);
		positions.push_back(geomStruct.Points[pointIdx1]);
		weights.Add(geomStruct.Weights[pointIdx0]);
		weights.Add(geomStruct.Weights[pointIdx1]);

		int pointIdx2 = static_cast<int>(geomStruct.Indices[index2]);
		if (pointIdx2 >= 0)
		{
			positions.push_back(geomStruct.Points[pointIdx2]);
			weights.Add(geomStruct.Weights[pointIdx2]);
			index1 = index2;
			index2++;
			continue;
		}
		pointIdx2 = -pointIdx2 - 1;
		positions.push_back(geomStruct.Points[pointIdx2]);
		weights.Add(geomStruct.Weights[pointIdx2]);
		index0 = index2 + 1;
		index1 = index0 + 1;
		index2 += 3;
	}

	geomStruct.Points = DsUtils::ToArray(positions);
	geomStruct.Uvs = DsUtils::ToArray(uvs);
	geomStruct.Indices = { 0 };
	geomStruct.Weights = weights.ToArray();

	//create normals
	geomStruct.Normals = { geomStruct.Points.GetSize() };
	for (unsigned i = 0; i < geomStruct.Points.GetSize(); i += 3)
	{
		const Float3& point0{ geomStruct.Points[i + 0] };
		const Float3& point1{ geomStruct.Points[i + 1] };
		const Float3& point2{ geomStruct.Points[i + 2] };
		const Float3 edge1{ point1 - point0 };
		const Float3 edge2{ point2 - point0 };
		const Float3 normal{ edge1.Cross(edge2).Normalized() };
		geomStruct.Normals[i + 0] = normal;
		geomStruct.Normals[i + 1] = normal;
		geomStruct.Normals[i + 2] = normal;
	}
}
