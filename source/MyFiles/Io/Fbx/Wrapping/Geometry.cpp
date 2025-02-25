#include "Geometry.h"

#include "Deformer.h"
#include "Model.h"
#include "Io/Fbx/Reading/FbxElement.h"
#include "Io/Fbx/Reading/Properties/FbxPropArray.h"
#include "Logger/Logger.h"

using namespace MyEngine::Io::Fbx::Wrapping;

Geometry::Geometry(Reading::FbxElement& geometryObject)
	: m_Id{ geometryObject.GetProperty(0).AsPrimitive<int64_t>().GetValue() }
{
	LoadPoints(geometryObject);
	LoadNormals(geometryObject);
	LoadIndices(geometryObject);
	LoadUvs(geometryObject);
}

int64_t Geometry::GetRootModelId() const
{
	return m_pRootModel->GetId();
}

void Geometry::SetRootModel(const Model& model)
{
	if (m_pRootModel)
		Logger::Error("RootModel of geometry already set");
	m_pRootModel = &model;
	m_Name = model.GetName();
}

void Geometry::AddDeformer(const Deformer& deformer)
{
	m_Deformers.Add(&deformer);
}

const Deformer* Geometry::GetSkinDeformer() const
{
	for (unsigned i = 0; i < m_Deformers.GetSize(); i++)
		if (m_Deformers[i]->HasSkinData())
			return m_Deformers[i];
	return nullptr;
}

void Geometry::LoadPoints(const Reading::FbxElement& geometryObject)
{
	const Reading::FbxElement& verticesObject{ *geometryObject.GetChild("Vertices") };
	const Array<double>& coordArray{ verticesObject.GetProperty(0).AsArray<double>().GetValues() };
	m_Points = { coordArray.GetSize() / 3 };
	const double* pPoint{ &coordArray[0] };

	for (unsigned i = 0; i < m_Points.GetSize(); i++, pPoint += 3)
		m_Points[i] = {
		static_cast<float>(pPoint[0]),
		static_cast<float>(pPoint[1]),
		static_cast<float>(pPoint[2])
	};
}

void Geometry::LoadNormals(const Reading::FbxElement& geometryObject)
{
	const Reading::FbxElement& layerElementNormalObject{ *geometryObject.GetChild("LayerElementNormal") };
	const Reading::FbxElement& normalsObject{ *layerElementNormalObject.GetChild("Normals") };
	const Array<double>& normalDoublesArray{ normalsObject.GetProperty(0).AsArray<double>().GetValues() };
	m_Normals = { normalDoublesArray.GetSize() / 3 };
	const double* pNormalsDouble = &normalDoublesArray[0];
	for (unsigned i = 0; i < m_Normals.GetSize(); i++)
	{
		m_Normals[i] = {
			static_cast<float>(*pNormalsDouble++),
			static_cast<float>(*pNormalsDouble++),
			static_cast<float>(*pNormalsDouble++) };
	}
}

void Geometry::LoadIndices(Reading::FbxElement& geometryObject)
{
	Reading::FbxElement& indicesObject{ *geometryObject.GetChild("PolygonVertexIndex") };

	indicesObject.MovePropertyTo(0, m_Indices);
}

void Geometry::LoadUvs(Reading::FbxElement& geometryObject)
{
	Reading::FbxElement& layerElementUvObject{ *geometryObject.GetChild("LayerElementUV") };
	Reading::FbxElement& uvIndexElement{ *layerElementUvObject.GetChild("UVIndex") };

	const Array<double>& uvValues{ layerElementUvObject.GetChild("UV")->GetProperty(0).AsArray<double>().GetValues() };
	Array<unsigned> uvIndices{};

	uvIndexElement.MovePropertyTo(0, uvIndices);

	m_Uvs = { uvIndices.GetSize() };

	for (unsigned i = 0; i < uvIndices.GetSize(); i++)
	{
		const unsigned index = uvIndices[i];
		const double x = uvValues[index * 2];
		const double y = uvValues[index * 2 + 1];
		m_Uvs[i] = { static_cast<float>(x),
			static_cast<float>(1 - y) };
	}
}
