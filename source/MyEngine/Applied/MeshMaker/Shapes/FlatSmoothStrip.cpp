#include "pch.h"
#include "FlatSmoothStrip.h"

using namespace MeshMaker;

void FlatSmoothStrip::EnsureEdgesSize(unsigned size)
{
    m_Edges.EnsureCapacity(size);
}

void FlatSmoothStrip::AddEdge(Vertex bottom, Vertex top)
{
    m_Edges.Add({ bottom, top });
}

void FlatSmoothStrip::SetNormal(const Float3& normal)
{
    m_Normal = normal;
}

void FlatSmoothStrip::SetEdgeBot(unsigned iEdge, Vertex vertex)
{
    m_Edges[iEdge][0] = vertex;
}

void FlatSmoothStrip::SetEdgeTop(unsigned iEdge, Vertex vertex)
{
    m_Edges[iEdge][1] = vertex;
}

const MeshMaker::Line& FlatSmoothStrip::GetEdge(unsigned i) const
{
    return m_Edges[i % m_Edges.GetSize()];
}
