#pragma once

#include "..\Maker.h"
#include "..\Shapes\Strip.h"
#include "StripMakerBase.h"

namespace MyEngine
{
namespace MeshMaker
{
class StripMakerHelper;

template<typename TVertex, ModelTopology TTopology>
class StripMaker
	: private StripMakerBase<TVertex, TTopology>
{
public:
	using BaseClass = StripMakerBase<TVertex, TTopology>;
	using typename BaseClass::DataType;

	using StripMakerBase<TVertex, TTopology>::StripMakerBase;

	MakerResult<TVertex, TTopology> Make_Sharp(const Strip& strip);
};

template<typename TVertex, ModelTopology TTopology>
inline MakerResult<TVertex, TTopology> StripMaker<TVertex, TTopology>::Make_Sharp(const Strip& strip)
{
	return BaseClass::Make_Sharp(strip);
}

}
}