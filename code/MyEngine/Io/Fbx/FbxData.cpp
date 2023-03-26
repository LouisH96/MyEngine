#include "pch.h"
#include "FbxData.h"

#include <Debug/DebugRenderer.h>
#include <Math/Float3.h>
#include "FbxObject.h"
#include "FbxReader.h"
#include "Properties/FbxPropArray.h"

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
		//models[iModel]->Print();

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

			//std::cout << "FOUND translation: " << x << ", " << y << ", " << z << std::endl;
		}
	}

	LoadPoints(geometry);
	LoadNormals(geometry);
	LoadUvs(geometry);
	LoadIndices(geometry);
}

void Io::Fbx::FbxData::LoadPoints(FbxObject& geometry)
{
	const FbxObject& verticesObject{ *geometry.GetChild("Vertices") };
	const Array<double>& coordArray{ verticesObject.GetProperty(0)->AsArray<double>().GetValues() };
	m_Points = { coordArray.GetSize() / 3 };
	const double* pCoord = &coordArray[0];
	for (int i = 0; i < m_Points.GetSize(); i++)
		m_Points[i] = {
			static_cast<float>(*pCoord++),
			static_cast<float>(*pCoord++),
			static_cast<float>(*pCoord++) };
}

void Io::Fbx::FbxData::LoadNormals(FbxObject& geometry)
{
	const FbxObject& layerElementNormalObject{ *geometry.GetChild("LayerElementNormal") };
	const FbxObject& normalsObject{ *layerElementNormalObject.GetChild("Normals") };
	const Array<double>& normalDoublesArray{ normalsObject.GetProperty(0)->AsArray<double>().GetValues() };
	m_Normals = { normalDoublesArray.GetSize() / 3 };
	const double* pNormalsDouble = &normalDoublesArray[0];
	for (int i = 0; i < m_Normals.GetSize(); i++)
	{
		m_Normals[i] = {
			static_cast<float>(*pNormalsDouble++),
			static_cast<float>(*pNormalsDouble++),
			static_cast<float>(*pNormalsDouble++) };
	}
}

void Io::Fbx::FbxData::LoadIndices(FbxObject& geometry)
{
	FbxObject& indicesObject{ *geometry.GetChild("PolygonVertexIndex") };
	Array<int>& indices{ indicesObject.GetProperty(0)->AsArray<int>().GetValues() };
	m_Indices = std::move(indices);
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
