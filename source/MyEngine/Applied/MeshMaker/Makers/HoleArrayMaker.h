#pragma once

#include <DataStructures\Pointers\SharedPtr.h>
#include <Geometry\Shapes\Rects.h>

#include "..\Maker.h"
#include "..\Shapes\Arc.h"
#include "..\Shapes\HoleArray.h"
#include "..\Shapes\SmoothStrip.h"
#include "ArcMaker.h"
#include "SmoothStripMaker.h"
#include "..\Results\HoleArrayResult.h"

namespace MyEngine
{
namespace MeshMaker
{

#define TEMP_DEF template<typename TVertex, ModelTopology TTopology, typename TResult>
#define TEMP_ARG TVertex, TTopology, TResult

template<typename TVertex, ModelTopology TTopology, typename TResult = DefaultHoleArrayResult<TVertex, TTopology>>
class HoleArrayMaker
	: private Maker<TVertex, TTopology, TResult>
{
public:
	using BaseClass = Maker<TVertex, TTopology, TResult>;
	using PointType = typename BaseClass::DataType;
	using Maker<TVertex, TTopology, TResult>::Maker;

	TResult Make(const HoleArray& holeArray, RectFloat& bounds);

private:
	using BaseClass::Transform;
	using BaseClass::GetPosition;
	using BaseClass::m_MeshData;
	using BaseClass::m_Result;
	using BaseClass::Begin;
	using BaseClass::End;
	using ArcResult = ArcMakerResult<TVertex, TTopology>;

	static void GetNrSides(unsigned nrCircleCorners, unsigned& left, unsigned& right);
	static unsigned GetNrCornersPerArc(const SmoothStrip& gapStrip);

	SmoothStrip MakeGapStrip(const HoleArray& holeArray, unsigned nrSides);

	void MakeStartCap(const HoleArray& holeArray, const SmoothStrip& firstGap, SharedPtr<const MakerVertex>* pSharedVertices, RectFloat& bounds);
	void MakeEndCap(const HoleArray& holeArray, const SmoothStrip& lastGap, SharedPtr<const MakerVertex>* pSharedVertices, RectFloat& bounds);
};

TEMP_DEF
inline TResult HoleArrayMaker<TEMP_ARG>::Make(
	const HoleArray& holeArray, RectFloat& bounds)
{
	Begin();
	const unsigned nrGaps{ holeArray.GetNrHoles() - 1 };
	unsigned nrSidesLeft, nrSidesRight;
	GetNrSides(holeArray.GetNrCornersPerHole(), nrSidesLeft, nrSidesRight);

	//---| Gaps |---

	//Make vertices for 1 gap (and then copy & move them for others)
	//There is a strip for each side of the hole (left & right)
	SmoothStrip strips[]{
		MakeGapStrip(holeArray, nrSidesRight),
		MakeGapStrip(holeArray, nrSidesLeft)
	};

	//display -for now- with SharpStripMaker
	SmoothStripMaker<TVertex, TTopology, StripResult<TVertex, TTopology>> stripMaker{ m_MeshData };
	const Float3 holeInterval{ holeArray.GetHoleRadius() * 2 + holeArray.GetHoleGap(), 0,0 };

	//Shared 'horizontal' vertices (top&bot)
	SharedPtr<const MakerVertex> sharedVertices[2]{};

	//First/Left Cap
	MakeStartCap(holeArray, strips[1], sharedVertices, bounds);
	if (holeArray.GetNrHoles() > 1)
		strips[1].MoveEdges(holeInterval, m_MeshData);

	//Other Gaps
	for (unsigned iGap = 0; iGap < holeArray.GetNrGaps(); iGap++)
	{
		BaseClass::StartShape();
		SmoothStrip& strip{ strips[iGap % 2] };

		if (holeArray.GetNrCornersPerHole() % 2 == 1)
		{
			strip.SetEdgeBot(0, sharedVertices[1]);
			strip.SetEdgeBot(strip.GetNrEdges() - 1, sharedVertices[0]);
		}

		const StripResult stripResult{ stripMaker.Make(strip) };

		if (holeArray.GetNrCornersPerHole() % 2 == 1)
		{
			sharedVertices[0] = MakerRefVertex{ stripResult.GetLastEdgeTop() };
			sharedVertices[1] = MakerRefVertex{ stripResult.GetFirstEdgeTop() };
		}

		if (iGap + 1 < holeArray.GetNrGaps())
			strip.MoveEdges(holeInterval * 2, m_MeshData);
	}

	//Last/Right Cap
	MakeEndCap(holeArray, strips[holeArray.GetNrGaps() % 2], sharedVertices, bounds);
	End();

	return m_Result;
}

TEMP_DEF
inline void HoleArrayMaker<TEMP_ARG>::GetNrSides(unsigned nrCircleCorners, unsigned& left, unsigned& right)
{
	const unsigned half{ Uint::Floor(nrCircleCorners * .5f) };

	left = half - 1 + (2 - half % 2) * (nrCircleCorners % 2);
	right = half - 1 + (1 + half % 2) * (nrCircleCorners % 2);
}

TEMP_DEF
inline unsigned HoleArrayMaker<TEMP_ARG>::GetNrCornersPerArc(const SmoothStrip& gapStrip)
{
	return Uint::Ceil(gapStrip.GetNrEdges() * .5f);
}

TEMP_DEF
inline SmoothStrip HoleArrayMaker<TEMP_ARG>::MakeGapStrip(const HoleArray& holeArray, unsigned nrSides)
{
	const float radius{ holeArray.GetHoleRadius() };
	const float gapDistance{ holeArray.GetHoleGap() };

	/*origin is at mathematical left-bot of the left/first hole */
	const unsigned nrCorners{ nrSides + 1 };

	SmoothStrip strip{};
	strip.EnsureEdgesSize(nrCorners);
	strip.SetNormal({ 0,1,0 });

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
	}
	return strip;
}

TEMP_DEF
inline void HoleArrayMaker<TEMP_ARG>::MakeStartCap(
	const HoleArray& holeArray, const SmoothStrip& firstGap,
	SharedPtr<const MakerVertex>* pSharedVertices, RectFloat& bounds)
{
	const unsigned nrCornersPerArc{ GetNrCornersPerArc(firstGap) };
	const float gapOffset{ -(holeArray.GetHoleRadius() * 2 + holeArray.GetHoleGap()) };

	//Find Bounds
	const unsigned topFirstEdgeIdx{ 0 };
	const unsigned topLastEdgeIdx{ nrCornersPerArc - 1 };
	const unsigned botFirstEdgeIdx{ firstGap.GetNrEdges() - nrCornersPerArc };
	const unsigned botLastEdge{ firstGap.GetNrEdges() - 1 };

	SharedPtr<const MakerVertex> highestVertex{ firstGap.GetEdge(0)[1] };
	SharedPtr<const MakerVertex> mostLeftVertex{ firstGap.GetEdge(topLastEdgeIdx)[1] };
	SharedPtr<const MakerVertex> lowestVertex{ firstGap.GetEdge(botLastEdge)[1] };

	bounds.SetLeft(GetPosition(mostLeftVertex).x + gapOffset);
	bounds.SetBottom(GetPosition(lowestVertex).z);
	bounds.SetTopUseHeight(GetPosition(highestVertex).z);

	//Prepare creation
	if (firstGap.GetNrEdges() <= 2)
	{
		pSharedVertices[0] = MakerPointVertex{
			bounds.GetLeft(), 0, bounds.GetBottom() };
		pSharedVertices[1] = MakerPointVertex{
			bounds.GetLeft(), 0, bounds.GetTop() };
		return;
	}
	ArcMaker<TVertex, TTopology, ArcResult> arcMaker{ m_MeshData };

	//Setup Arc
	Arc arc{};
	arc.EnsureNrCorners(nrCornersPerArc);
	arc.SetNormal(firstGap.GetNormal());

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
	const ArcResult topArcResult{ arcMaker.Make(arc) };
	m_Result.SetLeftTop(topArcResult.GetCenter());

	//Store shared-vertex between arcs
	unsigned sharedVertex{};
	unsigned topArcStart{ botFirstEdgeIdx };
	if (firstGap.GetNrEdges() % 2 == 1)
	{
		sharedVertex = topArcResult.GetLastCorner();
		++topArcStart;
	}

	//Add Bot Arc
	BaseClass::StartShape();
	arc.ClearCorners();
	arc.SetCenter(Float3::FromXz(bounds.GetLeftBot()));

	if (firstGap.GetNrEdges() % 2 == 1)
		arc.AddCorner(MakerRefVertex{ sharedVertex });

	for (unsigned iCorner{ topArcStart }; iCorner < firstGap.GetNrEdges(); ++iCorner)
	{
		const Float3 oppositePos{ GetPosition(firstGap.GetEdge(iCorner)[1]) };
		const Float3 pos{
			oppositePos.x + gapOffset,
			oppositePos.y,
			oppositePos.z };

		arc.AddCorner(pos);
	}
	const ArcResult botArcResult{ arcMaker.Make(arc) };
	m_Result.SetLeftBot(botArcResult.GetCenter());

	//Store shared vertices between cap & gap
	if (holeArray.GetNrCornersPerHole() % 2 == 1)
	{
		pSharedVertices[0] = MakerRefVertex{ botArcResult.GetLastCorner() };
		pSharedVertices[1] = MakerRefVertex{ topArcResult.GetFirstCorner() };
	}
}

TEMP_DEF
inline void HoleArrayMaker<TEMP_ARG>::MakeEndCap(
	const HoleArray& holeArray, const SmoothStrip& lastGap,
	SharedPtr<const MakerVertex>* pSharedVertices, RectFloat& bounds)
{
	const unsigned nrCornersPerArc{ GetNrCornersPerArc(lastGap) };

	//Find Bounds
	const unsigned topLastEdgeIdx{ nrCornersPerArc - 1 };
	const MakerVertex& mostRightVertex{ lastGap.GetEdge(topLastEdgeIdx)[0].Get() };

	bounds.SetRightUseWidth(GetPosition(mostRightVertex).x);

	//Prepare creation
	if (lastGap.GetNrEdges() <= 2)
		return;
	ArcMaker<TVertex, TTopology, ArcResult> arcMaker{ BaseClass::m_MeshData };

	//Setup arc
	Arc arc{};
	arc.EnsureNrCorners(nrCornersPerArc);
	arc.SetNormal(lastGap.GetNormal());

	const unsigned topArcEnd{ -1 + (holeArray.GetNrCornersPerHole() % 2) };
	const unsigned botArcBegin{ lastGap.GetNrEdges() - 1 - (holeArray.GetNrCornersPerHole() % 2) };

	//Add Top Arc
	BaseClass::StartShape();
	arc.SetCenter(Float3::FromXz(bounds.GetRightTop()));
	for (unsigned iCorner{ nrCornersPerArc - 1 }; iCorner != topArcEnd; --iCorner)
	{
		const Float3 pos{ GetPosition(lastGap.GetEdge(iCorner)[0]) };
		arc.AddCorner(pos);
	}
	if (holeArray.GetNrCornersPerHole() % 2 == 1)
		arc.AddCorner(pSharedVertices[1]);

	const ArcResult topArcResult{ arcMaker.Make(arc) };
	m_Result.SetRightTop(topArcResult.GetCenter());

	unsigned sharedVertexIdx{};
	unsigned botArcEnd{ lastGap.GetNrEdges() - 1 - nrCornersPerArc };
	if (lastGap.GetNrEdges() % 2 == 1)
	{
		sharedVertexIdx = topArcResult.GetCorner(0);
		++botArcEnd;
	}

	//Add Bot arc
	BaseClass::StartShape();
	arc.ClearCorners();
	arc.SetCenter(Float3::FromXz(bounds.GetRightBot()));

	if (holeArray.GetNrCornersPerHole() % 2 == 1)
		arc.AddCorner(pSharedVertices[0]);

	for (unsigned iCorner{ botArcBegin }; iCorner != botArcEnd; iCorner--)
	{
		const Float3 pos{ GetPosition(lastGap.GetEdge(iCorner)[0]) };
		arc.AddCorner(pos);
	}

	if (lastGap.GetNrEdges() % 2 == 1)
		arc.AddCorner(MakerRefVertex{ sharedVertexIdx });

	const ArcResult botArcResult{ arcMaker.Make(arc) };
	m_Result.SetRightBot(botArcResult.GetCenter());
}

}
}