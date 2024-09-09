#pragma once

#include "..\MakerResult.h"
#include "Arc.h"

namespace MyEngine
{
namespace MeshMaker2
{
template<typename TVertex, ModelTopology TTopology,
	typename TResult,
	ArcEdgeStyle TEdgeStyle>
class ArcMaker
{
public:
	using TArc = Arc<TVertex, TEdgeStyle>;
	using ShapeVertex = typename MakerData<TVertex, TTopology>::ShapeVertex;

	ArcMaker(MakerData<TVertex, TTopology>& data, TArc& arc);

	TResult Make();

private:
	TResult m_Result;
	MakerData<TVertex, TTopology>& m_Data;
	TArc& m_Arc;

	Array<ShapeVertex> CreateSharedShapeVertices();
	Array<ShapeVertex> CreateSplitShapeVertices();

	void Make_Lines();
	void Make_Triangles_Shared();
	void Make_Triangles_Split();
};

template<typename TVertex, ModelTopology TTopology, typename TResult, ArcEdgeStyle TEdgeStyle>
inline ArcMaker<TVertex, TTopology, TResult, TEdgeStyle>::ArcMaker(MakerData<TVertex, TTopology>& data, TArc& arc)
	: m_Data{ data }
	, m_Arc{ arc }
{
}

template<typename TVertex, ModelTopology TTopology, typename TResult, ArcEdgeStyle TEdgeStyle>
inline TResult ArcMaker<TVertex, TTopology, TResult, TEdgeStyle>::Make()
{
	m_Result.Begin(m_Data);

	if constexpr (TopologyInfo::IsLineType(TTopology))
		Make_Lines();
	else
	{
		if constexpr (TEdgeStyle == ArcEdgeStyle::Split)
			Make_Triangles_Split();
		else
			Make_Triangles_Shared();
	}

	m_Result.End(m_Data);
	return m_Result;
}

template<typename TVertex, ModelTopology TTopology, typename TResult, ArcEdgeStyle TEdgeStyle>
inline Array<typename ArcMaker<TVertex, TTopology, TResult, TEdgeStyle>::ShapeVertex>
ArcMaker<TVertex, TTopology, TResult, TEdgeStyle>::CreateSharedShapeVertices()
{
	Array<ShapeVertex> vertices{ m_Arc.GetNrWings() + 2 };
	vertices[0] = m_Arc.GetWingCenter(0).MakeShapeVertex(m_Data);
	vertices[1] = m_Arc.GetWingLeft(0).MakeShapeVertex(m_Data);
	for (unsigned iWing{ 0 }; iWing < m_Arc.GetNrWings(); ++iWing)
		vertices[iWing + 2] = m_Arc.GetWingRight(iWing).MakeShapeVertex(m_Data);
	return vertices;
}

template<typename TVertex, ModelTopology TTopology, typename TResult, ArcEdgeStyle TEdgeStyle>
inline Array<typename ArcMaker<TVertex, TTopology, TResult, TEdgeStyle>::ShapeVertex>
ArcMaker<TVertex, TTopology, TResult, TEdgeStyle>::CreateSplitShapeVertices()
{
	Array<ShapeVertex> vertices{ m_Arc.GetNrWings() * 3 };
	for (unsigned iWing{ 0 }; iWing < m_Arc.GetNrWings(); ++iWing)
	{
		vertices[iWing * 3 + 0] = m_Arc.GetWingCenter(iWing).MakeShapeVertex(m_Data);
		vertices[iWing * 3 + 1] = m_Arc.GetWingLeft(iWing).MakeShapeVertex(m_Data);
		vertices[iWing * 3 + 2] = m_Arc.GetWingRight(iWing).MakeShapeVertex(m_Data);
	}
	return vertices;
}

template<typename TVertex, ModelTopology TTopology, typename TResult, ArcEdgeStyle TEdgeStyle>
inline void ArcMaker<TVertex, TTopology, TResult, TEdgeStyle>::Make_Lines()
{
	const Array<ShapeVertex> vertices{ CreateSharedShapeVertices() };

	if constexpr (TopologyInfo::IsListFormat(TTopology))
	{
		//first left side
		m_Result.AddCenter(m_Data.Add(vertices[0]));
		m_Result.AddCorner(0, m_Data.Add(vertices[1]));
		for (unsigned iWing{ 0 }; iWing < m_Arc.GetNrWings(); ++iWing)
		{
			//Right side of wing
			m_Result.AddCenter(m_Data.Add(vertices[0]));
			m_Result.AddCorner(iWing + 1, m_Data.Add(vertices[iWing + 2]));

			//Top line of wing
			m_Result.AddCorner(iWing, m_Data.Add(vertices[iWing + 1]));
			m_Result.AddCorner(iWing + 1, m_Data.Add(vertices[iWing + 2]));
		}
	}
	else
	{
		//zig-zag
		for (unsigned iWing{ 0 }; iWing < m_Arc.GetNrWings(); iWing += 2)
		{
			m_Result.AddCenter(m_Data.Add(vertices[0]));
			m_Result.AddCorner(iWing, m_Data.Add(vertices[iWing + 1]));
			m_Result.AddCorner(iWing + 1, m_Data.Add(vertices[iWing + 2]));
		}
		m_Result.AddCenter(m_Data.Add(vertices[0]));

		for (unsigned iCorner{ vertices.GetSize() - 1 - (m_Arc.GetNrWings() % 2) }; iCorner > 1; --iCorner)
			m_Result.AddCorner(iCorner - 1, m_Data.Add(vertices[iCorner]));
	}
}

template<typename TVertex, ModelTopology TTopology, typename TResult, ArcEdgeStyle TEdgeStyle>
inline void ArcMaker<TVertex, TTopology, TResult, TEdgeStyle>::Make_Triangles_Shared()
{
	const Array<ShapeVertex> vertices{ CreateSharedShapeVertices() };
	if constexpr (TopologyInfo::IsListFormat(TTopology))
	{
		for (unsigned iWing{ 0 }; iWing < m_Arc.GetNrWings(); ++iWing)
		{
			m_Result.AddCenter(m_Data.Add(vertices[0]));
			m_Result.AddCorner(iWing, m_Data.Add(vertices[iWing + 1]));
			m_Result.AddCorner(iWing + 1, m_Data.Add(vertices[iWing + 2]));
		}
	}
	else //Strip
	{
		m_Result.AddCorner(0, m_Data.Add(vertices[1]));
		m_Result.AddCorner(1, m_Data.Add(vertices[2]));
		m_Result.AddCenter(m_Data.Add(vertices[0]));

		unsigned iWing{ 1 };
		for (; iWing + 2 < m_Arc.GetNrWings(); iWing += 2)
		{
			m_Result.AddCorner(iWing + 1, m_Data.Add(vertices[iWing + 2]));
			m_Result.AddCorner(iWing + 2, m_Data.Add(vertices[iWing + 3]));
			m_Result.AddCorner(iWing + 2, m_Data.Add(vertices[iWing + 3]));
			m_Result.AddCenter(m_Data.Add(vertices[0]));
		}
		for (; iWing < m_Arc.GetNrWings(); ++iWing)
			m_Result.AddCorner(iWing + 1, m_Data.Add(vertices[iWing + 2]));
	}
}

template<typename TVertex, ModelTopology TTopology, typename TResult, ArcEdgeStyle TEdgeStyle>
inline void ArcMaker<TVertex, TTopology, TResult, TEdgeStyle>::Make_Triangles_Split()
{
	const Array<ShapeVertex> vertices{ CreateSplitShapeVertices() };
	if constexpr (TopologyInfo::IsListFormat(TTopology))
	{
		m_Result.AddCenter(m_Data.Add(vertices[0]));
		for (unsigned iVertex{ 1 }; iVertex < vertices.GetSize(); ++iVertex)
			m_Result.AddCorner(iVertex - 1, m_Data.Add(vertices[iVertex]));
	}
	else //Strip
	{
		for (unsigned iWing{ 0 }; iWing < m_Arc.GetNrWings(); ++iWing)
		{
			const unsigned center{ iWing * 3 };
			m_Result.AddCorner(iWing, m_Data.Add(vertices[center + 1]));

			if (iWing % 2 == 0)
			{
				m_Result.AddCorner(iWing + 1, m_Data.Add(vertices[center + 2]));
				m_Result.AddCenter(m_Data.Add(vertices[center]));
			}
			else
			{
				m_Result.AddCenter(m_Data.Add(vertices[center]));
				m_Result.AddCorner(iWing + 1, m_Data.Add(vertices[center + 2]));
			}
		}
	}
}

}
}