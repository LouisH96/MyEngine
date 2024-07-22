#pragma once

#include <Rendering\Mesh\MeshData.h>

#include "Maker.h"
#include "..\Line.h"

namespace MyEngine
{
namespace MeshMaker
{
template<typename TVertex, ModelTopology TTopology>
class StripMakerBase
	: private Maker<TVertex, TTopology>
{
public:
	using BaseClass = Maker<TVertex, TTopology>;
	using typename BaseClass::DataType;

	StripMakerBase(MeshData<TVertex, TTopology>& meshData);

	template<typename TStrip>
	MakerResult Make_Sharp(const TStrip& strip);

protected:
	template<typename TStrip>
	void AddPhase_LineList_Sharp(const Array<DataType>& data, const TStrip& strip);
	template<typename TStrip>
	void AddPhase_LineStrip_Sharp(const Array<DataType>& data, const TStrip& strip);
	template<typename TStrip>
	void AddPhase_TriangleList_Sharp(const Array<DataType>& data, const TStrip& strip);
	template<typename TStrip>
	void AddPhase_TriangleStrip_Sharp(const Array<DataType>& data, const TStrip& strip);

};

template<typename TVertex, ModelTopology TTopology>
inline StripMakerBase<TVertex, TTopology>::StripMakerBase(MeshData<TVertex, TTopology>& meshData)
	: BaseClass{ meshData }
{
}
template<typename TVertex, ModelTopology TTopology>
template<typename TStrip>
inline MakerResult StripMakerBase<TVertex, TTopology>::Make_Sharp(const TStrip& strip)
{
	//Transform phase
	Array<DataType> data{ strip.GetNrWalls() * 4 };
	for (unsigned iWall{ 0 }, iData{ 0 }; iWall < strip.GetNrWalls(); iWall++)
	{
		const Line& edgeLeft{ strip.GetEdge(iWall) };
		const Line& edgeRight{ strip.GetEdge(iWall + 1) };

		data[iData++] = BaseClass::Transform(edgeLeft[0].Get());
		data[iData++] = BaseClass::Transform(edgeLeft[1].Get());
		data[iData++] = BaseClass::Transform(edgeRight[0].Get());
		data[iData++] = BaseClass::Transform(edgeRight[1].Get());
	}
	BaseClass::FinishTransformPhase(data);

	//Add phase
	constexpr TopologyInfo::BaseType baseType{ TopologyInfo::GetBaseType(TTopology) };

	if constexpr (baseType == TopologyInfo::BaseType::LineList)
		AddPhase_LineList_Sharp(data, strip);
	else if constexpr (baseType == TopologyInfo::BaseType::LineStrip)
		AddPhase_LineStrip_Sharp(data, strip);
	else if constexpr (baseType == TopologyInfo::BaseType::TriangleList)
		AddPhase_TriangleList_Sharp(data, strip);
	else if constexpr (baseType == TopologyInfo::BaseType::TriangleStrip)
		AddPhase_TriangleStrip_Sharp(data, strip);
	else
		Logger::PrintError("[StripMakerBase::Make_Sharp] unknown topology");

	return BaseClass::m_Result;
}
template<typename TVertex, ModelTopology TTopology>
template<typename TStrip>
inline void StripMakerBase<TVertex, TTopology>::AddPhase_LineList_Sharp(const Array<DataType>& data, const TStrip& strip)
{
	const unsigned nrWalls{ strip.GetNrWalls() };

	//vertical lines (wall's left side)
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
		BaseClass::Add(data[nrWalls * 4 - 2], normal);
		BaseClass::Add(data[nrWalls * 4 - 1], normal);
	}

	//horizontal lines
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

template<typename TVertex, ModelTopology TTopology>
template<typename TStrip>
inline void StripMakerBase<TVertex, TTopology>::AddPhase_LineStrip_Sharp(const Array<DataType>& data, const TStrip& strip)
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

template<typename TVertex, ModelTopology TTopology>
template<typename TStrip>
inline void StripMakerBase<TVertex, TTopology>::AddPhase_TriangleList_Sharp(const Array<DataType>& data, const TStrip& strip)
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

template<typename TVertex, ModelTopology TTopology>
template<typename TStrip>
inline void StripMakerBase<TVertex, TTopology>::AddPhase_TriangleStrip_Sharp(const Array<DataType>& data, const TStrip& strip)
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