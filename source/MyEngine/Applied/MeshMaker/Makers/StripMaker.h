#pragma once

#include "Maker.h"
#include "..\Strip.h"

namespace MyEngine
{
namespace MeshMaker
{
template<typename Vertex, ModelTopology Topology>
class StripMaker
	: public Maker<Vertex, Topology>
{
public:
	using BaseClass = Maker<Vertex, Topology>;
	using typename BaseClass::DataType;

	StripMaker(MeshData<Vertex, Topology>& meshData);

	MakerResult Make(const Strip& strip);

private:
	void AddPhase_LineList(const Array<DataType>& data, const Strip& strip);
	void AddPhase_LineStrip(const Array<DataType>& data, const Strip& strip);
	void AddPhase_TriangleList(const Array<DataType>& data, const Strip& strip);
};
template<typename Vertex, ModelTopology Topology>
inline StripMaker<Vertex, Topology>::StripMaker(MeshData<Vertex, Topology>& meshData)
	: BaseClass{ meshData }
{
}
template<typename Vertex, ModelTopology Topology>
inline MakerResult StripMaker<Vertex, Topology>::Make(const Strip& strip)
{
	constexpr TopologyInfo::BaseType baseType{ TopologyInfo::GetBaseType(Topology) };
	const unsigned nrEdges{ strip.GetEdges().GetSize() };
	const unsigned nrWalls{ nrEdges - 1 };

	//Transform phase
	Array<DataType> data{ nrEdges * 2 };
	for (unsigned iEdge{ 0 }, iData{ 0 }; iEdge < nrEdges; iEdge++)
	{
		const Strip::Edge& edge{ strip.GetEdges()[iEdge] };

		data[iData++] = BaseClass::Transform(edge.Bottom.Get());
		data[iData++] = BaseClass::Transform(edge.Top.Get());
	}
	if constexpr (TopologyInfo::HasIndices(Topology))
		BaseClass::AddAllToResult({ data });

	//Add phase
	if constexpr (baseType == TopologyInfo::BaseType::LineList)
		AddPhase_LineList(data, strip);
	else if constexpr (baseType == TopologyInfo::BaseType::LineStrip)
		AddPhase_LineStrip(data, strip);
	else if constexpr (baseType == TopologyInfo::BaseType::TriangleList)
		AddPhase_TriangleList(data, strip);

	return BaseClass::m_Result;
}
template<typename Vertex, ModelTopology Topology>
inline void StripMaker<Vertex, Topology>::AddPhase_LineList(const Array<DataType>& data, const Strip& strip)
{
	const unsigned nrEdges{ strip.GetEdges().GetSize() };
	const unsigned nrWalls{ nrEdges - 1 };

	//vertical lines
	for (unsigned iEdge{ 0 }; iEdge < nrEdges; iEdge++)
	{
		const unsigned bot{ iEdge * 2 };
		const unsigned top{ bot + 1 };
		BaseClass::Add(data[bot]);
		BaseClass::Add(data[top]);
	}

	//horizontal line(s)
	for (unsigned iWall{ 0 }; iWall < nrWalls; iWall++)
	{
		const unsigned leftBot{ iWall * 2 };
		const unsigned leftTop{ leftBot + 1 };
		const unsigned rightBot{ leftTop + 1 };
		const unsigned rightTop{ rightBot + 1 };
		BaseClass::Add(data[leftBot]);
		BaseClass::Add(data[rightBot]);
		BaseClass::Add(data[leftTop]);
		BaseClass::Add(data[rightTop]);
	}
}
template<typename Vertex, ModelTopology Topology>
inline void StripMaker<Vertex, Topology>::AddPhase_LineStrip(const Array<DataType>& data, const Strip& strip)
{
	const unsigned nrEdges{ strip.GetEdges().GetSize() };
	const unsigned nrWalls{ nrEdges - 1 };

	//first zig-zag
	BaseClass::Add(data.First());
	for (unsigned iWall{ 0 }; iWall < nrWalls; iWall++)
	{
		const unsigned first{ iWall * 2 + 1 - iWall % 2 };
		const unsigned second{ first + 2 };
		BaseClass::Add(data[first]);
		BaseClass::Add(data[second]);
	}
	BaseClass::Add(data[nrWalls * 2 + 1 - nrWalls % 2]);

	//second zig-zag
	for (unsigned iWall{ nrWalls - 1 }; iWall > 0; iWall--)
	{
		const unsigned first{ iWall * 2 + iWall % 2 };
		const unsigned second{ first + 1 - iWall % 2 * 2 };
		BaseClass::Add(data[first]);
		BaseClass::Add(data[second]);
	}
	BaseClass::Add(data.First());
}
template<typename Vertex, ModelTopology Topology>
inline void StripMaker<Vertex, Topology>::AddPhase_TriangleList(const Array<DataType>& data, const Strip& strip)
{
	const unsigned nrEdges{ strip.GetEdges().GetSize() };
	const unsigned nrWalls{ nrEdges - 1 };

	for (unsigned iWall{ 0 }; iWall < nrWalls; iWall++)
	{
		const unsigned leftBot{ iWall * 2 };
		const unsigned leftTop{ leftBot + 1 };
		const unsigned rightBot{ leftTop + 1 };
		const unsigned rightTop{ rightBot + 1 };
		BaseClass::Add(data[leftBot]); //left-bot triangle
		BaseClass::Add(data[leftTop]);
		BaseClass::Add(data[rightBot]);
		BaseClass::Add(data[leftTop]); //right-top triangle
		BaseClass::Add(data[rightTop]);
		BaseClass::Add(data[rightBot]);
	}
}
}
}