#pragma once

#include "Maker.h"
#include "..\StripLoop.h"
#include "StripMakerBase.h"

namespace MyEngine
{
namespace MeshMaker
{
template<typename TVertex, ModelTopology TTopology>
class StripLoopMaker
	: private StripMakerBase<TVertex, TTopology>
{
public:
	using BaseClass = StripMakerBase<TVertex, TTopology>;
	using typename BaseClass::DataType;

	using StripMakerBase<TVertex, TTopology>::StripMakerBase;

	MakerResult Make_Sharp(const StripLoop& strip);
};
template<typename TVertex, ModelTopology TTopology>
inline MakerResult StripLoopMaker<TVertex, TTopology>::Make_Sharp(const StripLoop& strip)
{
	return BaseClass::Make_Sharp(strip);
}

}
}