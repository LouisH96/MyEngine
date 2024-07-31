#pragma once

#include "..\Maker.h"
#include "..\Shapes\Arc.h"
#include "ArcMakerResult.h"

namespace MyEngine
{
namespace MeshMaker
{
template<typename TVertex, ModelTopology TTopology,
	typename TResult = DefaultArcMakerResult<TVertex, TTopology>>
	class ArcMaker
	: private Maker<TVertex, TTopology, TResult>
{
public:
	using BaseClass = Maker<TVertex, TTopology, TResult>;
	using PointType = typename BaseClass::DataType;
	using Maker<TVertex, TTopology, TResult>::Maker;

	TResult Make(const Arc& arc);

private:
	using BaseClass::Transform;
	using BaseClass::Add;
	using BaseClass::m_Result;

	void Add_LineList(const Array<PointType>& points, const Arc& arc);
	void Add_LineStrip(const Array<PointType>& points, const Arc& arc);
	void Add_TriangleList(const Array<PointType>& points, const Arc& arc);
	void Add_TriangleStrip(const Array<PointType>& points, const Arc& arc);

};
template<typename TVertex, ModelTopology TTopology, typename TResult>
inline TResult ArcMaker<TVertex, TTopology, TResult>::Make(const Arc& arc)
{
	m_Result.SetNrCorners(arc.GetNrCorners());

	//---| Transform Phase |---
	Array<PointType> points{ arc.GetNrCorners() + 1 };

	points[0] = Transform(arc.GetCenter().Get());
	for (unsigned iCorner{ 0 }; iCorner < arc.GetNrCorners(); iCorner++)
		points[iCorner + 1] = Transform(arc.GetCorner(iCorner).Get());


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
template<typename TVertex, ModelTopology TTopology, typename TResult>
inline void ArcMaker<TVertex, TTopology, TResult>::Add_LineList(const Array<PointType>& points, const Arc& arc)
{
	const Float3& normal{ arc.GetNormal() };

	//Corners/Outer Line
	for (unsigned iCorner{ 2 }; iCorner < points.GetSize(); ++iCorner)
	{
		const unsigned iFirst{ Add(points[iCorner - 1], normal) };
		const unsigned iSecond{ Add(points[iCorner], normal) };

		m_Result.SetCorner(iCorner - 2, iFirst);
		m_Result.SetCorner(iCorner - 1, iSecond);
	}

	//Inner Lines
	for (unsigned iCorner{ 1 }; iCorner < points.GetSize(); ++iCorner)
	{
		const unsigned iVertex{ Add(points[0], normal) };
		Add(points[iCorner], normal);

		m_Result.SetCenter(iVertex);
	}
}
template<typename TVertex, ModelTopology TTopology, typename TResult>
inline void ArcMaker<TVertex, TTopology, TResult>::Add_LineStrip(const Array<PointType>& points, const Arc& arc)
{
	const Float3& normal{ arc.GetNormal() };

	//Zig-Zag
	for (unsigned iCorner{ 1 }; iCorner < points.GetSize(); ++iCorner)
	{
		if (iCorner % 2 == 0)
		{
			m_Result.SetCorner(iCorner - 1,
				Add(points[iCorner], normal));
			m_Result.SetCenter(
				Add(points[0], normal));
		}
		else
		{
			m_Result.SetCenter(
				Add(points[0], normal));
			m_Result.SetCorner(iCorner - 1,
				Add(points[iCorner], normal));
		}
	}
	if (points.GetSize() % 2 == 1)
		Add(points[points.GetSize() - 2], normal);

	for (unsigned iCorner{ points.GetSize() - 2 }; iCorner > 1; --iCorner)
		Add(points[iCorner], normal);
}
template<typename TVertex, ModelTopology TTopology, typename TResult>
inline void ArcMaker<TVertex, TTopology, TResult>::Add_TriangleList(const Array<PointType>& points, const Arc& arc)
{
	const Float3& normal{ arc.GetNormal() };

	for (unsigned iCorner{ 1 }; iCorner + 1 < points.GetSize(); ++iCorner)
	{
		m_Result.SetCenter(Add(points[0], normal));
		m_Result.SetCorner(iCorner - 1, Add(points[iCorner], normal));
		m_Result.SetCorner(iCorner, Add(points[iCorner + 1], normal));
	}
}
template<typename TVertex, ModelTopology TTopology, typename TResult>
inline void ArcMaker<TVertex, TTopology, TResult>::Add_TriangleStrip(const Array<PointType>& points, const Arc& arc)
{
	const Float3& normal{ arc.GetNormal() };

	m_Result.SetCorner(0, Add(points[1], normal));
	m_Result.SetCorner(1, Add(points[2], normal));
	m_Result.SetCenter(Add(points[0], normal));

	unsigned iCorner{ 3 };
	for (; iCorner + 2 < points.GetSize(); iCorner += 2)
	{
		m_Result.SetCorner(iCorner - 1, Add(points[iCorner], normal));
		m_Result.SetCorner(iCorner, Add(points[iCorner + 1], normal));
		Add(points[iCorner + 1], normal);
		Add(points[0], normal);
	}
	for (; iCorner < points.GetSize(); iCorner++)
		m_Result.SetCorner(iCorner - 1, Add(points[iCorner], normal));
}
}
}