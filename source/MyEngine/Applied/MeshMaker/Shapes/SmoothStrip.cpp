#include "pch.h"
#include "SmoothStrip.h"

using namespace MeshMaker;

void SmoothStrip::EnsureEdgeSize(unsigned size)
{
    m_Edges.EnsureCapacity(size);
}

void SmoothStrip::AddEdge(Vertex bottom, Vertex top)
{
    m_Edges.Add({ bottom, top });
}

void SmoothStrip::SetNormal(const Float3& normal)
{
    m_Normal = normal;
}

const MeshMaker::Line& SmoothStrip::GetEdge(unsigned i) const
{
    return m_Edges[i % m_Edges.GetSize()];
}
