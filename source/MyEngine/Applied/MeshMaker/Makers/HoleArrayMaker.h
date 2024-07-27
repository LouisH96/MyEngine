#pragma once

#include <DataStructures\Pointers\SharedPtr.h>

#include "..\Shapes\HoleArray.h"
#include "..\Shapes\Strip.h"
#include "..\Shapes\Line.h"
#include "..\Maker.h"
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
	using BaseClass::Transform;
	using Maker<TVertex, TTopology>::Maker;

	MakerResult Make(const HoleArray& holeArray);

private:
	Strip MakeGapStrip(const HoleArray& holeArray);

	void MakeStartCap(const HoleArray& holeArray, const Strip& firstGap);
	void MakeEndCap(const HoleArray& holeArray, const Strip& lastGap);
};
template<typename TVertex, ModelTopology TTopology>
inline MakerResult HoleArrayMaker<TVertex, TTopology>::Make(const HoleArray& holeArray)
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
	//BaseClass::StartShape();

	//Other Gaps

	//Last/Right Cap


	for (unsigned iGap = 0; iGap < holeArray.GetNrGaps(); iGap++)
	{
		BaseClass::StartShape();
		BaseClass::m_Result.Add(stripMaker.Make_Sharp(strip));
		strip.AdaptEdges(moveAdaptor);
	}

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
inline void HoleArrayMaker<TVertex, TTopology>::MakeStartCap(const HoleArray& holeArray, const Strip& firstGap)
{
	const PointType leftTop{ Transform({
		0, 0, holeArray.GetHoleRadius() * 2}) };
	const PointType leftBot{ Transform({0,0,0}) };
}
template<typename TVertex, ModelTopology TTopology>
inline void HoleArrayMaker<TVertex, TTopology>::MakeEndCap(const HoleArray& holeArray, const Strip& lastGap)
{
}

}
}