#include "pch.h"
#include "FbxModel.h"

#include "FbxData.h"
#include "DataStructures/DsUtils.h"
#include <Math/Float3.h>

using namespace Math;

Io::Fbx::FbxModel::FbxModel(const std::wstring& path)
{
	FbxData data{ path };
	m_Indices = std::move(data.GetIndices());
	m_Normals = std::move(data.GetNormals());
	m_Points = std::move(data.GetPoints());
	m_Uvs = std::move(data.GetUvs());

	MakeTriangleList();
}
void Io::Fbx::FbxModel::MakeTriangleList()
{
	std::vector<Float3> positions{};
	std::vector<Float3> normals{};
	std::vector<Float2> uvs{};
	positions.reserve(m_Indices.GetSize());
	uvs.reserve(m_Indices.GetSize());

	int index0 = 0;
	int index1 = 1;
	int index2 = 2;
	while (index2 < m_Indices.GetSize())
	{
		uvs.push_back(m_Uvs[index0]);
		uvs.push_back(m_Uvs[index1]);
		uvs.push_back(m_Uvs[index2]);
		const int pointIdx0 = m_Indices[index0];
		const int pointIdx1 = m_Indices[index1];
		positions.push_back(m_Points[pointIdx0]);
		positions.push_back(m_Points[pointIdx1]);

		int pointIdx2 = m_Indices[index2];
		if (pointIdx2 >= 0)
		{
			positions.push_back(m_Points[pointIdx2]);
			index1 = index2;
			index2++;
			continue;
		}
		pointIdx2 = -pointIdx2 - 1;
		positions.push_back(m_Points[pointIdx2]);
		index0 = index2 + 1;
		index1 = index0 + 1;
		index2 += 3;
	}

	m_Points = DsUtils::ToArray(positions);
	m_Uvs = DsUtils::ToArray(uvs);
	m_Indices = { 0 };

	//create normals
	m_Normals = { m_Points.GetSize() };
	for (int i = 0; i < m_Points.GetSize(); i += 3)
	{
		const Float3& point0{ m_Points[i + 0] };
		const Float3& point1{ m_Points[i + 1] };
		const Float3& point2{ m_Points[i + 2] };
		const Float3 edge1{ point1 - point0 };
		const Float3 edge2{ point2 - point0 };
		const Float3 normal{ edge1.Cross(edge2).Normalized() };
		m_Normals[i + 0] = normal;
		m_Normals[i + 1] = normal;
		m_Normals[i + 2] = normal;
	}
}
