#include "FbxClass.h"

#include "FbxLoadData.h"
#include "ClassInt/FbxClassWeights.h"
#include "DataStructures/Dictionary.h"
#include "DataStructures/DsUtils.h"
#include "Transform/WorldMatrix.h"
#include "Wrapping/FbxData.h"
#include "Wrapping/Model.h"

using namespace MyEngine;
using namespace Io::Fbx;
using namespace Wrapping;

FbxClass::FbxClass(const std::wstring& path, float scale, bool print)
	: FbxClass{ FbxData{path, print}, scale }
{
}

FbxClass::FbxClass(FbxData&& data, float scale)
{
	FbxLoadData loadData;
	loadData.Orientation = FbxOrientation{ scale };
	loadData.pFbxClass = this;
	loadData.pFbxData = &data;

	FbxClassWeights weightsManager{ data.GetGeometries() };

	m_Animations = { data.GetAnimationStacks().GetSize() };
	for (unsigned i = 0; i < m_Animations.GetSize(); i++)
		m_Animations[i] = FbxAnimation{ data.GetAnimationStacks()[i] };

	m_Skeleton = FbxSkeleton{ loadData };

	m_Geometries = { data.GetGeometries().GetSize() };
	for (unsigned iGeom = 0; iGeom < m_Geometries.GetSize(); iGeom++)
	{
		Geometry& modelGeometry = m_Geometries[iGeom];
		Wrapping::Geometry& dataGeometry = data.GetGeometries()[iGeom];
		const Model& rootModel{ dataGeometry.GetRootModel() };
		modelGeometry.Indices = std::move(dataGeometry.GetIndices());
		modelGeometry.Normals = std::move(dataGeometry.GetNormals());
		modelGeometry.Points = std::move(dataGeometry.GetPoints());
		modelGeometry.Uvs = std::move(dataGeometry.GetUvs());
		modelGeometry.Name = std::move(dataGeometry.GetName());
		modelGeometry.RotationOffset = rootModel.GetRotationOffset();
		modelGeometry.RotationPivot = rootModel.GetRotationPivot();
		modelGeometry.Weights = { modelGeometry.Points.GetSize() };

		const unsigned* pModelJoint{ loadData.ModelToJoint.Get(rootModel.GetId()) };
		const FbxJoint* pRootJoint{ pModelJoint ? &m_Skeleton.GetJoint(*pModelJoint) : nullptr };

		Float4X4 transform;
		if (pRootJoint)
			transform = WorldMatrix::GetInversed(pRootJoint->GetBindTransform());
		else
			transform = loadData.Orientation.MakeLocalTransform(rootModel).AsMatrix();

		const Float3 offset{
			rootModel.GetGeometricTranslation()
		};

		//Translate & Scale
		for (unsigned iPoint = 0; iPoint < modelGeometry.Points.GetSize(); iPoint++)
		{
			Float3& point{ modelGeometry.Points[iPoint] };
			point = loadData.Orientation.ConvertPoint(point);
			point += offset * loadData.Orientation.GetScale();
			WorldMatrix::TransformPoint(transform, point);
		}

		for (unsigned iNormal = 0; iNormal < modelGeometry.Normals.GetSize(); iNormal++)
		{
			Float3& normal{ modelGeometry.Normals[iNormal] };
			normal.x = -normal.x;
			WorldMatrix::RotatePoint(transform, normal);
		}

		//Weights
		weightsManager.CreateWeights(
			modelGeometry.Weights,
			dataGeometry,
			loadData.ModelToJoint);
	}

	for (unsigned i = 0; i < m_Geometries.GetSize(); i++)
		MakeTriangleList(m_Geometries[i], loadData.Orientation);
}

int FbxClass::GetNrOfAnimationLayers() const
{
	int total = 0;
	for (unsigned i = 0; i < m_Animations.GetSize(); i++)
		total += m_Animations[i].GetNrLayers();
	return total;
}

void FbxClass::Attach(const FbxClass& object, const std::string& jointName)
{
	unsigned jointIdx{};
	if (!GetSkeleton().FindJointIdx(jointIdx, jointName))
	{
		Logger::Warning("[FbxClass::Attach] Target joint not found");
		return;
	}
	Attach(object, jointIdx);
}

void FbxClass::Attach(const FbxClass& object, int jointIdx)
{
	//Get
	FbxClass::Geometry& modelGeom{ m_Geometries.First() };
	const FbxJoint& joint{ GetSkeleton().GetJoint(jointIdx) };
	const Float4X4 parentMatrix{ joint.CalculatePoseMatrix() };

	//Ensure Array-Size
	const unsigned nrOldPoints{ modelGeom.Points.GetSize() };
	const unsigned nrOldNormals{ modelGeom.Normals.GetSize() };
	const unsigned nrOldUvs{ modelGeom.Uvs.GetSize() };
	const unsigned nrOldIndices{ modelGeom.Indices.GetSize() };
	const unsigned nrOldWeights{ modelGeom.Weights.GetSize() };
	unsigned nrNewPoints{ 0 };
	unsigned nrNewNormals{ 0 };
	unsigned nrNewUvs{ 0 };
	unsigned nrNewIndices{ 0 };
	unsigned nrNewWeights{ 0 };
	for (unsigned iGeom{ 0 }; iGeom < object.GetGeometries().GetSize(); ++iGeom)
	{
		const FbxClass::Geometry& geom{ object.GetGeometry(iGeom) };
		nrNewPoints += geom.Points.GetSize();
		nrNewNormals += geom.Normals.GetSize();
		nrNewUvs += geom.Uvs.GetSize();
		nrNewIndices += geom.Indices.GetSize();
		nrNewWeights += geom.Weights.GetSize();
	}
	modelGeom.Points.IncreaseSizeWith(nrNewPoints);
	modelGeom.Normals.IncreaseSizeWith(nrNewNormals);
	modelGeom.Uvs.IncreaseSizeWith(nrNewUvs);
	modelGeom.Indices.IncreaseSizeWith(nrNewIndices);
	modelGeom.Weights.IncreaseSizeWith(nrNewWeights);

	//Copy & Transform data
	for (unsigned iGeom{ 0 }; iGeom < object.GetGeometries().GetSize(); ++iGeom)
	{
		const FbxClass::Geometry& geom{ object.GetGeometry(iGeom) };
		//Points
		for (unsigned iPoint{ 0 }; iPoint < geom.Points.GetSize(); ++iPoint)
		{
			modelGeom.Points[nrOldPoints + iPoint] =
				parentMatrix.AppliedToPoint(geom.Points[iPoint]);
		}
		//Normals
		for (unsigned iNormal{ 0 }; iNormal < geom.Normals.GetSize(); ++iNormal)
		{
			modelGeom.Normals[nrOldNormals + iNormal] =
				parentMatrix.AppliedToVector(geom.Normals[iNormal]);
		}
		//Uvs
		std::copy(&geom.Uvs.First(), geom.Uvs.End(), &modelGeom.Uvs[nrOldUvs]);
		//Indices
		for (unsigned iIndex{ 0 }; iIndex < geom.Indices.GetSize(); ++iIndex)
			modelGeom.Indices[nrOldIndices + iIndex] = nrOldPoints + geom.Indices[iIndex];
		//Weights
		for (unsigned iWeight{ 0 }; iWeight < geom.Weights.GetSize(); ++iWeight)
		{
			BlendData& weights{ modelGeom.Weights[nrOldWeights + iWeight] };
			weights.jointIdx[0] = jointIdx;
			weights.weight[0] = 1.f;
			for (unsigned iOtherWeight{ 1 }; iOtherWeight < BlendData::NR_WEIGHTS; ++iOtherWeight)
			{
				weights.jointIdx[iOtherWeight] = -1;
				weights.weight[iOtherWeight] = 0;
			}
		}
	}
}

void FbxClass::MakeTriangleList(Geometry& geomStruct, const FbxOrientation& orientation)
{
	std::vector<Float3> positions{};
	std::vector<Float3> normals{};
	std::vector<Float2> uvs{};
	List<BlendData> weights{ geomStruct.Indices.GetSize() };
	positions.reserve(geomStruct.Indices.GetSize());
	normals.reserve(geomStruct.Normals.GetSize());
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
		normals.push_back(geomStruct.Normals[*pFirst]);
		normals.push_back(geomStruct.Normals[*pSecond]);
		normals.push_back(geomStruct.Normals[index2]);
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
	geomStruct.Normals = DsUtils::ToArray(normals);
	geomStruct.Indices = { 0 };
	geomStruct.Weights = weights.ToArray();

	//remove smoothing groups
	/*geomStruct.Normals = { geomStruct.Points.GetSize() };
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

	}*/
}

Int4 FbxClass::BlendData::IndicesAsInt4() const
{
	return Int4{
		jointIdx[0], jointIdx[1],
		jointIdx[2], jointIdx[3]
	};
}

Float4 FbxClass::BlendData::WeightsAsFloat4() const
{
	return Float4{
		weight[0], weight[1],
		weight[2], weight[3]
	};
}
