#include "pch.h"
#include "FbxData.h"

#include "FbxObject.h"
#include "FbxReader.h"
#include "Properties/FbxPropArray.h"
#include <Math/Float3.h>

#include "DataStructures/DsUtils.h"
#include "Debug/DebugRenderer.h"

using namespace Math;

Io::Fbx::FbxData::FbxData(const std::wstring& fbxPath)
	: FbxData(FbxReader{ fbxPath })
{}

Io::Fbx::FbxData::FbxData(FbxReader&& reader)
{
	FbxObject& objects{ *reader.GetRoot().GetChild("Objects") };
	FbxObject& geometry{ *objects.GetChild("Geometry") };

	auto models = objects.GetChildren("Model");

	for (int iModel = 0; iModel < models.size(); iModel++)
	{
		models[iModel]->Print();

		auto* pProps70 = models[iModel]->GetChild("Properties70");
		for (int iProp70 = 0; iProp70 < pProps70->GetChildren().size(); iProp70++)
		{
			auto* pProp70Child = pProps70->GetChildren()[iProp70];

			auto propName = pProp70Child->GetProperty(0)->AsString();
			if (propName != "Lcl Translation") continue;

			const double x = pProp70Child->GetProperty(4)->AsPrimitive<double>().GetValue();
			const double y = pProp70Child->GetProperty(5)->AsPrimitive<double>().GetValue();
			const double z = pProp70Child->GetProperty(6)->AsPrimitive<double>().GetValue();

			Debug::DebugRenderer::AddSphere(Float3{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) }/100.f, { 1,0,0 }, .1f);

			std::cout << "FOUND translation: " << x << ", " << y << ", " << z << std::endl;
		}
	}


	//POINTS
	const FbxObject& verticesObject{ *geometry.GetChild("Vertices") };
	const Array<double>& coordArray{ verticesObject.GetProperty(0)->AsArray<double>().GetValues() };
	m_Points = { coordArray.GetSize() / 3 };
	const double* pCoord = &coordArray[0];
	for (int i = 0; i < m_Points.GetSize(); i++)
		m_Points[i] = {
			static_cast<float>(*pCoord++),
			static_cast<float>(*pCoord++),
			static_cast<float>(*pCoord++) };

	//NORMALS
	const FbxObject& layerElementNormalObject{ *geometry.GetChild("LayerElementNormal") };
	const FbxObject& normalsObject{ *layerElementNormalObject.GetChild("Normals") };
	const Array<double>& normalDoublesArray{ normalsObject.GetProperty(0)->AsArray<double>().GetValues() };
	m_Normals = { coordArray.GetSize() / 3 };
	const double* pNormalsDouble = &normalDoublesArray[0];
	for (int i = 0; i < m_Normals.GetSize(); i++)
	{
		m_Normals[i] = {
			static_cast<float>(*pNormalsDouble++),
			static_cast<float>(*pNormalsDouble++),
			static_cast<float>(*pNormalsDouble++) };
	}

	//UVS
	LoadUvs(geometry);

	//INDICES
	FbxObject& indicesObject{ *geometry.GetChild("PolygonVertexIndex") };
	Array<int>& indices{ indicesObject.GetProperty(0)->AsArray<int>().GetValues() };
	m_Indices = std::move(indices);

	//MAKE TRIANGLE LIST
	MakeTriangleList();
}

void Io::Fbx::FbxData::MakeTriangleList()
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

void Io::Fbx::FbxData::LoadUvs(FbxObject& geometry)
{
	const FbxObject& layerElementUvObject{ *geometry.GetChild("LayerElementUV") };
	const Array<double>& uvValues{ layerElementUvObject.GetChild("UV")->GetProperty(0)->AsArray<double>().GetValues() };
	const Array<int>& uvIndices{ layerElementUvObject.GetChild("UVIndex")->GetProperty(0)->AsArray<int>().GetValues() };
	m_Uvs = { uvIndices.GetSize() };

	for (int i = 0; i < uvIndices.GetSize(); i++)
	{
		const int index = uvIndices[i];
		const double x = uvValues[index * 2];
		const double y = uvValues[index * 2 + 1];
		m_Uvs[i] = { static_cast<float>(x),
			static_cast<float>(1 - y) };
	}
}
