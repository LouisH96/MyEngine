#pragma once

#include "..\MakerData.h"
#include "..\Quad\Quad.h"
#include "..\Shapes\Edge.h"

namespace MyEngine
{
namespace MeshMaker2
{

enum class StripEdgeStyle {
	Shared, Split
};

enum class StripEndStyle {
	Open, Closed
};

template<typename TVertex, StripEndStyle TEndStyle, StripEdgeStyle TEdgeStyle>
class Strip;

template<typename TVertex, StripEndStyle TEndStyle>
using SharedStrip = Strip<TVertex, TEndStyle, StripEdgeStyle::Shared>;

template<typename TVertex, StripEndStyle TEndStyle>
using SplitStrip = Strip<TVertex, TEndStyle, StripEdgeStyle::Split>;

template<typename TVertex, StripEndStyle TEndStyle, StripEdgeStyle TEdgeStyle>
class Strip
{
public:
	template<bool THasIndices>
	using Data = MakerDataBase<TVertex, THasIndices>;

	Strip() = default;

	void AddEdge(Edge<TVertex> edge);
	void SetEdgeBot(unsigned iEdge, MakerVertex<TVertex> vertex);
	void SetEdgeTop(unsigned iEdge, MakerVertex<TVertex> vertex);

	unsigned GetNrShapeEdges() const; //a shared edge counts as one
	unsigned GetNrWalls() const;

	Edge<TVertex> GetWallLeftEdge(unsigned iWall) const;
	Edge<TVertex> GetWallRightEdge(unsigned iWall) const;
	Edge<TVertex> GetShapeEdge(unsigned iEdge) const; //doesnt change if shared or split edges

	template<bool THasIndices>
	void CalculateNormals(Data<THasIndices>& data);

private:
	List<Edge<TVertex>> m_Edges;
};

template<typename TVertex, StripEndStyle TEndStyle, StripEdgeStyle TEdgeStyle>
inline void Strip<TVertex, TEndStyle, TEdgeStyle>::AddEdge(Edge<TVertex> edge)
{
	m_Edges.Add(edge);
}

template<typename TVertex, StripEndStyle TEndStyle, StripEdgeStyle TEdgeStyle>
inline void Strip<TVertex, TEndStyle, TEdgeStyle>::SetEdgeBot(unsigned iEdge, MakerVertex<TVertex> vertex)
{
	m_Edges[iEdge].Vertices[0] = vertex;
}

template<typename TVertex, StripEndStyle TEndStyle, StripEdgeStyle TEdgeStyle>
inline void Strip<TVertex, TEndStyle, TEdgeStyle>::SetEdgeTop(unsigned iEdge, MakerVertex<TVertex> vertex)
{
	m_Edges[iEdge].Vertices[1] = vertex;
}

template<typename TVertex, StripEndStyle TEndStyle, StripEdgeStyle TEdgeStyle>
inline unsigned Strip<TVertex, TEndStyle, TEdgeStyle>::GetNrShapeEdges() const
{
	if constexpr (TEdgeStyle == StripEdgeStyle::Split)
	{
		unsigned count{ m_Edges.GetSize() / 2 };
		if constexpr (TEndStyle == StripEndStyle::Open)
			++count;
		return count;
	}
	else
		return m_Edges.GetSize();
}

template<typename TVertex, StripEndStyle TEndStyle, StripEdgeStyle TEdgeStyle>
inline unsigned Strip<TVertex, TEndStyle, TEdgeStyle>::GetNrWalls() const
{
	unsigned count;

	if constexpr (TEdgeStyle == StripEdgeStyle::Split)
		count = m_Edges.GetSize() / 2;
	else
	{
		count = m_Edges.GetSize();
		if constexpr (TEndStyle == StripEndStyle::Open)
			--count;
	}

	return count;
}

template<typename TVertex, StripEndStyle TEndStyle, StripEdgeStyle TEdgeStyle>
inline Edge<TVertex> Strip<TVertex, TEndStyle, TEdgeStyle>::GetWallLeftEdge(unsigned iWall) const
{
	if constexpr (TEndStyle == StripEndStyle::Closed)
		iWall %= GetNrWalls();

	if constexpr (TEdgeStyle == StripEdgeStyle::Split)
		return m_Edges[iWall * 2];
	else 
		return m_Edges[iWall];
}

template<typename TVertex, StripEndStyle TEndStyle, StripEdgeStyle TEdgeStyle>
inline Edge<TVertex> Strip<TVertex, TEndStyle, TEdgeStyle>::GetWallRightEdge(unsigned iWall) const
{
	if constexpr (TEdgeStyle == StripEdgeStyle::Split)
	{
		if constexpr (TEndStyle == StripEndStyle::Closed)
			iWall %= GetNrWalls();
		return m_Edges[iWall * 2 + 1];
	}
	else
	{
		iWall++;
		if constexpr (TEndStyle == StripEndStyle::Closed)
			iWall %= GetNrWalls();
		return m_Edges[iWall];
	}
}

template<typename TVertex, StripEndStyle TEndStyle, StripEdgeStyle TEdgeStyle>
inline Edge<TVertex> Strip<TVertex, TEndStyle, TEdgeStyle>::GetShapeEdge(unsigned iEdge) const
{
	if constexpr (TEdgeStyle == StripEdgeStyle::Shared)
	{
		if constexpr (TEndStyle == StripEndStyle::Closed)
			iEdge %= m_Edges.GetSize();
		return m_Edges[iEdge];
	}
	else //Split
	{
		if (iEdge == 0)
			return m_Edges[0];

		iEdge = iEdge * 2 - 1;
		if constexpr (TEndStyle == StripEndStyle::Closed)
			iEdge %= m_Edges.GetSize();
		return m_Edges[iEdge];
	}
}

template<typename TVertex, StripEndStyle TEndStyle, StripEdgeStyle TEdgeStyle>
template<bool THasIndices>
inline void Strip<TVertex, TEndStyle, TEdgeStyle>::CalculateNormals(Data<THasIndices>& data)
{
	if constexpr (TEdgeStyle == StripEdgeStyle::Split)
	{
		for (unsigned iWall{ 0 }; iWall < GetNrWalls(); ++iWall)
		{
			Quad<TVertex> quad{};
			quad.SetLeft(GetWallLeftEdge(iWall));
			quad.SetRight(GetWallRightEdge(iWall));
			quad.CalculateNormal(data);
		}
	}
	else
	{
		//todo:
		//requires Slerp
	}
}

}
}