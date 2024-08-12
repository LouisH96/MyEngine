#pragma once

#include "..\MakerData.h"
#include "..\MakerResult.h"
#include "Strip.h"

namespace MyEngine
{
namespace MeshMaker2
{

template<
	typename TVertex,
	ModelTopology TTopology,
	StripEdgeStyle TEdgeStyle,
	StripEndStyle TEndStyle
>
class StripMaker
{
public:
	using TStrip = Strip<TVertex, TEndStyle, TEdgeStyle>;
	using ShapeVertex = typename MakerData<TVertex, TTopology>::ShapeVertex;

	StripMaker(
		MakerData<TVertex, TTopology>& data,
		TStrip& strip);

	MakerResult<TVertex, TTopology> Make();

private:
	MakerResult<TVertex, TTopology> m_Result;
	MakerData<TVertex, TTopology>& m_Data;
	TStrip& m_Strip;

	Array<ShapeVertex> CreateSharedEdgeShapeVertices();
	Array<ShapeVertex> CreateSplitEdgeShapeVertices();

	void Make_Lines();
	void Make_Triangles_Sharp();
	void Make_Triangles_Smooth();
};

template<typename TVertex, ModelTopology TTopology, StripEdgeStyle TEdgeStyle, StripEndStyle TEndStyle>
inline StripMaker<TVertex, TTopology, TEdgeStyle, TEndStyle>::StripMaker(MakerData<TVertex, TTopology>& data, TStrip& strip)
	: m_Data{ data }
	, m_Strip{ strip }
{
}

template<typename TVertex, ModelTopology TTopology, StripEdgeStyle TEdgeStyle, StripEndStyle TEndStyle>
inline MakerResult<TVertex, TTopology> StripMaker<TVertex, TTopology, TEdgeStyle, TEndStyle>::Make()
{
	m_Result.Begin(m_Data);

	if constexpr (TopologyInfo::IsLineType(TTopology))
	{
		Make_Lines();
	}
	else
	{
		if constexpr (TEdgeStyle == StripEdgeStyle::Sharp)
			Make_Triangles_Sharp();
		else
			Make_Triangles_Smooth();
	}

	m_Result.End(m_Data);
	return m_Result;
}

template<typename TVertex, ModelTopology TTopology, StripEdgeStyle TEdgeStyle, StripEndStyle TEndStyle>
inline Array<typename StripMaker<TVertex, TTopology, TEdgeStyle, TEndStyle>::ShapeVertex>
StripMaker<TVertex, TTopology, TEdgeStyle, TEndStyle>::CreateSharedEdgeShapeVertices()
{
	unsigned nrVertices{ m_Strip.GetNrShapeEdges() * 2 };
	if constexpr (TEndStyle == StripEndStyle::Closed)
		nrVertices += 2;

	Array<ShapeVertex> vertices{ nrVertices };
	for (unsigned iEdge{ 0 }; iEdge < m_Strip.GetNrShapeEdges(); ++iEdge)
	{
		Edge<TVertex> edge{ m_Strip.GetShapeEdge(iEdge) };
		vertices[iEdge * 2 + 0] = edge[0].MakeShapeVertex(m_Data);
		vertices[iEdge * 2 + 1] = edge[1].MakeShapeVertex(m_Data);
	}

	if constexpr (TEndStyle == StripEndStyle::Closed)
	{
		//copy the first 2 vertices to the end, for easier algorithms in CombinePhase
		vertices[vertices.GetSize() - 2] = vertices[0];
		vertices[vertices.GetSize() - 1] = vertices[1];
	}
	return vertices;
}

template<typename TVertex, ModelTopology TTopology, StripEdgeStyle TEdgeStyle, StripEndStyle TEndStyle>
inline Array<typename StripMaker<TVertex, TTopology, TEdgeStyle, TEndStyle>::ShapeVertex>
StripMaker<TVertex, TTopology, TEdgeStyle, TEndStyle>::CreateSplitEdgeShapeVertices()
{
	if constexpr (TEdgeStyle != StripEdgeStyle::Sharp)
		Logger::PrintWarning("[StripMaker::CreateSplitEdgeShapeVertices] should only be called with Sharp edges");

	Array<ShapeVertex> vertices{ m_Strip.GetNrWalls() * 4 };
	for (unsigned iWall{ 0 }; iWall < m_Strip.GetNrWalls(); ++iWall)
	{
		Edge<TVertex> left{ m_Strip.GetWallLeftEdge(iWall) };
		Edge<TVertex> right{ m_Strip.GetWallRightEdge(iWall) };

		vertices[iWall * 4 + 0] = left[0].MakeShapeVertex(m_Data);
		vertices[iWall * 4 + 1] = left[1].MakeShapeVertex(m_Data);
		vertices[iWall * 4 + 2] = right[0].MakeShapeVertex(m_Data);
		vertices[iWall * 4 + 3] = right[1].MakeShapeVertex(m_Data);
	}
	return vertices;
}

template<typename TVertex, ModelTopology TTopology, StripEdgeStyle TEdgeStyle, StripEndStyle TEndStyle>
inline void StripMaker<TVertex, TTopology, TEdgeStyle, TEndStyle>::Make_Lines()
{
	const Array<ShapeVertex> vertices{ CreateSharedEdgeShapeVertices() };

	if constexpr (TopologyInfo::IsListFormat(TTopology))
	{
		/* Used for all LineList's */
		for (unsigned iVertex{ 0 }; iVertex + 2 < vertices.GetSize(); iVertex += 2)
		{
			//Vertical
			m_Data.Add(vertices[iVertex]);
			m_Data.Add(vertices[iVertex + 1]);

			//Bot
			m_Data.Add(vertices[iVertex]);
			m_Data.Add(vertices[iVertex + 2]);
			//Top
			m_Data.Add(vertices[iVertex + 1]);
			m_Data.Add(vertices[iVertex + 3]);
		}
		if constexpr (TEndStyle == StripEndStyle::Open)
		{
			m_Data.Add(vertices[vertices.GetSize() - 2]);
			m_Data.Add(vertices[vertices.GetSize() - 1]);
		}
	}
	else
	{
		/* Used for all LineStrip's */
		//Zig-Zag (note: vertices contains duplicate of first edge at the end, if Closed)
		m_Data.Add(vertices[0]);

		int stepChange{ -2 }; //will be 2 or -2
		int step{ 3 }; //will be 1 or 3

		int left{ -2 }; //first step will change this into 1
		int right{ 0 }; //first step will change this into 3

		for (unsigned iWall{ 0 }; iWall < m_Strip.GetNrWalls(); ++iWall)
		{
			left += step;
			right += step;
			step += stepChange;
			stepChange *= -1;

			m_Data.Add(vertices[left]);
			m_Data.Add(vertices[right]);
		}
		left -= stepChange / 2;
		right -= stepChange / 2;
		for (unsigned iWall{ 0 }; iWall < m_Strip.GetNrWalls(); ++iWall)
		{
			m_Data.Add(vertices[right]);
			m_Data.Add(vertices[left]);

			left -= step;
			right -= step;
			step += stepChange;
			stepChange *= -1;
		}
	}
}

template<typename TVertex, ModelTopology TTopology, StripEdgeStyle TEdgeStyle, StripEndStyle TEndStyle>
inline void StripMaker<TVertex, TTopology, TEdgeStyle, TEndStyle>::Make_Triangles_Sharp()
{
	const Array<ShapeVertex> vertices{ CreateSplitEdgeShapeVertices() };

	if constexpr (TopologyInfo::IsListFormat(TTopology))
	{
		for (unsigned iWall{ 0 }; iWall < m_Strip.GetNrWalls(); ++iWall)
		{
			m_Data.Add(vertices[iWall * 4 + 0]);
			m_Data.Add(vertices[iWall * 4 + 1]);
			m_Data.Add(vertices[iWall * 4 + 2]);

			m_Data.Add(vertices[iWall * 4 + 1]);
			m_Data.Add(vertices[iWall * 4 + 3]);
			m_Data.Add(vertices[iWall * 4 + 2]);
		}
	}
	else //Strip
	{
		for (unsigned iWall{ 0 }; iWall < m_Strip.GetNrWalls(); ++iWall)
		{
			m_Data.Add(vertices[iWall * 4 + 0]);
			m_Data.Add(vertices[iWall * 4 + 1]);
			m_Data.Add(vertices[iWall * 4 + 2]);
			m_Data.Add(vertices[iWall * 4 + 3]);
		}
	}
}

template<typename TVertex, ModelTopology TTopology, StripEdgeStyle TEdgeStyle, StripEndStyle TEndStyle>
inline void StripMaker<TVertex, TTopology, TEdgeStyle, TEndStyle>::Make_Triangles_Smooth()
{
	const Array<ShapeVertex> vertices{ CreateSharedEdgeShapeVertices() }; // will copy first edge in last indices when Closed

	if constexpr (TopologyInfo::IsListFormat(TTopology))
	{
		for (unsigned iWall{ 0 }; iWall < m_Strip.GetNrWalls(); ++iWall)
		{
			m_Data.Add(vertices[iWall * 2 + 0]);
			m_Data.Add(vertices[iWall * 2 + 1]);
			m_Data.Add(vertices[iWall * 2 + 2]);

			m_Data.Add(vertices[iWall * 2 + 1]);
			m_Data.Add(vertices[iWall * 2 + 3]);
			m_Data.Add(vertices[iWall * 2 + 2]);
		}
	}
	else //Strip
	{
		for (unsigned iEdge{ 0 }; iEdge < m_Strip.GetNrShapeEdges(); ++iEdge)
		{
			m_Data.Add(vertices[iEdge * 2 + 0]);
			m_Data.Add(vertices[iEdge * 2 + 1]);
		}
		if constexpr (TEndStyle == StripEndStyle::Closed)
		{
			m_Data.Add(vertices[0]);
			m_Data.Add(vertices[1]);
		}
	}
}

}
}