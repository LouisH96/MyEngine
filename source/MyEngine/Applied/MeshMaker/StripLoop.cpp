#include "pch.h"
#include "StripLoop.h"

using namespace MeshMaker;

void StripLoop::AddEdge(Vertex bottom, Vertex top)
{
	m_Edges.Add({ bottom, top });
}

void StripLoop::AddEdge(Line line)
{
	m_Edges.Add(line);
}

void StripLoop::AddNormal(const Float3& normal)
{
	m_Normals.Add(normal);
}

void StripLoop::SetEdges(const List<Line>& edges)
{
	m_Edges = edges;
}

void StripLoop::SetNormals(const List<Float3>& normals)
{
	m_Normals = normals;
}

const MeshMaker::Line& StripLoop::GetEdge(unsigned i) const
{
	return m_Edges[i % m_Edges.GetSize()];
}
