#include "pch.h"
#include "Strip.h"

using namespace MeshMaker;

void Strip::EnsureEdgesSize(unsigned size)
{
	m_Edges.EnsureCapacity(size);
}

void Strip::EnsureNormalsSize(unsigned size)
{
	m_Normals.EnsureCapacity(size);
}

void Strip::AddEdge(Vertex bottom, Vertex top)
{
	m_Edges.Add({ bottom, top });
}

void Strip::AddNormal(const Float3& normal)
{
	m_Normals.Add(normal);
}

const MeshMaker::Line& Strip::GetEdge(unsigned i) const
{
	return m_Edges[i];
}
