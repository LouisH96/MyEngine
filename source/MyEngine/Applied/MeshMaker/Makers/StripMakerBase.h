#pragma once

#include <Rendering\Mesh\MeshData.h>

#include "..\Maker.h"
#include "..\Shapes\Line.h"

namespace MyEngine
{
namespace MeshMaker
{
template<typename TVertex, ModelTopology TTopology>
class StripMakerBase
	: private Maker<TVertex, TTopology, MakerResult>
{
public:
	using BaseClass = Maker<TVertex, TTopology, MakerResult>;
	using typename BaseClass::DataType;

	StripMakerBase(MeshData<TVertex, TTopology>& meshData);

	template<typename TStrip>
	MakerResult Make_Sharp(const TStrip& strip);
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
	for (unsigned iWall{ 0 }; iWall < strip.GetNrWalls(); iWall++)
	{
		const Float3& normal{ strip.GetNormals()[iWall] };
		const unsigned leftBot{ iWall * 4 };
		const unsigned leftTop{ leftBot + 1 };
		const unsigned rightBot{ leftTop + 1 };
		const unsigned rightTop{ rightBot + 1 };
		BaseClass::AddSharpQuad(data, normal,
			leftBot, leftTop,
			rightBot, rightTop);
	}

	return BaseClass::m_Result;
}
}
}