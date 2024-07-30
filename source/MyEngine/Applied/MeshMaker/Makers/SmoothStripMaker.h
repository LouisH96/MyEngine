#pragma once

#include "..\Maker.h"
#include "..\Shapes\SmoothStrip.h"
#include "StripResult.h"

namespace MyEngine
{
namespace MeshMaker
{
#define BASE_CLASS Maker<TVertex, TTopology, TResult>

template<typename TVertex, ModelTopology TTopology, typename TResult = DefaultStripResult>
class SmoothStripMaker
	: private BASE_CLASS
{
public:
	using BaseClass = BASE_CLASS;
	using PointId = typename BaseClass::DataType;
	using Result = TResult;

	SmoothStripMaker(MeshData<TVertex, TTopology>& meshData);

	Result Make(const SmoothStrip& strip);

private:
	using BaseClass::Add;
	using BaseClass::Transform;
	using BaseClass::m_Result;

	void AddPhase_LineList(const SmoothStrip& strip, const Array<PointId>& points);
	void AddPhase_LineStrip(const SmoothStrip& strip, const Array<PointId>& points);
	void AddPhase_TriangleList(const SmoothStrip& strip, const Array<PointId>& points);
	void AddPhase_TriangleStrip(const SmoothStrip& strip, const Array<PointId>& points);
};

#undef BASE_CLASS
#undef MAKER_RESULT

template<typename TVertex, ModelTopology TTopology, typename TResult>
inline SmoothStripMaker<TVertex, TTopology, TResult>::SmoothStripMaker(MeshData<TVertex, TTopology>& meshData)
	: BaseClass{ meshData }
{
}
template<typename TVertex, ModelTopology TTopology, typename TResult>
inline typename SmoothStripMaker<TVertex, TTopology, TResult>::Result SmoothStripMaker<TVertex, TTopology, TResult>::Make(const SmoothStrip& strip)
{
	m_Result.SetNrEdges(strip.GetNrEdges());

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
template<typename TVertex, ModelTopology TTopology, typename TResult>
inline void SmoothStripMaker<TVertex, TTopology, TResult>::AddPhase_LineList(const SmoothStrip& strip, const Array<PointId>& points)
{
	const Float3& normal{ strip.GetNormal() };

	//Vertical Lines
	for (unsigned iEdge{ 0 }; iEdge < strip.GetNrEdges(); ++iEdge)
	{
		m_Result.SetEdgeBot(iEdge, Add(points[iEdge * 2 + 0], normal));
		m_Result.SetEdgeTop(iEdge, Add(points[iEdge * 2 + 1], normal));
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
template<typename TVertex, ModelTopology TTopology, typename TResult>
inline void SmoothStripMaker<TVertex, TTopology, TResult>::AddPhase_LineStrip(const SmoothStrip& strip, const Array<PointId>& points)
{
	const Float3& normal{ strip.GetNormal() };
	m_Result.SetEdgeBot(0, Add(points[0], normal));

	for (unsigned iWall{ 0 }; iWall < strip.GetNrWalls(); ++iWall)
	{
		const unsigned invMod{ 1 - iWall % 2 };
		m_Result.SetPoint(iWall, invMod,
			Add(points[iWall * 2 + invMod], normal));
		m_Result.SetPoint(iWall + 1, invMod,
			Add(points[iWall * 2 + 2 + invMod], normal));
	}
	for (unsigned iWall{ strip.GetNrWalls() - 1 }; iWall != static_cast<unsigned>(-1); --iWall)
	{
		const unsigned mod{ iWall % 2 };
		m_Result.SetPoint(iWall + 1, mod,
			Add(points[iWall * 2 + 2 + mod], normal));
		Add(points[iWall * 2 + 0 + mod], normal);
	}
}
template<typename TVertex, ModelTopology TTopology, typename TResult>
inline void SmoothStripMaker<TVertex, TTopology, TResult>::AddPhase_TriangleList(const SmoothStrip& strip, const Array<PointId>& points)
{
	const Float3& normal{ strip.GetNormal() };

	for (unsigned iWall{ 0 }; iWall < strip.GetNrWalls(); ++iWall)
	{
		m_Result.SetEdgeBot(iWall,
			Add(points[iWall * 2 + 0], normal));
		m_Result.SetEdgeTop(iWall,
			Add(points[iWall * 2 + 1], normal));
		m_Result.SetEdgeBot(iWall + 1,
			Add(points[iWall * 2 + 2], normal));

		Add(points[iWall * 2 + 1], normal);
		m_Result.SetEdgeTop(iWall + 1,
			Add(points[iWall * 2 + 3], normal));
		Add(points[iWall * 2 + 2], normal);
	}
}
template<typename TVertex, ModelTopology TTopology, typename TResult>
inline void SmoothStripMaker<TVertex, TTopology, TResult>::AddPhase_TriangleStrip(const SmoothStrip& strip, const Array<PointId>& points)
{
	const Float3& normal{ strip.GetNormal() };

	for (unsigned i{ 0 }; i < points.GetSize(); ++i)
	{
		m_Result.SetPoint(i / 2, i % 2,
			Add(points[i], normal));
	}
}
}
}