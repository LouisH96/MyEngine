#pragma once

#include "..\MakerData.h"
#include "..\MakerResult.h"
#include "Strip.h"
#include "StripResult.h"

namespace MyEngine
{
namespace MeshMaker2
{

template<
	typename TVertex,
	ModelTopology TTopology,
	StripEdgeStyle TEdgeStyle,
	StripEndStyle TEndStyle,
	typename TResult = EmptyStripResult<TVertex, TTopology>
>
class StripMaker
{
public:
	using TStrip = Strip<TVertex, TEndStyle, TEdgeStyle>;
	using ShapeVertex = typename MakerData<TVertex, TTopology>::ShapeVertex;

	StripMaker(
		MakerData<TVertex, TTopology>& data,
		TStrip& strip);

	TResult Make();

private:
	TResult m_Result;
	MakerData<TVertex, TTopology>& m_Data;
	TStrip& m_Strip;

	Array<ShapeVertex> CreateSharedEdgeShapeVertices();
	Array<ShapeVertex> CreateSplitEdgeShapeVertices();

	void Make_Lines();
	void Make_Triangles_Sharp();
	void Make_Triangles_Smooth();

	//AddHelper
	void AddWallLeftBot(unsigned iWall, const Array<ShapeVertex>& vertices);
	void AddWallLeftTop(unsigned iWall, const Array<ShapeVertex>& vertices);
	void AddWallRightBot(unsigned iWall, const Array<ShapeVertex>& vertices);
	void AddWallRightTop(unsigned iWall, const Array<ShapeVertex>& vertices);
};

template<typename TVertex, ModelTopology TTopology,
	StripEdgeStyle TEdgeStyle, StripEndStyle TEndStyle,
	typename TResult>
inline StripMaker<TVertex, TTopology, TEdgeStyle, TEndStyle, TResult>::StripMaker(MakerData<TVertex, TTopology>& data, TStrip& strip)
	: m_Data{ data }
	, m_Strip{ strip }
{
}

template<typename TVertex, ModelTopology TTopology,
	StripEdgeStyle TEdgeStyle, StripEndStyle TEndStyle,
	typename TResult>
inline TResult StripMaker<TVertex, TTopology, TEdgeStyle, TEndStyle, TResult>::Make()
{
	m_Result.Begin(m_Data);

	if constexpr (TopologyInfo::IsLineType(TTopology))
	{
		Make_Lines();
	}
	else
	{
		if constexpr (TEdgeStyle == StripEdgeStyle::Split)
			Make_Triangles_Sharp();
		else
			Make_Triangles_Smooth();
	}

	m_Result.End(m_Data);
	return m_Result;
}

template<typename TVertex, ModelTopology TTopology,
	StripEdgeStyle TEdgeStyle, StripEndStyle TEndStyle,
	typename TResult>
inline Array<typename StripMaker<TVertex, TTopology, TEdgeStyle, TEndStyle, TResult>::ShapeVertex>
StripMaker<TVertex, TTopology, TEdgeStyle, TEndStyle, TResult>::CreateSharedEdgeShapeVertices()
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

template<typename TVertex, ModelTopology TTopology,
	StripEdgeStyle TEdgeStyle, StripEndStyle TEndStyle,
	typename TResult>
inline Array<typename StripMaker<TVertex, TTopology, TEdgeStyle, TEndStyle, TResult>::ShapeVertex>
StripMaker<TVertex, TTopology, TEdgeStyle, TEndStyle, TResult>::CreateSplitEdgeShapeVertices()
{
	if constexpr (TEdgeStyle != StripEdgeStyle::Split)
		Logger::Warning("[StripMaker::CreateSplitEdgeShapeVertices] should only be called with Split edges");

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

template<typename TVertex, ModelTopology TTopology,
	StripEdgeStyle TEdgeStyle, StripEndStyle TEndStyle,
	typename TResult>
inline void StripMaker<TVertex, TTopology, TEdgeStyle, TEndStyle, TResult>::Make_Lines()
{
	const Array<ShapeVertex> vertices{ CreateSharedEdgeShapeVertices() };

	if constexpr (TopologyInfo::IsListFormat(TTopology))
	{
		/* Used for all LineList's */
		for (unsigned iWall{ 0 }; iWall < m_Strip.GetNrWalls(); ++iWall)
		{
			//Vertical
			AddWallLeftBot(iWall, vertices);
			AddWallLeftTop(iWall, vertices);

			//Bot
			AddWallLeftBot(iWall, vertices);
			AddWallRightBot(iWall, vertices);

			//Top
			AddWallLeftTop(iWall, vertices);
			AddWallRightTop(iWall, vertices);
		}
		if constexpr (TEndStyle == StripEndStyle::Open)
		{
			AddWallRightBot(m_Strip.GetNrWalls() - 1, vertices);
			AddWallRightTop(m_Strip.GetNrWalls() - 1, vertices);
		}
	}
	else
	{
		/* Used for all LineStrip's */
		//Zig-Zag (note: vertices contains duplicate of first edge at the end, if Closed)
		AddWallLeftBot(0, vertices);

		for (unsigned iWall{ 0 }; iWall < m_Strip.GetNrWalls(); ++iWall)
		{
			if (iWall % 2 == 0)
			{
				AddWallLeftTop(iWall, vertices);
				AddWallRightTop(iWall, vertices);
			}
			else
			{
				AddWallLeftBot(iWall, vertices);
				AddWallRightBot(iWall, vertices);
			}
		}
		for (unsigned iWall{ m_Strip.GetNrWalls() - 1 }; iWall != Uint::Cast(-1); --iWall)
		{
			if (iWall % 2 == 0)
			{
				AddWallRightBot(iWall, vertices);
				AddWallLeftBot(iWall, vertices);
			}
			else
			{
				AddWallRightTop(iWall, vertices);
				AddWallRightBot(iWall, vertices);
			}
		}
	}
}

template<typename TVertex, ModelTopology TTopology,
	StripEdgeStyle TEdgeStyle, StripEndStyle TEndStyle,
	typename TResult>
inline void StripMaker<TVertex, TTopology, TEdgeStyle, TEndStyle, TResult>::Make_Triangles_Sharp()
{
	const Array<ShapeVertex> vertices{ CreateSplitEdgeShapeVertices() };

	if constexpr (TopologyInfo::IsListFormat(TTopology))
	{
		for (unsigned iWall{ 0 }; iWall < m_Strip.GetNrWalls(); ++iWall)
		{
			AddWallLeftBot(iWall, vertices);
			AddWallLeftTop(iWall, vertices);
			AddWallRightBot(iWall, vertices);

			AddWallLeftTop(iWall, vertices);
			AddWallRightTop(iWall, vertices);
			AddWallRightBot(iWall, vertices);
		}
	}
	else //Strip
	{
		for (unsigned iWall{ 0 }; iWall < m_Strip.GetNrWalls(); ++iWall)
		{
			AddWallLeftBot(iWall, vertices);
			AddWallLeftTop(iWall, vertices);
			AddWallRightBot(iWall, vertices);
			AddWallRightTop(iWall, vertices);
		}
	}
}

template<typename TVertex, ModelTopology TTopology,
	StripEdgeStyle TEdgeStyle, StripEndStyle TEndStyle,
	typename TResult>
inline void StripMaker<TVertex, TTopology, TEdgeStyle, TEndStyle, TResult>::Make_Triangles_Smooth()
{
	const Array<ShapeVertex> vertices{ CreateSharedEdgeShapeVertices() }; // will copy first edge in last indices when Closed

	if constexpr (TopologyInfo::IsListFormat(TTopology))
	{
		for (unsigned iWall{ 0 }; iWall < m_Strip.GetNrWalls(); ++iWall)
		{
			AddWallLeftBot(iWall, vertices);
			AddWallLeftTop(iWall, vertices);
			AddWallRightBot(iWall, vertices);

			AddWallLeftTop(iWall, vertices);
			AddWallRightTop(iWall, vertices);
			AddWallRightBot(iWall, vertices);
		}
	}
	else //Strip
	{
		for (unsigned iWall{ 0 }; iWall < m_Strip.GetNrWalls(); ++iWall)
		{
			AddWallLeftBot(iWall, vertices);
			AddWallLeftTop(iWall, vertices);
		}

		if constexpr (TEndStyle == StripEndStyle::Closed)
		{
			AddWallLeftBot(0, vertices);
			AddWallLeftTop(0, vertices);
		}
		else
		{
			AddWallRightBot(m_Strip.GetNrWalls() - 1, vertices);
			AddWallRightTop(m_Strip.GetNrWalls() - 1, vertices);
		}
	}
}

template<typename TVertex, ModelTopology TTopology, StripEdgeStyle TEdgeStyle, StripEndStyle TEndStyle, typename TResult>
inline void StripMaker<TVertex, TTopology, TEdgeStyle, TEndStyle, TResult>::AddWallLeftBot(unsigned iWall, const Array<ShapeVertex>& vertices)
{
	m_Result.AddEdgeBot(iWall, m_Data.Add(vertices[iWall * 2]));
}

template<typename TVertex, ModelTopology TTopology, StripEdgeStyle TEdgeStyle, StripEndStyle TEndStyle, typename TResult>
inline void StripMaker<TVertex, TTopology, TEdgeStyle, TEndStyle, TResult>::AddWallLeftTop(unsigned iWall, const Array<ShapeVertex>& vertices)
{
	m_Result.AddEdgeTop(iWall, m_Data.Add(vertices[iWall * 2 + 1]));
}

template<typename TVertex, ModelTopology TTopology, StripEdgeStyle TEdgeStyle, StripEndStyle TEndStyle, typename TResult>
inline void StripMaker<TVertex, TTopology, TEdgeStyle, TEndStyle, TResult>::AddWallRightBot(unsigned iWall, const Array<ShapeVertex>& vertices)
{
	m_Result.AddEdgeBot(iWall + 1, m_Data.Add(vertices[iWall * 2 + 2]));
}

template<typename TVertex, ModelTopology TTopology, StripEdgeStyle TEdgeStyle, StripEndStyle TEndStyle, typename TResult>
inline void StripMaker<TVertex, TTopology, TEdgeStyle, TEndStyle, TResult>::AddWallRightTop(unsigned iWall, const Array<ShapeVertex>& vertices)
{
	m_Result.AddEdgeTop(iWall + 1, m_Data.Add(vertices[iWall * 2 + 3]));
}

}
}