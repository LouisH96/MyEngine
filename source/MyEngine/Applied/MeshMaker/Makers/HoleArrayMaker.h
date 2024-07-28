#pragma once

#include <DataStructures\Pointers\SharedPtr.h>
#include <Geometry\Shapes\Rects.h>

#include "..\Maker.h"
#include "..\Shapes\Arc.h"
#include "..\Shapes\HoleArray.h"
#include "..\Shapes\Line.h"
#include "..\Shapes\Strip.h"
#include "ArcMaker.h"
#include "StripMaker.h"

namespace MyEngine
{
namespace MeshMaker
{
template<typename TVertex, ModelTopology TTopology>
class HoleArrayMaker
	: private Maker<TVertex, TTopology>
{
public:
	using BaseClass = Maker<TVertex, TTopology>;
	using PointType = typename BaseClass::DataType;
	using Maker<TVertex, TTopology>::Maker;

	MakerResult Make(const HoleArray& holeArray, RectFloat& bounds);

private:
	using BaseClass::Transform;
	using BaseClass::GetPosition;
	using BaseClass::m_MeshData;

	static void GetNrSides(unsigned nrCircleCorners, unsigned& left, unsigned& right);
	static unsigned GetNrCornersPerArc(const Strip& gapStrip);

	Strip MakeGapStrip(const HoleArray& holeArray, unsigned nrSides);

	void MakeStartCap(const HoleArray& holeArray, const Strip& firstGap, RectFloat& bounds);
	void MakeEndCap(const HoleArray& holeArray, const Strip& lastGap, RectFloat& bounds);
};
template<typename TVertex, ModelTopology TTopology>
inline MakerResult HoleArrayMaker<TVertex, TTopology>::Make(
	const HoleArray& holeArray, RectFloat& bounds)
{
	const unsigned nrGaps{ holeArray.GetNrHoles() - 1 };
	unsigned nrSidesLeft, nrSidesRight;
	GetNrSides(holeArray.GetNrCornersPerHole(), nrSidesLeft, nrSidesRight);

	//---| Gaps |---

	//Make vertices for 1 gap (and then copy & move them for others)
	//There is a strip for each side of the hole (left & right)
	Strip strips[]{
		MakeGapStrip(holeArray, nrSidesRight),
		MakeGapStrip(holeArray, nrSidesLeft)
	};

	//display -for now- with SharpStripMaker
	StripMaker<TVertex, TTopology> stripMaker{ m_MeshData };

	const Float3 holeInterval{ holeArray.GetHoleRadius() * 2 + holeArray.GetHoleGap(), 0,0 };

	//First/Left Cap
	MakeStartCap(holeArray, strips[1], bounds);
	strips[1].MoveEdges(holeInterval, m_MeshData);

	//Other Gaps
	for (unsigned iGap = 0; iGap < holeArray.GetNrGaps(); iGap++)
	{
		BaseClass::StartShape();
		Strip& strip{ strips[iGap % 2] };
		BaseClass::m_Result.Add(stripMaker.Make_Sharp(strip));
		if (iGap + 1 < holeArray.GetNrGaps())
			strip.MoveEdges(holeInterval * 2, m_MeshData);
	}

	//Last/Right Cap
	MakeEndCap(holeArray, strips[holeArray.GetNrGaps() % 2], bounds);

	return BaseClass::m_Result;
}
template<typename TVertex, ModelTopology TTopology>
inline void HoleArrayMaker<TVertex, TTopology>::GetNrSides(unsigned nrCircleCorners, unsigned& left, unsigned& right)
{
	const unsigned half{ Uint::Floor(nrCircleCorners * .5f) };

	left = half - 1 + (2 - half % 2) * (nrCircleCorners % 2);
	right = half - 1 + (1 + half % 2) * (nrCircleCorners % 2);
}
template<typename TVertex, ModelTopology TTopology>
inline unsigned HoleArrayMaker<TVertex, TTopology>::GetNrCornersPerArc(const Strip& gapStrip)
{
	return Uint::Ceil(gapStrip.GetNrEdges() * .5f);
}
template<typename TVertex, ModelTopology TTopology>
inline Strip HoleArrayMaker<TVertex, TTopology>::MakeGapStrip(const HoleArray& holeArray, unsigned nrSides)
{
	const float radius{ holeArray.GetHoleRadius() };
	const float gapDistance{ holeArray.GetHoleGap() };

	/*origin is at mathematical left-bot of the left/first hole */
	const unsigned nrCorners{ nrSides + 1 };

	Strip strip{};
	strip.EnsureEdgesSize(nrCorners);
	strip.EnsureNormalsSize(nrSides);

	//first vertex is 'left-top' (= highest of the left hole)
	//and considered the left-bot of the strip
	const float rightHoleX{ radius * 3 + gapDistance };
	const float angleStep{ Constants::PI2 / holeArray.GetNrCornersPerHole() };
	float angle{ nrSides * .5f * angleStep };

	for (unsigned iCorner = 0; iCorner < nrCorners; iCorner++, angle -= angleStep)
	{
		const float cornerX{ cosf(angle) * radius };
		MakerPointVertex left{ {
			cornerX + radius,
			0,
			sinf(angle) * radius + radius
		} };
		MakerPointVertex right{ {
				rightHoleX - cornerX,
				left.Position.y,
				left.Position.z
		} };
		strip.AddEdge(left, right);
		strip.AddNormal({ 0,1,0 });
	}
	return strip;
}
template<typename TVertex, ModelTopology TTopology>
inline void HoleArrayMaker<TVertex, TTopology>::MakeStartCap(
	const HoleArray& holeArray, const Strip& firstGap, RectFloat& bounds)
{
	if (firstGap.GetNrEdges() <= 2)
		return;
	const unsigned nrCornersPerArc{ GetNrCornersPerArc(firstGap) };

	const float gapOffset{ -(holeArray.GetHoleRadius() * 2 + holeArray.GetHoleGap()) };
	ArcMaker<TVertex, TTopology> arcMaker{ m_MeshData };

	//Find Bounds
	const unsigned topFirstEdgeIdx{ 0 };
	const unsigned topLastEdgeIdx{ nrCornersPerArc - 1 };
	const unsigned botFirstEdgeIdx{ firstGap.GetNrEdges() - nrCornersPerArc };
	const unsigned botLastEdge{ firstGap.GetNrEdges() - 1 };

	const MakerVertex& highestVertex{ firstGap.GetEdge(0)[1].Get() };
	const MakerVertex& mostLeftVertex{ firstGap.GetEdge(topLastEdgeIdx)[1].Get() };
	const MakerVertex& lowestVertex{ firstGap.GetEdge(botLastEdge)[1].Get() };

	bounds.SetLeft(GetPosition(mostLeftVertex).x + gapOffset);
	bounds.SetBottom(GetPosition(lowestVertex).z);
	bounds.SetTopUseHeight(GetPosition(highestVertex).z);

	//Setup Arc
	Arc arc{};
	arc.EnsureNrCorners(nrCornersPerArc);
	arc.SetNormal({ 0,1,0 });

	//Add Top Arc
	BaseClass::StartShape();
	arc.SetCenter(Float3::FromXz(bounds.GetLeftTop()));

	for (unsigned iCorner{ 0 }; iCorner < nrCornersPerArc; ++iCorner)
	{
		const Float3 stripPos{ GetPosition(firstGap.GetEdge(iCorner)[1]) };
		const Float3 pos{
			stripPos.x + gapOffset,
			stripPos.y,
			stripPos.z };

		arc.AddCorner(pos);
	}
	BaseClass::m_Result.Add(arcMaker.Make(arc));

	//Add Bot Arc
	BaseClass::StartShape();
	arc.ClearCorners();
	arc.SetCenter(Float3::FromXz(bounds.GetLeftBot()));

	for (unsigned iCorner{ botFirstEdgeIdx }; iCorner < firstGap.GetNrEdges(); ++iCorner)
	{
		const Float3 oppositePos{ GetPosition(firstGap.GetEdge(iCorner)[1]) };
		const Float3 pos{
			oppositePos.x + gapOffset,
			oppositePos.y,
			oppositePos.z };

		arc.AddCorner(pos);
	}
	BaseClass::m_Result.Add(arcMaker.Make(arc));
}
template<typename TVertex, ModelTopology TTopology>
inline void HoleArrayMaker<TVertex, TTopology>::MakeEndCap(
	const HoleArray& holeArray, const Strip& lastGap, RectFloat& bounds)
{
	if (lastGap.GetNrEdges() <= 2)
		return;
	const unsigned nrCornersPerArc{ GetNrCornersPerArc(lastGap) };

	ArcMaker<TVertex, TTopology> arcMaker{ BaseClass::m_MeshData };

	//Find Bounds
	const unsigned topLastEdgeIdx{ nrCornersPerArc - 1 };
	const MakerVertex& mostRightVertex{ lastGap.GetEdge(topLastEdgeIdx)[0].Get() };

	bounds.SetRightUseWidth(GetPosition(mostRightVertex).x);

	//Setup arc
	Arc arc{};
	arc.EnsureNrCorners(nrCornersPerArc);
	arc.SetNormal({ 0,1,0 });

	//Add Top Arc
	BaseClass::StartShape();
	arc.SetCenter(Float3::FromXz(bounds.GetRightTop()));
	for (unsigned iCorner{ nrCornersPerArc - 1 }; iCorner != static_cast<unsigned>(-1); --iCorner)
	{
		const Float3 stripPos{ GetPosition(lastGap.GetEdge(iCorner)[0]) };
		const Float3 pos{
			stripPos.x,
			stripPos.y,
			stripPos.z };
		arc.AddCorner(pos);
	}
	BaseClass::m_Result.Add(arcMaker.Make(arc));

	//Add Bot arc
	BaseClass::StartShape();
	arc.ClearCorners();
	arc.SetCenter(Float3::FromXz(bounds.GetRightBot()));
	for (unsigned iCorner{ lastGap.GetNrEdges() - 1 }; iCorner != static_cast<unsigned>(lastGap.GetNrEdges() - 1 - nrCornersPerArc); iCorner--)
	{
		const Float3 stripPos{ GetPosition(lastGap.GetEdge(iCorner)[0]) };
		const Float3 pos{
			stripPos.x,
			stripPos.y,
			stripPos.z };
		arc.AddCorner(pos);
	}
	BaseClass::m_Result.Add(arcMaker.Make(arc));
}

}
}