#pragma once

#include <DataStructures\Pointers\SharedPtr.h>
#include <Geometry\Shapes\Rects.h>

#include "..\Shapes\Arc.h"
#include "..\Shapes\HoleArray.h"
#include "..\Shapes\Strip.h"
#include "..\Shapes\Line.h"
#include "..\Maker.h"
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

	Strip MakeGapStrip(const HoleArray& holeArray);

	void MakeStartCap(const HoleArray& holeArray, const Strip& firstGap, RectFloat& bounds);
	void MakeEndCap(const HoleArray& holeArray, const Strip& lastGap, RectFloat& bounds);
};
template<typename TVertex, ModelTopology TTopology>
inline MakerResult HoleArrayMaker<TVertex, TTopology>::Make(
	const HoleArray& holeArray, RectFloat& bounds)
{
	/*only even nrCorners, for now*/

	const unsigned nrCornersPerSide{ Uint::Floor(holeArray.GetNrCornersPerHole() / 2) };
	const unsigned nrQuadsPerSide{ nrCornersPerSide - 1 };
	const unsigned nrGaps{ holeArray.GetNrHoles() - 1 };

	//---| Gaps |---

	//Make vertices for 1 gap (and then copy & move them for others)
	//note: for odd nrCorners there will be shared vertices
	Strip strip{ MakeGapStrip(holeArray) };

	//display -for now- with SharpStripMaker
	StripMaker<TVertex, TTopology> stripMaker{ BaseClass::m_MeshData };

	const float moveDistance{ holeArray.GetHoleRadius() * 2 + holeArray.GetHoleGap() };
	auto moveAdaptor = [this, &moveDistance](Line& edge)
		{
			for (unsigned iVertex{ 0 }; iVertex < Line::NrVertices; iVertex++)
				edge[iVertex] = SharedPtr<const MakerVertex>{ MakerPointVertex{
						MeshMakerHelper::GetPosition(edge[iVertex].Get(), BaseClass::m_MeshData)
						+ Float3{moveDistance, 0, 0}
				} };
		};

	//First/Left Cap
	BaseClass::StartShape();
	MakeStartCap(holeArray, strip, bounds);

	//Other Gaps
	for (unsigned iGap = 0; iGap < holeArray.GetNrGaps(); iGap++)
	{
		BaseClass::StartShape();
		BaseClass::m_Result.Add(stripMaker.Make_Sharp(strip));
		if (iGap + 1 < holeArray.GetNrGaps())
			strip.AdaptEdges(moveAdaptor);
	}

	//Last/Right Cap
	BaseClass::StartShape();
	MakeEndCap(holeArray, strip, bounds);

	return BaseClass::m_Result;
}
template<typename TVertex, ModelTopology TTopology>
inline Strip HoleArrayMaker<TVertex, TTopology>::MakeGapStrip(const HoleArray& holeArray)
{
	const float radius{ holeArray.GetHoleRadius() };
	const unsigned nrGaps{ holeArray.GetNrGaps() };
	const float gapDistance{ holeArray.GetHoleGap() };

	/*origin is at left-bot of the left/first hole */
	const unsigned nrCornersPerSide{ Uint::Floor(holeArray.GetNrCornersPerHole() / 2) };
	const unsigned nrQuadsPerSide{ nrCornersPerSide - 1 };

	Strip strip{};
	strip.EnsureEdgesSize(nrCornersPerSide);
	strip.EnsureNormalsSize(nrQuadsPerSide);

	//first vertex is 'left-top' (= highest of the left hole)
	//and considered the left-bot of the strip
	const float rightHoleX{ radius * 3 + gapDistance };
	const float angleStep{ Constants::PI2 / holeArray.GetNrCornersPerHole() };
	float angle{ (nrCornersPerSide - 1) * .5f * angleStep };

	for (unsigned iCorner = 0; iCorner < nrCornersPerSide; iCorner++, angle -= angleStep)
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
	const float gapOffset{ -(holeArray.GetHoleRadius() * 2 + holeArray.GetHoleGap()) };
	const unsigned nrCornersPerArc{ Uint::Ceil(firstGap.GetNrEdges() / 2.f) };
	ArcMaker<TVertex, TTopology> arcMaker{ BaseClass::m_MeshData };

	//get stripVertices
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

	Arc arc{};
	arc.EnsureNrCorners(nrCornersPerArc);
	arc.SetNormal({ 0,1,0 });

	//top
	arc.SetCenter(Float3::FromXz(bounds.GetLeftTop()));

	for (unsigned iCorner{ 0 }; iCorner < nrCornersPerArc; ++iCorner)
	{
		const Float3 oppositePos{ GetPosition(firstGap.GetEdge(iCorner)[1]) };
		const Float3 pos{
			oppositePos.x + gapOffset,
			oppositePos.y,
			oppositePos.z };

		arc.AddCorner(pos);
	}
	BaseClass::m_Result.Add(arcMaker.Make(arc));

	//bot
	BaseClass::StartShape();
	arc.ClearCorners();
	arc.SetCenter(Float3::FromXz(bounds.GetLeftBot()));

	for (unsigned iCorner{ firstGap.GetNrEdges() - nrCornersPerArc }; iCorner < firstGap.GetNrEdges(); ++iCorner)
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
	const float gapOffset{ holeArray.GetHoleRadius() * 2 + holeArray.GetHoleGap() };
	const unsigned nrCornersPerArc{ Uint::Ceil(lastGap.GetNrEdges() / 2.f) };
	ArcMaker<TVertex, TTopology> arcMaker{ BaseClass::m_MeshData };

	//Find right edge
	const unsigned topLastEdgeIdx{ nrCornersPerArc - 1 };
	const MakerVertex& mostRightVertex{ lastGap.GetEdge(topLastEdgeIdx)[0].Get() };

	bounds.SetRightUseWidth(GetPosition(mostRightVertex).x + gapOffset);

	//Setup arc
	Arc arc{};
	arc.EnsureNrCorners(nrCornersPerArc);
	arc.SetNormal({ 0,1,0 });

	//Top Arc
	arc.SetCenter(Float3::FromXz(bounds.GetRightTop()));
	for (unsigned iCorner{ nrCornersPerArc - 1 }; iCorner < Uint::Max(); --iCorner)
	{
		const Float3 stripPos{ GetPosition(lastGap.GetEdge(iCorner)[0]) };
		const Float3 pos{
			stripPos.x + gapOffset,
			stripPos.y,
			stripPos.z };
		arc.AddCorner(pos);
	}
	BaseClass::m_Result.Add(arcMaker.Make(arc));

	//Bot arc
	BaseClass::StartShape();
	arc.ClearCorners();
	arc.SetCenter(Float3::FromXz(bounds.GetRightBot()));
	for (unsigned iCorner{ lastGap.GetNrEdges() - 1 }; iCorner >= lastGap.GetNrEdges() - nrCornersPerArc; --iCorner)
	{
		const Float3 stripPos{ GetPosition(lastGap.GetEdge(iCorner)[0]) };
		const Float3 pos{
			stripPos.x + gapOffset,
			stripPos.y,
			stripPos.z };
		arc.AddCorner(pos);
	}
	BaseClass::m_Result.Add(arcMaker.Make(arc));
}

}
}