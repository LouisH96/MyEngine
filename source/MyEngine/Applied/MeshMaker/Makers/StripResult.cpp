#include "pch.h"
#include "StripResult.h"

using namespace MyEngine::MeshMaker;

void StripResult::SetNrEdges(unsigned nrEdges)
{
	m_Vertices = { nrEdges * 2 };
}

void StripResult::SetPoint(unsigned iEdge, unsigned iEdgeEnd, unsigned vertexIndex)
{
	m_Vertices[iEdge * 2 + iEdgeEnd] = vertexIndex;
}

void StripResult::SetEdgeBot(unsigned iEdge, unsigned vertexIndex)
{
	m_Vertices[iEdge * 2 + 0] = vertexIndex;
}

void StripResult::SetEdgeTop(unsigned iEdge, unsigned vertexIndex)
{
	m_Vertices[iEdge * 2 + 1] = vertexIndex;
}

unsigned StripResult::GetPoint(unsigned iEdge, unsigned iEdgeEnd)
{
	return m_Vertices[iEdge * 2 + iEdgeEnd];
}

unsigned StripResult::GetEdgeBot(unsigned iEdge)
{
	return m_Vertices[iEdge * 2 + 0];
}

unsigned StripResult::GetEdgeTop(unsigned iEdge)
{
	return m_Vertices[iEdge * 2 + 1];
}
