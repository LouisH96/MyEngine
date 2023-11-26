#include "Deformer.h"

#include "Io/Fbx/Reading/FbxElement.h"
#include "Io/Fbx/Reading/Properties/FbxPropPrimitive.h"
#include "Logger/Logger.h"

using namespace MyEngine::Io::Fbx::Wrapping;

DeformerSkinData::DeformerSkinData(const Reading::FbxElement& object)
	: Version{ object.GetChildProperty(0, 0).AsPrimitive<int>().GetValue() }
	, Accuracy{ object.GetChildProperty(1,0).AsPrimitive<double>().GetValue() }
{
	if (object.GetNrChildren() >= 3)
		SkinningType = object.GetChildProperty(2, 0).AsString();
}

IDeformerData* DeformerSkinData::Clone() const
{
	return new DeformerSkinData{ *this };
}

IDeformerData* DeformerClusterData::Clone() const
{
	return new DeformerClusterData{ *this };
}

DeformerClusterData::DeformerClusterData(Reading::FbxElement& object)
	: Version{ object.GetChildProperty(0,0).AsPrimitive<int>().GetValue() }
{
	unsigned next{ 2 };

	if (object.GetChild(next).GetName() == "Indexes")
	{
		Indexes = std::move(object.GetChildProperty(next, 0).AsArray<int>().GetValues());
		next++;
	}
	if (object.GetChild(next).GetName() == "Weights")
	{
		Weights = std::move(object.GetChildProperty(next, 0).AsArray<double>().GetValues());
		next++;
	}

	Transform = std::move(object.GetChildProperty(next++, 0).AsArray<double>().GetValues());
	TransformLink = std::move(object.GetChildProperty(next++, 0).AsArray<double>().GetValues());
}

Deformer::Deformer(Reading::FbxElement& object)
	: Id{ object.GetProperty(0).AsPrimitive<int64_t>().GetValue() }
{
	const std::string& type{ object.GetProperty(2).AsString() };

	if (type == "Cluster")
		m_pData = new DeformerClusterData(object);
	else if (type == "Skin")
		m_pData = new DeformerSkinData(object);
	else
		Logger::PrintWarning("[Deformer] unknown deformer type: " + type);
}

Deformer::~Deformer()
{
	delete m_pData;
	m_pData = nullptr;
}

Deformer::Deformer(const Deformer& other)
	: Id{ other.Id }
	, m_pData{ other.m_pData ? other.m_pData->Clone() : nullptr }
	, m_pParentGeometry{ other.m_pParentGeometry }
	, m_pParentDeformer{ other.m_pParentDeformer }
	, m_ChildDeformers{ other.m_ChildDeformers }
{
}

Deformer::Deformer(Deformer&& other) noexcept
	: Id{ other.Id }
	, m_pData{ other.m_pData }
	, m_pParentGeometry{ other.m_pParentGeometry }
	, m_pParentDeformer{ other.m_pParentDeformer }
	, m_ChildDeformers{ std::move(other.m_ChildDeformers) }
{
	other.Id = 0;
	other.m_pData = nullptr;
	other.m_pParentGeometry = nullptr;
	other.m_pParentDeformer = nullptr;
}

Deformer& Deformer::operator=(const Deformer& other)
{
	if (&other == this) return *this;

	Id = other.Id;
	m_pParentGeometry = other.m_pParentGeometry;
	m_pParentDeformer = other.m_pParentDeformer;
	m_ChildDeformers = other.m_ChildDeformers;

	delete m_pData;
	m_pData = other.m_pData ? other.m_pData->Clone() : nullptr;

	return *this;
}

Deformer& Deformer::operator=(Deformer&& other) noexcept
{
	if (&other == this) return *this;

	Id = other.Id;
	m_pData = other.m_pData;
	m_pParentGeometry = other.m_pParentGeometry;
	m_pParentDeformer = other.m_pParentDeformer;
	m_ChildDeformers = std::move(other.m_ChildDeformers);

	other.Id = 0;
	other.m_pData = nullptr;
	other.m_pParentGeometry = nullptr;
	other.m_pParentDeformer = nullptr;

	return *this;
}

void Deformer::SetParentGeometry(const Geometry& geometry)
{
	if (m_pParentDeformer || m_pParentGeometry)
		Logger::PrintError("Deformer already has a parent");
	m_pParentGeometry = &geometry;
}

void Deformer::SetParentDeformer(const Deformer& deformer)
{
	if (m_pParentDeformer || m_pParentGeometry)
		Logger::PrintError("Deformer already has a parent");
	m_pParentDeformer = &deformer;
}

void Deformer::AddChildDeformer(const Deformer& deformer)
{
	m_ChildDeformers.Add(&deformer);
}

void Deformer::SetModel(const Model& model)
{
	if (m_pModel)
		Logger::PrintError("[Deformer::SetModel] Model already set");
	m_pModel = &model;
}
