#pragma once

#include <Geometry\ModelTopology.h>

#include "..\Results\MakerResult.h"
#include "..\Shapes\StripLoop.h"
#include "StripMakerBase.h"

namespace MyEngine
{
namespace MeshMaker
{
template<
	typename TVertex,
	ModelTopology TTopology>
class StripLoopMaker
	: private StripMakerBase<TVertex, TTopology>
{
public:
	using BaseClass = StripMakerBase<TVertex, TTopology>;
	using typename BaseClass::DataType;

	using StripMakerBase<TVertex, TTopology>::StripMakerBase;

	template<typename TStripLoop>
	MakerResult<TVertex, TTopology> Make_Sharp(const TStripLoop& strip);
};
template<
	typename TVertex,
	ModelTopology TTopology>
template<typename TStripLoop>
inline MakerResult<TVertex, TTopology> StripLoopMaker<TVertex, TTopology>::Make_Sharp(const TStripLoop& strip)
{
	return BaseClass::Make_Sharp(strip);
}

}
}