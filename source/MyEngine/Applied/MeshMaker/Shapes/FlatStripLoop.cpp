#include "pch.h"
#include "FlatStripLoop.h"

using namespace MeshMaker;

FlatStripLoop::FlatStripLoop(unsigned edgesCapacity)
	: m_Edges{ edgesCapacity }
{
}

void FlatStripLoop::AddEdge(Vertex bottom, Vertex top)
{
	m_Edges.Add({ bottom, top });
}

void FlatStripLoop::AddEdge(Line line)
{
	m_Edges.Add(line);
}

void FlatStripLoop::SetNormal(const Float3& normal)
{
	m_Normal = normal;
}

void FlatStripLoop::SetEdges(const List<Line>& edges)
{
	m_Edges = edges;
}
