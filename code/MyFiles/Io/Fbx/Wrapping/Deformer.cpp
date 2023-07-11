#include "Deformer.h"

#include "Io/Fbx/Reading/FbxElement.h"
#include "Io/Fbx/Reading/Properties/FbxPropPrimitive.h"
#include "Logger/Logger.h"

MyEngine::Io::Fbx::Wrapping::Deformer::Deformer(Reading::FbxElement& object)
	: Id{ object.GetProperty(0).AsPrimitive<int64_t>().GetValue() }
{
}

void MyEngine::Io::Fbx::Wrapping::Deformer::SetParentGeometry(const Geometry& geometry)
{
	if (m_pParentDeformer || m_pParentGeometry)
		Logger::PrintError("Deformer already has a parent");
	m_pParentGeometry = &geometry;
}

void MyEngine::Io::Fbx::Wrapping::Deformer::SetParentDeformer(const Deformer& deformer)
{
	if (m_pParentDeformer || m_pParentGeometry)
		Logger::PrintError("Deformer already has a parent");
	m_pParentDeformer = &deformer;
}

void MyEngine::Io::Fbx::Wrapping::Deformer::AddChildDeformer(const Deformer& deformer)
{
	m_ChildDeformers.Add(&deformer);
}
