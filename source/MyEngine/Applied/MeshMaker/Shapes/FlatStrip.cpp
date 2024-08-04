#include "pch.h"
#include "FlatStrip.h"

using namespace MeshMaker;

void FlatStrip::EnsureEdgesSize(unsigned size)
{
    m_Edges.EnsureCapacity(size);
}

void FlatStrip::AddEdge(Vertex bottom, Vertex top)
{
    m_Edges.Add({ bottom, top });
}

void FlatStrip::SetNormal(const Float3& normal)
{
    m_Normal = normal;
}

void FlatStrip::SetEdgeBot(unsigned iEdge, Vertex vertex)
{
    m_Edges[iEdge][0] = vertex;
}

void FlatStrip::SetEdgeTop(unsigned iEdge, Vertex vertex)
{
    m_Edges[iEdge][1] = vertex;
}

const MeshMaker::Line& FlatStrip::GetEdge(unsigned i) const
{
    return m_Edges[i % m_Edges.GetSize()];
}
