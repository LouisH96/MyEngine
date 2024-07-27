#pragma once

#include "..\Maker.h"
#include "..\Shapes\Arc.h"

namespace MyEngine
{
namespace MeshMaker
{
template<typename TVertex, ModelTopology TTopology>
class ArcMaker
	: private Maker<TVertex, TTopology>
{
public:
	using BaseClass = Maker<TVertex, TTopology>;
	using PointType = typename BaseClass::DataType;
	using Maker<TVertex, TTopology>::Maker;

	MakerResult Make(const Arc& arc);

private:
	using BaseClass::Transform;
	using BaseClass::Add;

	void Add_LineList(const Array<PointType>& points, const Arc& arc);
	void Add_LineStrip(const Array<PointType>& points, const Arc& arc);
	void Add_TriangleList(const Array<PointType>& points, const Arc& arc);
	void Add_TriangleStrip(const Array<PointType>& points, const Arc& arc);

};
template<typename TVertex, ModelTopology TTopology>
inline MakerResult ArcMaker<TVertex, TTopology>::Make(const Arc& arc)
{
	//---| Transform Phase |---
	Array<PointType> points{ arc.GetNrCorners() + 1 };

	points[0] = Transform(arc.GetCenter().Get());
	for (unsigned iCorner{ 0 }; iCorner < arc.GetNrCorners(); iCorner++)
		points[iCorner + 1] = Transform(arc.GetCorner(iCorner).Get());

	BaseClass::FinishTransformPhase(points);

	//---| Add Phase |---
	switch (TopologyInfo::GetBaseType(TTopology))
	{
	case TopologyInfo::BaseType::LineList:
		Add_LineList(points, arc);
		break;
	case TopologyInfo::BaseType::LineStrip:
		Add_LineStrip(points, arc);
		break;
	case TopologyInfo::BaseType::TriangleList:
		Add_TriangleList(points, arc);
		break;
	case TopologyInfo::BaseType::TriangleStrip:
		Add_TriangleStrip(points, arc);
		break;
	}

	return BaseClass::m_Result;
}
template<typename TVertex, ModelTopology TTopology>
inline void ArcMaker<TVertex, TTopology>::Add_LineList(const Array<PointType>& points, const Arc& arc)
{
	const Float3& normal{ arc.GetNormal() };

	//Corners/Outer Line
	for (unsigned iCorner{ 2 }; iCorner < points.GetSize(); ++iCorner)
	{
		Add(points[iCorner - 1], normal);
		Add(points[iCorner], normal);
	}

	//Inner Lines
	for (unsigned iCorner{ 1 }; iCorner < points.GetSize(); ++iCorner)
	{
		Add(points[0], normal);
		Add(points[iCorner], normal);
	}
}
template<typename TVertex, ModelTopology TTopology>
inline void ArcMaker<TVertex, TTopology>::Add_LineStrip(const Array<PointType>& points, const Arc& arc)
{
	const Float3& normal{ arc.GetNormal() };

	//Zig-Zag
	for (unsigned iCorner{ 1 }; iCorner < points.GetSize(); ++iCorner)
	{
		if (iCorner % 2 == 0)
		{
			Add(points[iCorner], normal);
			Add(points[0], normal);
		}
		else
		{
			Add(points[0], normal);
			Add(points[iCorner], normal);
		}
	}
	if (points.GetSize() % 2 == 1)
		Add(points[points.GetSize() - 2], normal);

	for (unsigned iCorner{ points.GetSize() - 2 }; iCorner > 1; --iCorner)
		Add(points[iCorner], normal);
}
template<typename TVertex, ModelTopology TTopology>
inline void ArcMaker<TVertex, TTopology>::Add_TriangleList(const Array<PointType>& points, const Arc& arc)
{
	const Float3& normal{ arc.GetNormal() };

	for (unsigned iCorner{ 1 }; iCorner + 1 < points.GetSize(); ++iCorner)
	{
		Add(points[0], normal);
		Add(points[iCorner], normal);
		Add(points[iCorner + 1], normal);
	}
}
template<typename TVertex, ModelTopology TTopology>
inline void ArcMaker<TVertex, TTopology>::Add_TriangleStrip(const Array<PointType>& points, const Arc& arc)
{
	const Float3& normal{ arc.GetNormal() };

	Add(points[1], normal);
	Add(points[2], normal);
	Add(points[0], normal);

	unsigned iCorner{ 3 };
	for (; iCorner + 2 < points.GetSize(); iCorner += 2)
	{
		Add(points[iCorner], normal);
		Add(points[iCorner + 1], normal);
		Add(points[iCorner + 1], normal);
		Add(points[0], normal);
	}
	for (; iCorner < points.GetSize(); iCorner++)
		Add(points[iCorner], normal);
}
}
}