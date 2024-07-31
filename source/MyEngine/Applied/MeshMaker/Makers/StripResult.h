#pragma once

#include <DataStructures\Array.h>
#include "..\MakerResult.h"

namespace MyEngine
{
namespace MeshMaker
{
#pragma region StripResult

#define CLASS StripResult
#define TEMPLATE template<typename TVertex, ModelTopology TTopology>
#define FULL_CLASS StripResult<TVertex, TTopology>

template<typename TVertex, ModelTopology TTopology>
class StripResult
	: public MakerResult<TVertex, TTopology>
{
public:
	void SetNrEdges(unsigned nrEdges);

	void SetPoint(unsigned iEdge, unsigned iEdgeEnd, unsigned vertexIndex);
	void SetEdgeBot(unsigned iEdge, unsigned vertexIndex);
	void SetEdgeTop(unsigned iEdge, unsigned vertexIndex);

	unsigned GetPoint(unsigned iEdge, unsigned iEdgeEnd);
	unsigned GetEdgeBot(unsigned iEdge);
	unsigned GetEdgeTop(unsigned iEdge);

	unsigned GetFirstEdgeBot() const;
	unsigned GetFirstEdgeTop() const;
	unsigned GetLastEdgeBot() const;
	unsigned GetLastEdgeTop() const;

private:
	Array<unsigned> m_Vertices;
};

TEMPLATE
inline void FULL_CLASS::SetNrEdges(unsigned nrEdges)
{
	m_Vertices = { nrEdges * 2 };
}

TEMPLATE
void FULL_CLASS::SetPoint(unsigned iEdge, unsigned iEdgeEnd, unsigned vertexIndex)
{
	m_Vertices[iEdge * 2 + iEdgeEnd] = vertexIndex;
}

TEMPLATE
void FULL_CLASS::SetEdgeBot(unsigned iEdge, unsigned vertexIndex)
{
	m_Vertices[iEdge * 2] = vertexIndex;
}

TEMPLATE
void FULL_CLASS::SetEdgeTop(unsigned iEdge, unsigned vertexIndex)
{
	m_Vertices[iEdge * 2 + 1] = vertexIndex;
}

TEMPLATE
unsigned FULL_CLASS::GetPoint(unsigned iEdge, unsigned iEdgeEnd)
{
	return m_Vertices[iEdge * 2 + iEdgeEnd];
}

TEMPLATE
unsigned FULL_CLASS::GetEdgeBot(unsigned iEdge)
{
	return m_Vertices[iEdge * 2];
}

TEMPLATE
unsigned FULL_CLASS::GetEdgeTop(unsigned iEdge)
{
	return m_Vertices[iEdge * 2 + 1];
}

TEMPLATE
unsigned FULL_CLASS::GetFirstEdgeBot() const
{
	return m_Vertices[0];
}

TEMPLATE
unsigned FULL_CLASS::GetFirstEdgeTop() const
{
	return m_Vertices[1];
}

TEMPLATE
unsigned FULL_CLASS::GetLastEdgeBot() const
{
	return m_Vertices[m_Vertices.GetSize() - 2];
}

TEMPLATE
unsigned FULL_CLASS::GetLastEdgeTop() const
{
	return m_Vertices[m_Vertices.GetSize() - 1];
}

#undef FULL_CLASS
#undef TEMPLATE
#undef CLASS

#pragma endregion

#pragma region DefaultStripResult

template<typename TVertex, ModelTopology TTopology>
class DefaultStripResult
	: public MakerResult<TVertex, TTopology>
{
public:
	void SetNrEdges(unsigned) {};
	void SetPoint(unsigned, unsigned, unsigned) {};
	void SetEdgeBot(unsigned, unsigned) {};
	void SetEdgeTop(unsigned, unsigned) {};
};

#pragma endregion
}
}