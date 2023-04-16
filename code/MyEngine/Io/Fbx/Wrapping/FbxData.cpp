#include "pch.h"
#include "FbxData.h"

#include <Debug/DebugRenderer.h>
#include <Math/Float3.h>
#include "../Reading/FbxObject.h"
#include "../Reading/FbxReader.h"
#include "../Reading/Properties/FbxPropArray.h"

using namespace Math;

Io::Fbx::Wrapping::FbxData::FbxData(const std::wstring& fbxPath)
	: FbxData(Fbx::Reading::FbxReader{ fbxPath })
{}

Io::Fbx::Wrapping::FbxData::FbxData(Reading::FbxReader&& reader)
{
	Reading::FbxObject& objects{ *reader.GetRoot().GetChild("Objects") };
	std::vector<Reading::FbxObject*> geometries{ objects.GetChildren("Geometry") };

	for(int i = 0; i < geometries.size(); i++)
	{
		//geometries[i]->Print();
		Geometry geomStruct{};
		LoadPoints(*geometries[i], geomStruct);
		LoadNormals(*geometries[i], geomStruct);
		LoadUvs(*geometries[i], geomStruct);
		LoadIndices(*geometries[i], geomStruct);
		m_Geometries.Add(geomStruct);
	}

	//FbxObject& geometry{ *objects.GetChild("Geometry") };

	//objects.Print();

	//auto models = objects.GetChildren("Model");
	//for (int iModel = 0; iModel < models.size(); iModel++)
	//{
	//	//models[iModel]->Print();

	//	auto* pProps70 = models[iModel]->GetChild("Properties70");
	//	for (int iProp70 = 0; iProp70 < pProps70->GetChildren().size(); iProp70++)
	//	{
	//		auto* pProp70Child = pProps70->GetChildren()[iProp70];

	//		auto propName = pProp70Child->GetProperty(0)->AsString();
	//		if (propName != "Lcl Translation") continue;

	//		const double x = pProp70Child->GetProperty(4)->AsPrimitive<double>().GetValue();
	//		const double y = pProp70Child->GetProperty(5)->AsPrimitive<double>().GetValue();
	//		const double z = pProp70Child->GetProperty(6)->AsPrimitive<double>().GetValue();

	//		Debug::DebugRenderer::AddSphere(Float3{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) }/100.f, { 1,0,0 }, .1f);

	//		//std::cout << "FOUND translation: " << x << ", " << y << ", " << z << std::endl;
	//	}
	//}
}

void Io::Fbx::Wrapping::FbxData::LoadPoints(Reading::FbxObject& fbxGeometry, Geometry& geometryStruct)
{
	const Reading::FbxObject& verticesObject{ *fbxGeometry.GetChild("Vertices") };
	const Array<double>& coordArray{ verticesObject.GetProperty(0)->AsArray<double>().GetValues() };
	geometryStruct.Points = { coordArray.GetSize() / 3 };
	const double* pCoord = &coordArray[0];
	for (int i = 0; i < geometryStruct.Points.GetSize(); i++)
		geometryStruct.Points[i] = {
			static_cast<float>(*pCoord++),
			static_cast<float>(*pCoord++),
			static_cast<float>(*pCoord++) };
}

void Io::Fbx::Wrapping::FbxData::LoadNormals(Reading::FbxObject& geometry, Geometry& geometryStruct)
{
	const Reading::FbxObject& layerElementNormalObject{ *geometry.GetChild("LayerElementNormal") };
	const Reading::FbxObject& normalsObject{ *layerElementNormalObject.GetChild("Normals") };
	const Array<double>& normalDoublesArray{ normalsObject.GetProperty(0)->AsArray<double>().GetValues() };
	geometryStruct.Normals = { normalDoublesArray.GetSize() / 3 };
	const double* pNormalsDouble = &normalDoublesArray[0];
	for (int i = 0; i < geometryStruct.Normals.GetSize(); i++)
	{
		geometryStruct.Normals[i] = {
			static_cast<float>(*pNormalsDouble++),
			static_cast<float>(*pNormalsDouble++),
			static_cast<float>(*pNormalsDouble++) };
	}
}

void Io::Fbx::Wrapping::FbxData::LoadIndices(Reading::FbxObject& geometry, Geometry& geometryStruct)
{
	Reading::FbxObject& indicesObject{ *geometry.GetChild("PolygonVertexIndex") };
	Array<int>& indices{ indicesObject.GetProperty(0)->AsArray<int>().GetValues() };
	geometryStruct.Indices = std::move(indices);
}

void Io::Fbx::Wrapping::FbxData::LoadUvs(Reading::FbxObject& geometry, Geometry& geometryStruct)
{
	const Reading::FbxObject& layerElementUvObject{ *geometry.GetChild("LayerElementUV") };
	const Array<double>& uvValues{ layerElementUvObject.GetChild("UV")->GetProperty(0)->AsArray<double>().GetValues() };
	const Array<int>& uvIndices{ layerElementUvObject.GetChild("UVIndex")->GetProperty(0)->AsArray<int>().GetValues() };
	geometryStruct.Uvs = { uvIndices.GetSize() };

	for (int i = 0; i < uvIndices.GetSize(); i++)
	{
		const int index = uvIndices[i];
		const double x = uvValues[index * 2];
		const double y = uvValues[index * 2 + 1];
		geometryStruct.Uvs[i] = { static_cast<float>(x),
			static_cast<float>(1 - y) };
	}
}
