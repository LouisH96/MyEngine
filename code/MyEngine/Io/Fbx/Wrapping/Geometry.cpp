#include "pch.h"
#include "Geometry.h"

#include "Io/Fbx/Reading/FbxObject.h"
#include "Io/Fbx/Reading/Properties/FbxPropArray.h"

Io::Fbx::Wrapping::Geometry::Geometry(Reading::FbxObject& geometryObject)
	: m_Id{geometryObject.GetProperty(0)->AsPrimitive<int64_t>().GetValue()}
{
	LoadPoints(geometryObject);
	LoadNormals(geometryObject);
	LoadIndices(geometryObject);
	LoadUvs(geometryObject);
}

void Io::Fbx::Wrapping::Geometry::SetRootModel(const Model& model)
{
	if (m_pRootModel)
		Logger::PrintError("RootModel of geometry already set");
	m_pRootModel = &model;
}

void Io::Fbx::Wrapping::Geometry::AddDeformer(const Deformer& deformer)
{
	m_Deformers.Add(&deformer);
}

void Io::Fbx::Wrapping::Geometry::LoadPoints(const Reading::FbxObject& geometryObject)
{
	const Reading::FbxObject& verticesObject{ *geometryObject.GetChild("Vertices") };
	const Array<double>& coordArray{ verticesObject.GetProperty(0)->AsArray<double>().GetValues() };
	m_Points = { coordArray.GetSize() / 3 };
	const double* pCoord = &coordArray[0];
	for (int i = 0; i < m_Points.GetSize(); i++)
		m_Points[i] = {
			-static_cast<float>(*pCoord++),
			static_cast<float>(*pCoord++),
			static_cast<float>(*pCoord++) };
}

void Io::Fbx::Wrapping::Geometry::LoadNormals(const Reading::FbxObject& geometryObject)
{
	const Reading::FbxObject& layerElementNormalObject{ *geometryObject.GetChild("LayerElementNormal") };
	const Reading::FbxObject& normalsObject{ *layerElementNormalObject.GetChild("Normals") };
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

void Io::Fbx::Wrapping::Geometry::LoadIndices(Reading::FbxObject& geometryObject)
{
	Reading::FbxObject& indicesObject{ *geometryObject.GetChild("PolygonVertexIndex") };
	Array<int>& indices{ indicesObject.GetProperty(0)->AsArray<int>().GetValues() };
	m_Indices = std::move(indices);
}

void Io::Fbx::Wrapping::Geometry::LoadUvs(const Reading::FbxObject& geometryObject)
{
	const Reading::FbxObject& layerElementUvObject{ *geometryObject.GetChild("LayerElementUV") };
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
