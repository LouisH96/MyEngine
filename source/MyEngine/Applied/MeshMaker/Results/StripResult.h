#pragma once

#include <DataStructures\Array.h>
#include "MakerResult.h"

namespace MyEngine
{
namespace MeshMaker
{
#pragma region StripResult

#define TEMP_DEF template<typename TVertex, ModelTopology TTopology>
#define TEMP_ARG TVertex, TTopology

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

TEMP_DEF
inline void StripResult<TEMP_ARG>::SetNrEdges(unsigned nrEdges)
{
	m_Vertices = { nrEdges * 2 };
}

TEMP_DEF
inline void StripResult<TEMP_ARG>::SetPoint(unsigned iEdge, unsigned iEdgeEnd, unsigned vertexIndex)
{
	m_Vertices[iEdge * 2 + iEdgeEnd] = vertexIndex;
}

TEMP_DEF
inline void StripResult<TEMP_ARG>::SetEdgeBot(unsigned iEdge, unsigned vertexIndex)
{
	m_Vertices[iEdge * 2] = vertexIndex;
}

TEMP_DEF
inline void StripResult<TEMP_ARG>::SetEdgeTop(unsigned iEdge, unsigned vertexIndex)
{
	m_Vertices[iEdge * 2 + 1] = vertexIndex;
}

TEMP_DEF
inline unsigned StripResult<TEMP_ARG>::GetPoint(unsigned iEdge, unsigned iEdgeEnd)
{
	return m_Vertices[iEdge * 2 + iEdgeEnd];
}

TEMP_DEF
inline unsigned StripResult<TEMP_ARG>::GetEdgeBot(unsigned iEdge)
{
	return m_Vertices[iEdge * 2];
}

TEMP_DEF
inline unsigned StripResult<TEMP_ARG>::GetEdgeTop(unsigned iEdge)
{
	return m_Vertices[iEdge * 2 + 1];
}

TEMP_DEF
inline unsigned StripResult<TEMP_ARG>::GetFirstEdgeBot() const
{
	return m_Vertices[0];
}

TEMP_DEF
inline unsigned StripResult<TEMP_ARG>::GetFirstEdgeTop() const
{
	return m_Vertices[1];
}

TEMP_DEF
inline unsigned StripResult<TEMP_ARG>::GetLastEdgeBot() const
{
	return m_Vertices[m_Vertices.GetSize() - 2];
}

TEMP_DEF
inline unsigned StripResult<TEMP_ARG>::GetLastEdgeTop() const
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