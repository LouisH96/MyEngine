#pragma once

#include "..\MakerData.h"
#include "..\MakerResult.h"
#include "HoleArray.h"
#include "..\Arc\Arc.h"
#include "..\Arc\ArcResult.h"
#include "..\Strip\Strip.h"
#include "..\Strip\StripMaker.h"
#include <Geometry\Shapes\Rects.h>

namespace MyEngine
{
namespace MeshMaker2
{

template<typename TVertex, ModelTopology TTopology>
class HoleArrayMaker
{
public:
	using ShapeVertex = typename MakerData<TVertex, TTopology>;

	HoleArrayMaker(MakerData<TVertex, TTopology>& data, HoleArray<TVertex>& holeArray);

	MakerResult<TVertex, TTopology> Make();

private:
	using Arc = SharedArc<TVertex>;
	using ArcResult = ArcResult<TVertex, TTopology>;
	using ArcMaker = ArcMaker<TVertex, TTopology, ArcResult, ArcEdgeStyle::Shared>;
	using Strip = SharedStrip<TVertex, StripEndStyle::Open>;
	using StripMaker = StripMaker<TVertex, TTopology, StripEdgeStyle::Shared, StripEndStyle::Open>;
	using Vertex = MakerVertex<TVertex>;

	MakerResult<TVertex, TTopology> m_Result;
	MakerData<TVertex, TTopology>& m_Data;
	HoleArray<TVertex>& m_HoleArray;

	static inline const Float3 NORMAL{ 0,1,0 };
	unsigned m_NrSidesLeft{};
	unsigned m_NrSidesRight{};
	unsigned m_NrLeftCapCorners{};
	unsigned m_NrRightCapCorners{};
	Array<Float3> m_PointsLeft;
	Array<Float3> m_PointsRight;
	RectFloat m_Bounds;
	Vertex m_SharedVertices[2]; //0 is top, 1 is bottom
	bool DoStripsShareVertex() const { return m_HoleArray.GetNrCorners() % 2 == 1; }

	static void GetNrSides(unsigned nrCircleCorners, unsigned& left, unsigned& right);
	void FindBounds();
	void MovePoints();

	void MakePointsLeft(float cornerInterval);
	void MakePointsRight(float cornerInterval);
	static Array<Float3> MakePoints(unsigned nrSides, float radius, float cornerInterval);

	void MakeLeftCap();
	ArcResult MakeLeftCapTopArc();
	ArcResult MakeLeftCapBotArc(int lastTopArcVertexIdx);

	void MakeRightStrip(unsigned iHole);
	void MakeLeftStrip(unsigned iHole);
};

template<typename TVertex, ModelTopology TTopology>
inline HoleArrayMaker<TVertex, TTopology>::HoleArrayMaker(MakerData<TVertex, TTopology>& data, HoleArray<TVertex>& holeArray)
	: m_Data{ data }
	, m_HoleArray{ holeArray }
{
}

template<typename TVertex, ModelTopology TTopology>
inline MakerResult<TVertex, TTopology> HoleArrayMaker<TVertex, TTopology>::Make()
{
	using GapStrip = SharedStrip<TVertex, StripEndStyle::Open>;

	m_Result.Begin(m_Data);

	//Get Nr Sides
	GetNrSides(m_HoleArray.GetNrCorners(), m_NrSidesLeft, m_NrSidesRight);

	//Get hole points
	const float angleInterval{ Constants::PI2 / m_HoleArray.GetNrCorners() };
	MakePointsLeft(angleInterval);
	MakePointsRight(angleInterval);
	FindBounds();
	MovePoints();

	//Left cap
	MakeLeftCap();

	//
	MakeRightStrip(0);
	//MakeLeftStrip(1);
	//MakeRightStrip(2);

	////Make Hole Points (relative to (0,0))
	//GapStrip gapStrips[]{


	//};

	m_Result.End(m_Data);
	return m_Result;
}

template<typename TVertex, ModelTopology TTopology>
inline void HoleArrayMaker<TVertex, TTopology>::MakeLeftCap()
{
	m_NrLeftCapCorners = Uint::Ceil(m_PointsLeft.GetSize() / 2.f);

	//Top Arc
	m_Data.StartShape<TTopology>();
	const ArcResult topArcResult{ MakeLeftCapTopArc() };

	//Bot Arc
	m_Data.StartShape<TTopology>();
	const int lastTopArcCornerIdx{ topArcResult.GetCorner(m_NrLeftCapCorners - 1) };
	const ArcResult botArcResult{ MakeLeftCapBotArc(lastTopArcCornerIdx) };
}

template<typename TVertex, ModelTopology TTopology>
inline typename HoleArrayMaker<TVertex, TTopology>::ArcResult HoleArrayMaker<TVertex, TTopology>::MakeLeftCapTopArc()
{
	const Float3 offset{ m_HoleArray.GetRadius(), 0, 0 };
	const Float3 leftTopPos{
		Float3{m_Bounds.GetLeft() , 0, m_Bounds.GetTop()} + offset
	};
	Vertex leftTopVertex{ MakeVertex<TVertex>::Full(leftTopPos, NORMAL) };

	Arc arc{};
	for (unsigned iCorner{ 1 }; iCorner < m_NrLeftCapCorners; ++iCorner)
	{
		arc.AddWing(leftTopVertex,
			MakeVertex<TVertex>::Full(m_PointsLeft[iCorner - 1] + offset, NORMAL),
			MakeVertex<TVertex>::Full(m_PointsLeft[iCorner] + offset, NORMAL));
	}

	ArcMaker maker{ m_Data, arc };
	const ArcResult result{ maker.Make() };

	if (DoStripsShareVertex())
		m_SharedVertices[0] = MakeVertex<TVertex>::Ref(result.GetCorner(0));

	return result;
}

template<typename TVertex, ModelTopology TTopology>
inline typename HoleArrayMaker<TVertex, TTopology>::ArcResult HoleArrayMaker<TVertex, TTopology>::MakeLeftCapBotArc(int lastTopArcVertexIdx)
{
	const Float3 offset{ m_HoleArray.GetRadius(), 0, 0 };
	const Float3 leftBotPos{
		Float3{m_Bounds.GetLeft(), 0, m_Bounds.GetBottom()} + offset
	};
	Vertex leftBotVertex{ MakeVertex<TVertex>::Full(leftBotPos, NORMAL) };

	Arc arc{};
	unsigned iPoint{ m_PointsLeft.GetSize() - m_NrLeftCapCorners };

	if (m_PointsLeft.GetSize() % 2 == 1)
	{
		iPoint++;
		MakerVertex<TVertex> sharedVertex{ MakeVertex<TVertex>::Ref(lastTopArcVertexIdx) };

		arc.AddWing(leftBotVertex, sharedVertex,
			MakeVertex<TVertex>::Full(m_PointsLeft[iPoint] + offset, NORMAL));
	}

	for (; iPoint + 1 < m_PointsLeft.GetSize(); ++iPoint)
	{
		arc.AddWing(leftBotVertex,
			MakeVertex<TVertex>::Full(m_PointsLeft[iPoint] + offset, NORMAL),
			MakeVertex<TVertex>::Full(m_PointsLeft[iPoint + 1] + offset, NORMAL));
	}
	ArcMaker maker{ m_Data, arc };
	const ArcResult result{ maker.Make() };

	if (DoStripsShareVertex())
		m_SharedVertices[1] = MakeVertex<TVertex>::Ref(result.GetCorner(m_NrLeftCapCorners - 1));

	return result;
}

template<typename TVertex, ModelTopology TTopology>
inline void HoleArrayMaker<TVertex, TTopology>::MakeRightStrip(unsigned iHole)
{
	m_Data.StartShape<TTopology>();

	const Float3 offset{
		m_HoleArray.GetRadius() +
		(m_HoleArray.GetRadius() * 2 + m_HoleArray.GetGap()) * iHole, 0, 0 };

	Strip strip{};
	for (unsigned iCorner{ 0 }; iCorner < m_PointsRight.GetSize(); ++iCorner)
	{
		Float3 pos{ m_PointsRight[iCorner] + offset };
		Vertex left{ MakeVertex<TVertex>::Full(pos, NORMAL) };

		pos.x += (m_HoleArray.GetRadius() - m_PointsRight[iCorner].x) * 2 + m_HoleArray.GetGap();
		Vertex right{ MakeVertex<TVertex>::Full(pos, NORMAL) };

		strip.AddEdge({ left, right });
	}

	if (DoStripsShareVertex())
	{
		strip.SetEdgeBot(0, m_SharedVertices[0]);
		strip.SetEdgeBot(m_PointsRight.GetSize() - 1, m_SharedVertices[1]);
	}

	StripMaker maker{ m_Data, strip };
	maker.Make();
}

//Uses the corners/points of the left side of the hole. But this strip still needs to be placed
// on the right side of iHole
template<typename TVertex, ModelTopology TTopology>
inline void HoleArrayMaker<TVertex, TTopology>::MakeLeftStrip(unsigned iHole)
{
	m_Data.StartShape<TTopology>();
	const Float3 offset{
		m_HoleArray.GetRadius() +
		(m_HoleArray.GetRadius() * 2 + m_HoleArray.GetGap()) * (iHole + 1), 0, 0 };

	Strip strip{};
	for (unsigned iCorner{ 0 }; iCorner < m_PointsLeft.GetSize(); ++iCorner)
	{
		Float3 pos{ m_PointsLeft[iCorner] + offset };
		Vertex right{ MakeVertex<TVertex>::Full(pos, NORMAL) };

		pos.x -= m_PointsLeft[iCorner].x * 2;
		pos.x -= m_HoleArray.GetRadius() * 2 + m_HoleArray.GetGap();
		Vertex left{ MakeVertex<TVertex>::Full(pos, NORMAL) };

		strip.AddEdge({ left, right });
	}

	StripMaker maker{ m_Data, strip };
	maker.Make();
}

template<typename TVertex, ModelTopology TTopology>
inline void HoleArrayMaker<TVertex, TTopology>::GetNrSides(unsigned nrHoleCorners, unsigned& left, unsigned& right)
{
	const unsigned half{ Uint::Floor(nrHoleCorners * .5f) };
	left = half - 1 + (2 - half % 2) * (nrHoleCorners % 2);
	right = half - 1 + (1 + half % 2) * (nrHoleCorners % 2);
}

template<typename TVertex, ModelTopology TTopology>
inline void HoleArrayMaker<TVertex, TTopology>::MakePointsLeft(float cornerInterval)
{
	m_PointsLeft = MakePoints(m_NrSidesLeft, m_HoleArray.GetRadius(), cornerInterval);
	for (unsigned i{ 0 }; i < m_PointsLeft.GetSize(); ++i)
		m_PointsLeft[i].x = -m_PointsLeft[i].x;
}

template<typename TVertex, ModelTopology TTopology>
inline void HoleArrayMaker<TVertex, TTopology>::MakePointsRight(float cornerInterval)
{
	m_PointsRight = MakePoints(m_NrSidesRight, m_HoleArray.GetRadius(), cornerInterval);
}

template<typename TVertex, ModelTopology TTopology>
inline Array<Float3> HoleArrayMaker<TVertex, TTopology>::MakePoints(unsigned nrSides, float radius, float cornerInterval)
{
	Array<Float3> points{ nrSides + 1 };

	float angle{ nrSides * .5f * cornerInterval };
	for (unsigned i{ 0 }; i <= nrSides; ++i)
	{
		points[i] = Float3{ cosf(angle), 0, sinf(angle) } *radius;
		angle -= cornerInterval;
	}
	return points;
}

template<typename TVertex, ModelTopology TTopology>
inline void HoleArrayMaker<TVertex, TTopology>::FindBounds()
{
	//Most left
	const unsigned mostLeftIdx{ m_PointsLeft.GetSize() / 2 };
	m_Bounds.SetLeft(m_PointsLeft[mostLeftIdx].x);

	//Lowest
	m_Bounds.SetBottom(m_PointsRight.Last().z);

	//Most right
	const unsigned mostRightIdx{ m_PointsRight.GetSize() / 2 };
	m_Bounds.SetRightUseWidth(m_PointsRight[mostRightIdx].x);

	//Highest
	m_Bounds.SetTopUseHeight(m_PointsRight.First().z);
}

template<typename TVertex, ModelTopology TTopology>
inline void HoleArrayMaker<TVertex, TTopology>::MovePoints()
{
	const Float3 movement{
		0, 0, m_HoleArray.GetRadius()
	};
	for (unsigned i{ 0 }; i < m_PointsLeft.GetSize(); ++i)
		m_PointsLeft[i] += movement;
	for (unsigned i{ 0 }; i < m_PointsRight.GetSize(); ++i)
		m_PointsRight[i] += movement;
	m_Bounds.Move(Float2{ 0, m_HoleArray.GetRadius() });
}

}
}