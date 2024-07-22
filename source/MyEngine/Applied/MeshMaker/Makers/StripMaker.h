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
	void TransformPhase(Array<DataType>& data, const Strip& strip);
	void AddPhase_LineList_Sharp(const Array<DataType>& data, const Strip& strip);
	void AddPhase_LineStrip_Sharp(const Array<DataType>& data, const Strip& strip);
	void AddPhase_TriangleList_Sharp(const Array<DataType>& data, const Strip& strip);
	void AddPhase_TriangleStrip_Sharp(const Array<DataType>& data, const Strip& strip);
};
template<typename Vertex, ModelTopology Topology>
inline StripMaker<Vertex, Topology>::StripMaker(MeshData<Vertex, Topology>& meshData)
	: BaseClass{ meshData }
{
}
template<typename Vertex, ModelTopology Topology>
inline MakerResult StripMaker<Vertex, Topology>::Make_Sharp(const Strip& strip)
{
	//Transform phase
	Array<DataType> data{ strip.GetNrWalls() * 4 };
	TransformPhase(data, strip);

	if constexpr (TopologyInfo::HasIndices(Topology))
		BaseClass::AddAllToResult({ data });

	//Add phase
	constexpr TopologyInfo::BaseType baseType{ TopologyInfo::GetBaseType(Topology) };

	if constexpr (baseType == TopologyInfo::BaseType::LineList)
		AddPhase_LineList_Sharp(data, strip);
	else if constexpr (baseType == TopologyInfo::BaseType::LineStrip)
		AddPhase_LineStrip_Sharp(data, strip);
	else if constexpr (baseType == TopologyInfo::BaseType::TriangleList)
		AddPhase_TriangleList_Sharp(data, strip);
	else if constexpr (baseType == TopologyInfo::BaseType::TriangleStrip)
		AddPhase_TriangleStrip_Sharp(data, strip);
	else
		Logger::PrintError("[StripMaker::Make_Sharp] unknown topology");

	return BaseClass::m_Result;
}
template<typename Vertex, ModelTopology Topology>
inline void StripMaker<Vertex, Topology>::TransformPhase(Array<DataType>& data, const Strip& strip)
{
	for (unsigned iWall{ 0 }, iData{ 0 }; iWall < strip.GetNrWalls(); iWall++)
	{
		const Strip::Edge& edgeLeft{ strip.GetEdges()[iWall] };
		const Strip::Edge& edgeRight{ strip.GetEdges()[iWall + 1] };

		data[iData++] = BaseClass::Transform(edgeLeft.Bottom.Get());
		data[iData++] = BaseClass::Transform(edgeLeft.Top.Get());
		data[iData++] = BaseClass::Transform(edgeRight.Bottom.Get());
		data[iData++] = BaseClass::Transform(edgeRight.Top.Get());
	}
}
template<typename Vertex, ModelTopology Topology>
inline void StripMaker<Vertex, Topology>::AddPhase_LineList_Sharp(const Array<DataType>& data, const Strip& strip)
{
	const unsigned nrWalls{ strip.GetNrWalls() };

	//vertical lines (wall left side)
	for (unsigned iWall{ 0 }; iWall < nrWalls; iWall++)
	{
		const Float3& normal{ strip.GetNormals()[iWall] };
		const unsigned bot{ iWall * 4 };
		const unsigned top{ bot + 1 };
		BaseClass::Add(data[bot], normal);
		BaseClass::Add(data[top], normal);
	}
	{
		//last verical line (wall right side)
		const Float3& normal{ strip.GetNormals()[nrWalls] };
		BaseClass::Add(data[(nrWalls - 1) * 4 + 2], normal);
		BaseClass::Add(data[(nrWalls - 1) * 4 + 3], normal);
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
inline void StripMaker<Vertex, Topology>::AddPhase_LineStrip_Sharp(const Array<DataType>& data, const Strip& strip)
{
	const List<Float3>& normals{ strip.GetNormals() };
	const unsigned nrWalls{ strip.GetNrWalls() };

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
	BaseClass::RemoveLast();

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
	for (unsigned iWall{ 0 }; iWall < strip.GetNrWalls(); iWall++)
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
template<typename Vertex, ModelTopology Topology>
inline void StripMaker<Vertex, Topology>::AddPhase_TriangleStrip_Sharp(const Array<DataType>& data, const Strip& strip)
{
	for (unsigned iWall{ 0 }; iWall < strip.GetNrWalls(); iWall++)
	{
		const Float3& normal{ strip.GetNormals()[iWall] };
		const unsigned leftBot{ iWall * 4 };
		const unsigned leftTop{ leftBot + 1 };
		const unsigned rightBot{ leftTop + 1 };
		const unsigned rightTop{ rightBot + 1 };
		BaseClass::Add(data[leftBot], normal);
		BaseClass::Add(data[leftTop], normal);
		BaseClass::Add(data[rightBot], normal);
		BaseClass::Add(data[rightTop], normal);
	}
}
}
}