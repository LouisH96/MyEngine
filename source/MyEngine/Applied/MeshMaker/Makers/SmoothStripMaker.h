#pragma once

#include "..\Maker.h"
#include "..\Shapes\SmoothStrip.h"

namespace MyEngine
{
namespace MeshMaker
{
#define MAKER_RESULT MakerResult
#define BASE_CLASS Maker<TVertex, TTopology, MAKER_RESULT>

template<typename TVertex, ModelTopology TTopology>
class SmoothStripMaker
	: private BASE_CLASS
{
public:
	using BaseClass = BASE_CLASS;
	using PointId = typename BaseClass::DataType;
	using Result = MAKER_RESULT;

	SmoothStripMaker(MeshData<TVertex, TTopology>& meshData);

	Result Make(const SmoothStrip& strip);

private:
	using BaseClass::Add;
	using BaseClass::Transform;

	void AddPhase_LineList(const SmoothStrip& strip, const Array<PointId>& points);
	void AddPhase_LineStrip(const SmoothStrip& strip, const Array<PointId>& points);
	void AddPhase_TriangleList(const SmoothStrip& strip, const Array<PointId>& points);
	void AddPhase_TriangleStrip(const SmoothStrip& strip, const Array<PointId>& points);
};

#undef BASE_CLASS
#undef MAKER_RESULT

template<typename TVertex, ModelTopology TTopology>
inline SmoothStripMaker<TVertex, TTopology>::SmoothStripMaker(MeshData<TVertex, TTopology>& meshData)
	: BaseClass{ meshData }
{
}
template<typename TVertex, ModelTopology TTopology>
inline typename SmoothStripMaker<TVertex, TTopology>::Result SmoothStripMaker<TVertex, TTopology>::Make(const SmoothStrip& strip)
{
	//Transform phase
	Array<PointId> points{ strip.GetNrEdges() * 2 };
	for (unsigned iEdge{ 0 }; iEdge < strip.GetNrEdges(); ++iEdge)
	{
		const Line& edge{ strip.GetEdge(iEdge) };
		points[iEdge * 2 + 0] = Transform(edge[0].Get());
		points[iEdge * 2 + 1] = Transform(edge[1].Get());
	}
	BaseClass::FinishTransformPhase(points);

	//Add phase
	switch (TopologyInfo::GetBaseType(TTopology))
	{
	case TopologyInfo::BaseType::LineList:
		AddPhase_LineList(strip, points);
		break;
	case TopologyInfo::BaseType::LineStrip:
		AddPhase_LineStrip(strip, points);
		break;
	case TopologyInfo::BaseType::TriangleList:
		AddPhase_TriangleList(strip, points);
		break;
	case TopologyInfo::BaseType::TriangleStrip:
		AddPhase_TriangleStrip(strip, points);
		break;
	}
	return BaseClass::m_Result;
}
template<typename TVertex, ModelTopology TTopology>
inline void SmoothStripMaker<TVertex, TTopology>::AddPhase_LineList(const SmoothStrip& strip, const Array<PointId>& points)
{
	const Float3& normal{ strip.GetNormal() };

	//Vertical Lines
	for (unsigned iEdge{ 0 }; iEdge < strip.GetNrEdges(); ++iEdge)
	{
		Add(points[iEdge * 2 + 0], normal);
		Add(points[iEdge * 2 + 1], normal);
	}

	//Horizontal Lines
	for (unsigned iEdge{ 0 }; iEdge + 1 < strip.GetNrEdges(); ++iEdge)
	{
		Add(points[iEdge * 2 + 0], normal);
		Add(points[iEdge * 2 + 2], normal);

		Add(points[iEdge * 2 + 1], normal);
		Add(points[iEdge * 2 + 3], normal);
	}
}
template<typename TVertex, ModelTopology TTopology>
inline void SmoothStripMaker<TVertex, TTopology>::AddPhase_LineStrip(const SmoothStrip& strip, const Array<PointId>& points)
{
	const Float3& normal{ strip.GetNormal() };
	Add(points[0], normal);

	for (unsigned iWall{ 0 }; iWall < strip.GetNrWalls(); ++iWall)
	{
		Add(points[iWall * 2 + 1 - iWall % 2], normal);
		Add(points[iWall * 2 + 3 - iWall % 2], normal);
	}
	for (unsigned iWall{ strip.GetNrWalls() - 1 }; iWall != static_cast<unsigned>(-1); --iWall)
	{
		Add(points[iWall * 2 + 2 + iWall % 2], normal);
		Add(points[iWall * 2 + 0 + iWall % 2], normal);
	}
}
template<typename TVertex, ModelTopology TTopology>
inline void SmoothStripMaker<TVertex, TTopology>::AddPhase_TriangleList(const SmoothStrip& strip, const Array<PointId>& points)
{
	const Float3& normal{ strip.GetNormal() };

	for (unsigned iWall{ 0 }; iWall < strip.GetNrWalls(); ++iWall)
	{
		Add(points[iWall * 2 + 0], normal);
		Add(points[iWall * 2 + 1], normal);
		Add(points[iWall * 2 + 2], normal);

		Add(points[iWall * 2 + 1], normal);
		Add(points[iWall * 2 + 3], normal);
		Add(points[iWall * 2 + 2], normal);
	}
}
template<typename TVertex, ModelTopology TTopology>
inline void SmoothStripMaker<TVertex, TTopology>::AddPhase_TriangleStrip(const SmoothStrip& strip, const Array<PointId>& points)
{
	const Float3& normal{ strip.GetNormal() };

	for (unsigned i{ 0 }; i < points.GetSize(); ++i)
		Add(points[i], normal);
}
}
}