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

	MakerResult Make_Sharp(const Strip& strip);

private:
	void AddPhase_LineList(const Array<DataType>& data, const Strip& strip);
	void AddPhase_LineStrip(const Array<DataType>& data, const Strip& strip);
	void AddPhase_TriangleList_Sharp(const Array<DataType>& data, const Strip& strip);
};
template<typename Vertex, ModelTopology Topology>
inline StripMaker<Vertex, Topology>::StripMaker(MeshData<Vertex, Topology>& meshData)
	: BaseClass{ meshData }
{
}
template<typename Vertex, ModelTopology Topology>
inline MakerResult StripMaker<Vertex, Topology>::Make_Sharp(const Strip& strip)
{
	constexpr TopologyInfo::BaseType baseType{ TopologyInfo::GetBaseType(Topology) };
	const unsigned nrEdges{ strip.GetEdges().GetSize() };
	const unsigned nrWalls{ nrEdges - 1 };

	//Transform phase
	Array<DataType> data{ nrWalls * 4 };
	for (unsigned iWall{ 0 }, iData{ 0 }; iWall < nrWalls; iWall++)
	{
		const Strip::Edge& edgeLeft{ strip.GetEdges()[iWall] };
		const Strip::Edge& edgeRight{ strip.GetEdges()[iWall + 1] };

		data[iData++] = BaseClass::Transform(edgeLeft.Bottom.Get());
		data[iData++] = BaseClass::Transform(edgeLeft.Top.Get());
		data[iData++] = BaseClass::Transform(edgeRight.Bottom.Get());
		data[iData++] = BaseClass::Transform(edgeRight.Top.Get());
	}
	if constexpr (TopologyInfo::HasIndices(Topology))
		BaseClass::AddAllToResult({ data });

	//Add phase
	if constexpr (baseType == TopologyInfo::BaseType::LineList)
		AddPhase_LineList(data, strip);
	else if constexpr (baseType == TopologyInfo::BaseType::LineStrip)
		AddPhase_LineStrip(data, strip);
	else if constexpr (baseType == TopologyInfo::BaseType::TriangleList)
		AddPhase_TriangleList_Sharp(data, strip);

	return BaseClass::m_Result;
}
template<typename Vertex, ModelTopology Topology>
inline void StripMaker<Vertex, Topology>::AddPhase_LineList(const Array<DataType>& data, const Strip& strip)
{
	const unsigned nrEdges{ strip.GetEdges().GetSize() };
	const unsigned nrWalls{ nrEdges - 1 };

	//vertical lines
	for (unsigned iWall{ 0 }; iWall < nrWalls; iWall++)
	{
		const Float3& normal{ strip.GetNormals()[iWall] };
		const unsigned bot{ iWall * 4 };
		const unsigned top{ bot + 1 };
		BaseClass::Add(data[bot], normal);
		BaseClass::Add(data[top], normal);
	}
	{
		//last verical line
		const Float3& normal{ strip.GetNormals().Last() };
		BaseClass::Add(data[data.GetSize() - 2], normal);
		BaseClass::Add(data[data.GetSize() - 1], normal);
	}

	//horizontal line(s)
	for (unsigned iWall{ 0 }; iWall < nrWalls; iWall++)
	{
		const Float3& normal{ strip.GetNormals()[iWall] };
		const unsigned leftBot{ iWall * 4 };
		const unsigned leftTop{ leftBot + 1 };
		const unsigned rightBot{ leftTop + 1 };
		const unsigned rightTop{ rightBot + 1 };
		BaseClass::Add(data[leftBot], normal);
		BaseClass::Add(data[rightBot], normal);
		BaseClass::Add(data[leftTop], normal);
		BaseClass::Add(data[rightTop], normal);
	}
}
template<typename Vertex, ModelTopology Topology>
inline void StripMaker<Vertex, Topology>::AddPhase_LineStrip(const Array<DataType>& data, const Strip& strip)
{
	const List<Float3>& normals{ strip.GetNormals() };
	const unsigned nrEdges{ strip.GetEdges().GetSize() };
	const unsigned nrWalls{ nrEdges - 1 };

	//first zig-zag
	BaseClass::Add(data.First(), normals.First());
	for (unsigned iWall{ 0 }; iWall < nrWalls; iWall++)
	{
		const Float3& normal{ strip.GetNormals()[iWall] };
		const unsigned first{ iWall * 4 + iWall % 2 };
		const unsigned second{ first + 1 - iWall % 2 * 2 };
		const unsigned third{ second + 2 };
		BaseClass::Add(data[first], normal);
		BaseClass::Add(data[second], normal);
		BaseClass::Add(data[third], normal);
	}
	//BaseClass::RemoveLast();

	//second zig-zag
	for (unsigned iWall{ nrWalls - 1 }; iWall != static_cast<unsigned>(-1); iWall--)
	{
		const Float3& normal{ strip.GetNormals()[iWall] };
		const unsigned first{ iWall * 4 + 3 - iWall % 2 };
		const unsigned second{ first - 1 + iWall % 2 * 2 };
		const unsigned third{ second - 2 };
		BaseClass::Add(data[first], normal);
		BaseClass::Add(data[second], normal);
		BaseClass::Add(data[third], normal);
	}
}
template<typename Vertex, ModelTopology Topology>
inline void StripMaker<Vertex, Topology>::AddPhase_TriangleList_Sharp(const Array<DataType>& data, const Strip& strip)
{
	const unsigned nrEdges{ strip.GetEdges().GetSize() };
	const unsigned nrWalls{ nrEdges - 1 };

	for (unsigned iWall{ 0 }; iWall < nrWalls; iWall++)
	{
		const Float3& normal{ strip.GetNormals()[iWall] };

		const unsigned leftBot{ iWall * 4 };
		const unsigned leftTop{ leftBot + 1 };
		const unsigned rightBot{ leftTop + 1 };
		const unsigned rightTop{ rightBot + 1 };
		BaseClass::Add(data[leftBot], normal); //left-bot triangle
		BaseClass::Add(data[leftTop], normal);
		BaseClass::Add(data[rightBot], normal);
		BaseClass::Add(data[leftTop], normal); //right-top triangle
		BaseClass::Add(data[rightTop], normal);
		BaseClass::Add(data[rightBot], normal);
	}
}
}
}