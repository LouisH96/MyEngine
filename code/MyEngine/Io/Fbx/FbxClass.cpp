#include "pch.h"
#include "FbxClass.h"

#include "Wrapping/FbxData.h"
#include "Wrapping/Model.h"
#include "DataStructures/DsUtils.h"
#include <Math/Float3.h>

using namespace Math;
using namespace Io::Fbx::Wrapping;

Io::Fbx::FbxClass::FbxClass(const std::wstring& path)
{
	FbxData data{ path };
	m_Geometries = { data.GetGeometries().GetSize() };
	for (int i = 0; i < m_Geometries.GetSize(); i++)
	{
		Wrapping::Geometry& dataGeometry = data.GetGeometries()[i];
		Model& dataModel = data.GetModel(i);
		Geometry& modelGeometry = m_Geometries[i];
		modelGeometry.Indices = std::move(dataGeometry.GetIndices());
		modelGeometry.Normals = std::move(dataGeometry.GetNormals());
		modelGeometry.Points = std::move(dataGeometry.GetPoints());
		modelGeometry.Uvs = std::move(dataGeometry.GetUvs());
		modelGeometry.Name = std::move(dataModel.GetName());
		modelGeometry.RotationOffset = dataModel.GetRotationOffset();
		modelGeometry.RotationPivot = dataModel.GetRotationPivot();
	}

	for (int i = 0; i < m_Geometries.GetSize(); i++)
		MakeTriangleList(m_Geometries[i]);

	m_Skeleton = FbxSkeleton{ data };
}

void Io::Fbx::FbxClass::MakeTriangleList(Geometry& geomStruct)
{
	std::vector<Float3> positions{};
	std::vector<Float3> normals{};
	std::vector<Float2> uvs{};
	positions.reserve(geomStruct.Indices.GetSize());
	uvs.reserve(geomStruct.Indices.GetSize());

	int index0 = 0;
	int index1 = 1;
	int index2 = 2;
	while (index2 < geomStruct.Indices.GetSize())
	{
		//fbx to right-hand side, this program lhs
		uvs.push_back(geomStruct.Uvs[index1]);
		uvs.push_back(geomStruct.Uvs[index0]);
		uvs.push_back(geomStruct.Uvs[index2]);
		const int pointIdx0 = geomStruct.Indices[index0];
		const int pointIdx1 = geomStruct.Indices[index1];
		positions.push_back(geomStruct.Points[pointIdx1]);
		positions.push_back(geomStruct.Points[pointIdx0]);

		int pointIdx2 = geomStruct.Indices[index2];
		if (pointIdx2 >= 0)
		{
			positions.push_back(geomStruct.Points[pointIdx2]);
			index1 = index2;
			index2++;
			continue;
		}
		pointIdx2 = -pointIdx2 - 1;
		positions.push_back(geomStruct.Points[pointIdx2]);
		index0 = index2 + 1;
		index1 = index0 + 1;
		index2 += 3;
	}

	geomStruct.Points = DsUtils::ToArray(positions);
	geomStruct.Uvs = DsUtils::ToArray(uvs);
	geomStruct.Indices = { 0 };

	//create normals
	geomStruct.Normals = { geomStruct.Points.GetSize() };
	for (int i = 0; i < geomStruct.Points.GetSize(); i += 3)
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
